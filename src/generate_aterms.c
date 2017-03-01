#include "generate.h"
#include "generate_aterms.h"
#include <aterm1.h>
#include <aterm2.h>
#include "log.h"

void generate_headers(FILE *stream) {
    fputs("#include <aterm1.h>\n", stream);
    fputs("#include <aterm2.h>\n", stream);
    fflush(stream);
}

void generate_transform_function(FILE *stream, Rule rule, int transform_id) {
    fprintf(stream, "ATerm transform_%d(ATerm rule_from, ATerm rule_to, ATerm before) {", transform_id);
    
    // allocate from variables: ATerm a, b, c;
    fputs("\t ATerm ", stream);
    ATermList from_vars = find_free_variables(rule.from, ATempty);
    generate_variable_list(stream, ", ", from_vars);
    fputs(";\n", stream);
    
    // fill in values: ATmatchTerm(before, rule_table[42].from_pattern, &a, &b, &c);
    // TODO need error check?
    fprintf(stream, "\t ATmatchTerm(before, rule_table[%d].from_pattern", rule.id);
    generate_variable_list(stream, ", &", from_vars); // TODO need prefix
    fputs(");\n", stream);
    
    // make after term from to rule: return ATmakeTerm(rule_table[42].to_pattern, a, c);
    ATermList to_vars = find_free_variables(rule.from, ATempty);
    fprintf(stream, "\t return ATmakeTerm(rule_table[%d].to_pattern", rule.id);
    generate_variable_list(stream, ", ", to_vars);
    fputs(");\n", stream);
    
    fputs("}\n", stream);
    fflush(stream);
}

ATermList find_free_variables(ATerm term, ATermList collector) {
    switch (ATgetType(term)) {
        case AT_APPL:
            trace("appl");
            AFun fun = ATgetAFun((ATermAppl) term);
            int arity = ATgetArity(fun);
            if (arity == 0) {
                return ATinsert(collector, term);
            } else {
                for (int i = 0; i < arity; i++) {
                    collector = find_free_variables(ATgetArgument((ATermAppl) term, i), collector);
                }
            }
            break;
        case AT_LIST:
            trace("list");
            int length = ATgetLength((ATermList) term);
            for (int i = 0; i < length; i++) {
                collector = find_free_variables(ATelementAt((ATermList) term, i), collector);
            }
            break;
        default:
            // only lists and applications can contain 0-arity functions
            break;
    }
    return ATreverse(collector);
}

void generate_variable_list(FILE *stream, char *delimiter, ATermList vars) {
    char *name;
    while (!ATisEmpty(vars)) {
        name = ATgetName(ATgetAFun(ATgetFirst(vars)));
        fputs(name, stream);
        vars = ATgetNext(vars);
        if (!ATisEmpty(vars)) fputs(delimiter, stream);
    }
    fflush(stream);
}

ATerm replace_free_variables(ATerm rule) {
    switch (ATgetType(rule)) {
        case AT_APPL:
            trace("appl");
            AFun fun = ATgetAFun((ATermAppl) rule);
            int arity = ATgetArity(fun);
            if (arity == 0) {
                // generate and return "<term>"
                AFun term = ATmakeAFun("term", 0, ATfalse);
                ATermPlaceholder term_placeholder = ATmakePlaceholder((ATerm) ATmakeAppl0(term));
                return (ATerm) term_placeholder;
            } else {
                for (int i = 0; i < arity; i++) {
                    ATerm arg = ATgetArgument(rule, i);
                    ATerm new = replace_free_variables(arg);
                    if (arg != new) rule = (ATerm) ATsetArgument((ATermAppl) rule, new, i);
                }
            }
            break;
        case AT_LIST:
            trace("list");
            int length = ATgetLength((ATermList) rule);
            for (int i = 0; i < length; i++) {
                ATerm arg = ATelementAt((ATermList) rule, i);
                ATerm new = replace_free_variables(arg);
                if (arg != new) rule = (ATerm) ATreplace((ATermList) rule, new, i);
            }
            break;
        case AT_INT:
        case AT_REAL:
        case AT_BLOB:
        case AT_SYMBOL:
            return rule;
            break;
        case AT_PLACEHOLDER:
            ATwarning("Found placeholder in rule, continuing: %t", rule); // TODO use logger
            return rule;
            break;
        case AT_FREE:
        default:
            ATerror("Found unknown or unhandleable type: %t", rule); // TODO use logger
            exit(1);
            break;
    }
    return rule;
}

void generate_transform_allocation_with_aterm(FILE *stream, ATerm rule) {
    fputs("ATerm ", stream);
    ATermList vars = find_free_variables(rule, ATempty);
    generate_variable_list(stream, ", ", vars);
    fputs(";\n", stream);
    fflush(stream);
}

//void stream_write_line(Stream stream, char *string);
//
//void generate_find_rule() {
//    int num_rules = 1;
//    Rule rule_table[num_rules];
//    rule_table[0]->from = ATmake("a(x, y, z)");
//    rule_table[0]->to = ATmake("b(y)");
//
//    stream_write_line(stream, "#include <aterm1.h>");
//    stream_write_line(stream, "#include <aterm2.h>");
//    stream_write_line(stream, "ATerm find(ATerm before) {");
//    for (int i = 0; i < num_rules; i++) {
//        stream_write_line(stream, "\t if(ATmatch(rule_table[" i "]->from_pattern, before)) return transform_" i "(rule_table[" i "], before);");
//        //        rule_table[i]->from_pattern = replace_free_variables(rule_table[i]->from);
//        //        write_free_variables(rule_table[0]->from);
//    }
//    stream_write_line(stream, "\t return error();");
//    stream_write_line(stream, "}");
//}