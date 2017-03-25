#include "generate.h"
#include "generate_aterms.h"
#include <aterm1.h>
#include <aterm2.h>
#include "logging.h"

#define TAB "    "
#define NEWLINE "\n"

void assign_indices(Specification *spec) {
    // assign indices to rules
    int i = 0;
    for (List_T rules = spec->rules; rules; rules = rules->rest, i++) {
        Rule *r = (Rule *) rules->first;
        r->index = i;
        // assign indices to premises
        int j = 0;
        for (List_T premises = r->premises; premises; premises = premises->rest, j++) {
            Premise *p = (Premise *) premises->first;
            p->index = j;
        }
    }
}

void generate_headers(FILE *stream) {
    fputs("#include <aterm1.h>" NEWLINE, stream);
    fputs("#include <aterm2.h>" NEWLINE, stream);
    fputs("#include \"transform.h\"" NEWLINE, stream);
    fputs(NEWLINE, stream);
    fflush(stream);
}

void generate_rule_table(FILE *stream, Specification *spec) {
    // build global variables
    fprintf(stream, "Rule rule_table[%d];" NEWLINE, List_length(spec->rules));
    for (List_T rules = spec->rules; rules; rules = rules->rest) {
        Rule *r = (Rule *) rules->first;
        int num_premises = List_length(r->premises);
        if (num_premises > 0) {
            fprintf(stream, "Premise rule_%d_premises[%d];" NEWLINE, r->index, num_premises);
        }
    }

    // generate rule setup function
    fputs("void build_rules() {" NEWLINE, stream);

    // build placeholder
    fputs(TAB "AFun term_symbol = ATmakeAFun(\"term\", 0, ATfalse);" NEWLINE, stream);
    fputs(TAB "ATermPlaceholder term_placeholder = ATmakePlaceholder((ATerm) ATmakeAppl0(term_symbol));" NEWLINE, stream);
    fputs(NEWLINE, stream);

    // setup each rule and premises
    for (List_T rules = spec->rules; rules; rules = rules->rest) {
        generate_rule_table_entry(stream, (Rule *) rules->first);
    }

    fputs("}" NEWLINE NEWLINE, stream);
    fflush(stream);
}

void generate_patternized_make(FILE *stream, ATerm term) {
    ATfprintf(stream, "ATmake(\"%t\"", replace_free_variables(term));
    int num_vars = count_free_variables(term);
    if (num_vars > 0) fputs(", ", stream);
    for (int i = 0; i < num_vars; i++) {
        fputs("term_placeholder", stream);
        if (i < num_vars - 1) fputs(", ", stream);
    }
    fputs(");" NEWLINE, stream);

    fflush(stream);
}

void generate_rule_table_entry(FILE *stream, Rule *rule) {
    // build 'from' pattern
    fprintf(stream, TAB "rule_table[%d].from = ", rule->index);
    generate_patternized_make(stream, rule->from);

    // build 'to' pattern
    fprintf(stream, TAB "rule_table[%d].to = ", rule->index);
    generate_patternized_make(stream, rule->to);

    // build 'premises'
    if (List_length(rule->premises) > 0) {
        for (List_T premises = rule->premises; premises; premises = premises->rest) {
            Premise *premise = (Premise *) premises->first;
            fprintf(stream, TAB "rule_%d_premises[%d].type = %d;" NEWLINE, rule->index, premise->index, premise->type);
            fprintf(stream, TAB "rule_%d_premises[%d].left = ", rule->index, premise->index);
            generate_patternized_make(stream, premise->left);
            fprintf(stream, TAB "rule_%d_premises[%d].right = ", rule->index, premise->index);
            generate_patternized_make(stream, premise->right);
            fprintf(stream, TAB "rule_%d_premises[%d].index = %d;" NEWLINE, rule->index, premise->index, premise->index);
        }
    }

    fputs(NEWLINE, stream);
    fflush(stream);
}

