#include "generate.h"
#include "generate_aterms.h"
#include <aterm1.h>
#include <aterm2.h>
#include "logging.h"

void generate_headers(FILE *stream) {
    fputs("#include <aterm1.h>\n", stream);
    fputs("#include <aterm2.h>\n", stream);
    fputs("#include \"transform.h\"\n", stream);
    fputs("\n", stream);
    fflush(stream);
}

void generate_rule_table(FILE *stream, RuleTable *rules) {
    fprintf(stream, "Rule rule_table[%d];\n", rules->length);
    fputs("void build_rules() {\n", stream);

    // build placeholder
    fputs("\t AFun term_symbol = ATmakeAFun(\"term\", 0, ATfalse);\n", stream);
    fputs("\t ATermPlaceholder term_placeholder = ATmakePlaceholder((ATerm) ATmakeAppl0(term_symbol));\n", stream);

    for (int i = 0; i < rules->length; i++) {
        generate_rule_table_entry(stream, rules->rules[i]);
    }

    fputs("}\n\n", stream);
    fflush(stream);
}

void generate_rule_table_entry(FILE *stream, Rule rule) {
    fprintf(stream, "\t rule_table[%d].from = ATmake(\"", rule.id);
    ATfprintf(stream, "%t", replace_free_variables(rule.from));
    fputs("\"", stream);

    int num_vars = count_free_variables(rule.from);
    if (num_vars > 0) fputs(", ", stream);
    for (int i = 0; i <= num_vars; i++) {
        fputs("term_placeholder", stream);
        if (i < num_vars) fputs(", ", stream);
    }
    fputs(");\n", stream);

    fprintf(stream, "\t rule_table[%d].to = ATmake(\"", rule.id);
    ATfprintf(stream, "%t", replace_free_variables(rule.to));
    fputs("\"", stream);

    num_vars = count_free_variables(rule.to);
    if (num_vars > 0) fputs(", ", stream);
    for (int i = 0; i <= num_vars; i++) {
        fputs("term_placeholder", stream);
        if (i < num_vars) fputs(", ", stream);
    }

    fputs(");\n", stream);
}

int find_max_rule_args(RuleTable *rules) {
    int max = 0;
    for (int i = 0; i < rules->length; i++) {
        int c = count_free_variables(rules->rules[i].from);
        if (c > max) max = c;
    }
    return max;
}

void generate_find_function(FILE *stream, RuleTable *rules) {
    fputs("ATerm match_and_transform(ATerm before) {\n", stream);

    // allocate terms for matches
    int max_args = find_max_rule_args(rules);
    if (max_args > 0) {
        fputs("\t ATerm ", stream);
        for (int i = 0; i < max_args; i++) {
            fprintf(stream, "arg%d", i);
            if (i < max_args - 1) fputs(", ", stream);
        }
        fputs(";\n", stream);
    }

    for (int i = 0; i < rules->length; i++) {
        fputs("\t ", stream);
        generate_find_case(stream, rules->rules[i], max_args);
        fputs("\n", stream);
    }

    fputs("\t return NULL;\n", stream);
    fputs("}\n", stream);
    fflush(stream);
}

ATermList find_matching_rule_args(ATerm from, ATerm to) {
    ATermList matches = ATempty;
    ATermList from_vars = find_free_variables(from, ATempty);
    ATermList to_vars = find_free_variables(to, ATempty);
    while (!ATisEmpty(to_vars)) {
        AFun to_var = ATgetAFun(ATgetFirst(to_vars));
        ATermList _from_vars = from_vars;
        int index = 0;
        while (!ATisEmpty(_from_vars)) {
            AFun from_var = ATgetAFun(ATgetFirst(_from_vars));
            if (to_var == from_var) {
                log_debug("found matching argument for %s in 'from' rule", ATgetName(to_var));
                matches = ATinsert(matches, (ATerm) ATmakeInt(index));
                break;
            }
            index++;
            _from_vars = ATgetNext(_from_vars);
        }
        to_vars = ATgetNext(to_vars);
    }
    return matches;
}

void generate_find_case(FILE *stream, Rule rule, int max_args) {
    fprintf(stream, "if(ATmatchTerm(before, rule_table[%d].from", rule.id);
    if (max_args > 0) {
        fputs(", ", stream);
        for (int i = 0; i < max_args; i++) {
            fprintf(stream, "&arg%d", i);
            if (i < max_args - 1) fputs(", ", stream);
        }
    }

    fprintf(stream, ")) return ATmakeTerm(rule_table[%d].to", rule.id);
    ATermList matches = find_matching_rule_args(rule.from, rule.to);
    ATbool vars_exist = !ATisEmpty(matches);
    if (!ATisEmpty(matches)) {
        fputs(", ", stream);
        while (!ATisEmpty(matches)) {
            int index = ATgetInt(ATgetFirst(matches));
            fprintf(stream, "arg%d", index);
            matches = ATgetNext(matches);
            if (!ATisEmpty(matches)) fputs(", ", stream);
        }
    }
    fputs(");", stream);

    fflush(stream);
}

void generate_transform_functions(FILE *stream, RuleTable *rules) {
    fputs("\n", stream);

    for (int i = 0; i < rules->length; i++) {
        generate_transform_function(stream, rules->rules[i]);
        fputs("\n", stream);
    }

    fflush(stream);
}

void generate_transform_function(FILE *stream, Rule rule) {
    fprintf(stream, "ATerm transform_%d(ATerm from, ATerm to, ATerm before) {\n", rule.id);

    // allocate from variables: ATerm a, b, c;
    ATermList from_vars = find_free_variables(rule.from, ATempty);
    generate_variable_list(stream, from_vars, "\t ATerm ", ", ", ";\n");

    // fill in values: ATmatchTerm(before, from, &a, &b, &c);
    // TODO need error check?
    fputs("\t ATmatchTerm(before, from", stream);
    generate_variable_list(stream, from_vars, ", &", ", &", NULL); // TODO need prefix
    fputs(");\n", stream);

    // make after term from to rule: return ATmakeTerm(to, a, c);
    ATermList to_vars = find_free_variables(rule.to, ATempty);
    fputs("\t return ATmakeTerm(to", stream);
    generate_variable_list(stream, to_vars, ", ", ", ", NULL);
    fputs(");\n", stream);

    fputs("}\n", stream);
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