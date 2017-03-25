#include <stdlib.h>
#include "allocation.h"
#include "logging.h"

Rule *rule_allocate(char *from, char *to, List_T premises) {
    log_info("allocating rule: %s --> %s (%d premises)", from, to, List_length(premises));

    Rule *rule = malloc(sizeof (Rule));
    rule->from = ATmake(from);
    rule->to = ATmake(to);
    rule->premises = premises;
    rule->index = -1;
    return rule;
}

Native *native_allocate(char *name, char *code) {
    log_info("allocating native operator: %s", name);
    Native *native = malloc(sizeof (Native));
    native->name = name;
    native->code = code;
    return native;
}

Premise *premise_allocate(char *left, char *right, PremiseType type) {
    char *ts;
    if (type == EQUALITY) ts = "==";
    else if (type == INEQUALITY) ts = "!=";
    else if (type == REDUCTION) ts = "=>";
    else ts = "?";
    log_info("allocating premise: %s %s %s", left, ts, right);

    Premise *premise = malloc(sizeof (Premise));
    premise->left = ATmake(left);
    premise->right = ATmake(right);
    premise->type = type;
    premise->index = -1;
    return premise;
}