void generate_match_function(FILE *stream, Specification *spec) {
    fputs("ATerm match_and_transform(ATerm before) {" NEWLINE, stream);

    // allocate out variable (TODO will putting this on the stack be a mistake even with aterms?)
    fputs(TAB "ATerm after;" NEWLINE, stream);

    // match
    fputs(TAB "ATbool matched = ", stream);
    for (List_T natives = spec->natives; natives; natives = natives->rest) {
        fprintf(stream, "%s(before, &after)", ((Native *) natives->first)->name);
        if (natives->rest != NULL) fputs(" || ", stream);
    }
    if(List_length(spec->natives) > 0 && List_length(spec->rules) > 0) fputs(" || " NEWLINE TAB TAB, stream);
    for (List_T rules = spec->rules; rules; rules = rules->rest) {
        fprintf(stream, "transform_rule_%d(before, &after)", ((Rule *) rules->first)->index);
        if (rules->rest != NULL) fputs(" || ", stream);
    }
    fputs(";" NEWLINE, stream);

    // return
    fputs(TAB "if(matched) return after;" NEWLINE, stream);
    fputs(TAB "else return NULL;" NEWLINE, stream);

    fputs("}" NEWLINE, stream);
    fflush(stream);
}

void generate_native_functions(FILE *stream, Specification *spec) {
    fputs(NEWLINE, stream);

    for (List_T natives = spec->natives; natives; natives = natives->rest) {
        generate_native_function(stream, (Native *) natives->first);
        fputs(NEWLINE, stream);
    }

    fflush(stream);
}

void generate_native_function(FILE *stream, Native *native) {
    // write signature
    fprintf(stream, "ATbool %s(ATerm before, ATerm *after) ", native->name);
    
    // write body
    fprintf(stream, "%s", native->code);
    
    fputs(NEWLINE, stream);
    fflush(stream);
}

void generate_rule_functions(FILE *stream, Specification *spec) {
    fputs(NEWLINE, stream);

    for (List_T rules = spec->rules; rules; rules = rules->rest) {
        generate_rule_function(stream, (Rule *) rules->first);
        fputs(NEWLINE, stream);
    }

    fflush(stream);
}

void generate_rule_function(FILE *stream, Rule *rule) {
    // setup memory location of premise
    char rule_location[1024];
    snprintf(rule_location, 1024, "rule_table[%d]", rule->index);

    // write signature
    fprintf(stream, "ATbool transform_rule_%d(ATerm before, ATerm *after) {" NEWLINE, rule->index);

    // allocate from variables: ATerm a, b, c;
    ATermList from_vars = find_free_variables(rule->from, ATempty);
    generate_variable_list(stream, from_vars, TAB "ATerm ", ", ", ";" NEWLINE);

    // fill in values using 'from' rule: ATmatchTerm(before, from, &a, &b, &c);
    fputs(TAB "// initial match" NEWLINE, stream);
    fprintf(stream, TAB "ATbool rc = ATmatchTerm(before, %s.from", rule_location);
    generate_variable_list(stream, from_vars, ", &", ", &", NULL);
    fputs(");" NEWLINE, stream);

    // return failed to match
    fputs(TAB "if(!rc) return ATfalse;" NEWLINE, stream);

    // evaluate premises
    for (List_T premises = rule->premises; premises; premises = premises->rest) {
        generate_premise(stream, rule, (Premise *) premises->first);
    }

    // build term from 'to' rule: return ATmakeTerm(to, a, c);
    fputs(TAB "// final transform" NEWLINE, stream);
    ATermList to_vars = find_free_variables(rule->to, ATempty);
    fprintf(stream, TAB "*after = ATmakeTerm(%s.to", rule_location);
    generate_variable_list(stream, to_vars, ", ", ", ", NULL);
    fputs(");" NEWLINE, stream);

    // return matched
    fputs(TAB "return ATtrue;" NEWLINE, stream);

    fputs("}" NEWLINE, stream);
    fflush(stream);
}

