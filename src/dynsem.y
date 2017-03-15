%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "types.h"
#include "logging.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

typedef struct premise_list_t {
    Premise premise;
    struct premise_list_t *next;
} PremiseList;

typedef struct rule_list_t {
    Rule rule;
    PremiseList *premises;
    struct rule_list_t *next;
} RuleList;

static int _num_rules = 0;
static Rule cursor;
static RuleList rules;
void rule_append(char *left, char *right){
    _num_rules++;
    log_info("Left: %s", left);
    log_info("Right: %s", right);
}
void premise_append(char *left, char *right, PremiseType type);

char *concat(int num_strings, ...) {
    int size = 0;
    char *strings[num_strings];

    va_list vl;
    va_start(vl, num_strings);
    for (int i = 0; i < num_strings; i++) {
        strings[i] = va_arg(vl, char*);
        size += strlen(strings[i]);
    }
    va_end(vl);

    char *buffer = malloc(size + 1);
    buffer[size] = 0;
    for (int j = 0; j < num_strings; j++) {
        strcat(buffer, strings[j]);
    }
    return buffer;
}

%}

%define parse.error verbose
%define parse.lac full

%token RULES WHERE RULE_END PREMISE_END
%token ARROW MATCH EQUALS NOT_EQUALS 
%token <text> SYMBOL NUMBER STRING

%start specifications
%union {
    char *text;
}
%type <text> term term_split terms term_constr term_list term_string term_number;


%%

specifications: /* EMPTY */ | specifications specification;
specification: RULES rules { log_info("found rule set"); };
rules: rule | rules rule;
rule: term ARROW term optional_premises RULE_END { 
    log_info("found rule: %s --> %s", $1, $3);
//    rule_append($1, $3);
};

optional_premises: /* EMPTY */ | WHERE premises;
premises: premise | premises premise;
premise: equality_premise | inequality_premise | match_premise;
equality_premise: term EQUALS term PREMISE_END { log_info("found equality premise"); };
inequality_premise: term NOT_EQUALS term PREMISE_END { log_info("found inequality premise"); };
match_premise: term MATCH term PREMISE_END { log_info("found match premise"); };

term: term_split                                { $$ = $1; log_info("found term: %s", $1); };
term_split: term_constr | term_list | term_string | term_number;
terms: term | terms ',' term                    { $$ = concat(3, $1, ",", $3); };
term_constr: SYMBOL | SYMBOL '('  terms ')'     { $$ = concat(4, $1, "(", $3, ")"); };
term_list: '[' terms ']'                        { $$ = concat(3, "(", $2, ")"); };
term_string: STRING;
term_number: NUMBER;


%%
int dynsem_parse(FILE *fd){
    log_info("beginning parse: %d", fd);

    yyin = fd;
    do { 
        yyparse();
    } while(!feof(yyin));
    
    log_info("found rules: %d", _num_rules);
    return _num_rules;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}