void generate_premise(FILE *stream, Rule *rule, Premise *premise) {
    // setup memory location of premise
    char premise_location[1024];
    snprintf(premise_location, 1024, "rule_%d_premises[%d]", rule->index, premise->index);

    // helpfully document
    fprintf(stream, TAB "// premise #%d" NEWLINE, premise->index);

    // build left-hand side
    fprintf(stream, TAB "ATerm p%d = match_and_transform(ATmakeTerm(%s.left", premise->index, premise_location);
    generate_variable_list(stream, find_free_variables(premise->left, ATempty), ", ", ", ", "));" NEWLINE);
    fprintf(stream, TAB "if (p%d == NULL) return ATfalse;" NEWLINE, premise->index);

    // match right-hand side
    switch (premise->type) {
        case REDUCTION:
            log_debug("generating reduction premise: %t --> %t", premise->left, premise->right);
            ATermList premise_free_vars = find_free_variables(premise->right, ATempty);
            generate_variable_list(stream, premise_free_vars, TAB "ATerm ", ", ", ";" NEWLINE);
            fprintf(stream, TAB "rc = ATmatchTerm(p%d, %s.right", premise->index, premise_location);
            generate_variable_list(stream, premise_free_vars, ", &", ", &", ");" NEWLINE);
            fputs(TAB "if(!rc) return ATfalse;" NEWLINE, stream);
            break;
        case EQUALITY:
            log_debug("generating equality premise: %t == %t", premise->left, premise->right);
            fprintf(stream, TAB "if(!ATisEqual(p%d, %s.right)) return ATfalse;" NEWLINE, premise->index, premise_location);
            break;
        case INEQUALITY:
            log_debug("generating inequality premise: %t != %t", premise->left, premise->right);
            fprintf(stream, TAB "if(ATisEqual(p%d, %s.right)) return ATfalse;" NEWLINE, premise->index, premise_location);
            break;
    }

    fflush(stream);
}

void generate_variable_list(FILE *stream, ATermList vars, char *prefix, char *delimiter, char *suffix) {
    char *name;
    ATbool vars_exist = !ATisEmpty(vars);
    if (vars_exist && prefix != NULL) fputs(prefix, stream);
    while (!ATisEmpty(vars)) {
        name = ATgetName(ATgetAFun(ATgetFirst(vars)));
        fputs(name, stream);
        vars = ATgetNext(vars);
        if (!ATisEmpty(vars) && delimiter != NULL) fputs(delimiter, stream);
    }
    if (vars_exist && suffix != NULL) fputs(suffix, stream);
    fflush(stream);
}

ATerm iterate_free_variables(ATerm term, free_variables_cb callback, void *callback_data) {
    switch (ATgetType(term)) {
        case AT_APPL:
            log_debug("iterating, found function application");
            AFun fun = ATgetAFun((ATermAppl) term);
            int arity = ATgetArity(fun);
            if (arity == 0) {
                log_debug("iterating, found free variable: %s", ATgetName(fun));
                return callback(term, callback_data);
            } else {
                for (int i = 0; i < arity; i++) {
                    ATerm old = ATgetArgument((ATermAppl) term, i);
                    ATerm new = iterate_free_variables(old, callback, callback_data);
                    if (new != NULL && old != new) term = (ATerm) ATsetArgument((ATermAppl) term, new, i);
                }
            }
            break;
        case AT_LIST:
            log_debug("iterating, found list");
            int length = ATgetLength((ATermList) term);
            for (int i = 0; i < length; i++) {
                ATerm old = ATelementAt((ATermList) term, i);
                ATerm new = iterate_free_variables(old, callback, callback_data);
                if (new != NULL && old != new) term = (ATerm) ATreplace((ATermList) term, new, i);
            }
            break;
        default:
            // only lists and applications can contain 0-arity functions
            break;
    }
    return term;
}

ATerm find_free_variables_callback(ATerm current, void *_collector) {
    ATermList *collector = (ATermList *) _collector;
    *collector = ATappend(*collector, current);
    return NULL;
}

ATermList find_free_variables(ATerm term, ATermList collector) {
    iterate_free_variables(term, find_free_variables_callback, &collector);
    return collector;
}

ATerm replace_free_variables_callback(ATerm current, void *nothing) {
    // generate and return "<term>"
    AFun term = ATmakeAFun("term", 0, ATfalse);
    ATermPlaceholder term_placeholder = ATmakePlaceholder((ATerm) ATmakeAppl0(term));
    return (ATerm) term_placeholder;
}

ATerm replace_free_variables(ATerm rule) {
    return iterate_free_variables(rule, replace_free_variables_callback, NULL);
}

ATerm count_free_variables_callback(ATerm current, void *_count) {
    int *count = (int *) _count;
    (*count)++;
    return NULL;
}

int count_free_variables(ATerm rule) {
    int count = 0;
    iterate_free_variables(rule, count_free_variables_callback, &count);
    return count;
}