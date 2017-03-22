%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cii/list.h>
#include "types.h"
#include "logging.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

static List_T spec = NULL; // TODO figure out how to do this with yylval

Rule *rule_allocate(char *from, char *to, List_T premises){
    Rule *rule = malloc(sizeof(Rule));
    rule->from = ATmake(from);
    rule->to = ATmake(to);
    rule->premise_list = premises;
    rule->premises_length = 0;
    rule->premises = NULL;
    return rule;
}

Premise *premise_allocate(char *left, char *right, PremiseType type){
    Premise *premise = malloc(sizeof(Premise));
    premise->left = ATmake(left);
    premise->right = ATmake(right);
    premise->type = type;
    return premise;
}

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
%code requires {
    #include <cii/list.h>
    #include "types.h"
}
%union {
    char *text;
    Rule *rule;
    Premise *premise;
    List_T list;
}
%type <text> term term_split terms term_constr term_list term_string term_number;
%type <list> specification specifications rules optional_premises premises 
%type <premise> premise equality_premise inequality_premise match_premise
%type <rule> rule

%%


specifications: %empty { $$ = List_list(NULL); spec = $$; } 
    | specifications specification { $$ = List_append($1, $2); spec = $$; };
specification: RULES rules { $$ = List_append($$, $2); };
rules: rule { $$ = List_list($1, NULL); } 
    | rules rule { $$ = List_append($1, List_list($2, NULL)); };
rule: term ARROW term optional_premises RULE_END { 
    log_info("found rule: %s --> %s (%d premises)", $1, $3, List_length($4));
    $$ = rule_allocate($1, $3, $4);
};

optional_premises: %empty { $$ = List_list(NULL); log_info("creating empty list"); } 
    | WHERE premises { $$ = $2; };
premises: premise { $$ = List_list($1, NULL); log_info("creating list"); } 
    | premises premise { $$ = List_append($1, List_list($2, NULL)); log_info("appending to list"); }
premise: equality_premise | inequality_premise | match_premise;
equality_premise: term EQUALS term PREMISE_END { log_info("found equality premise"); $$ = premise_allocate($1, $3, EQUALITY); };
inequality_premise: term NOT_EQUALS term PREMISE_END { log_info("found inequality premise"); $$ = premise_allocate($1, $3, INEQUALITY); };
match_premise: term MATCH term PREMISE_END { log_info("found match premise"); $$ = premise_allocate($1, $3, REDUCTION); };

term: term_split                                { $$ = $1; log_info("found term: %s", $1); };
term_split: term_constr | term_list | term_string | term_number;
terms: term | terms ',' term                    { $$ = concat(3, $1, ",", $3); };
term_constr: SYMBOL | SYMBOL '('  terms ')'     { $$ = concat(4, $1, "(", $3, ")"); };
term_list: '[' terms ']'                        { $$ = concat(3, "(", $2, ")"); };
term_string: STRING;
term_number: NUMBER;


%%


List_T dynsem_parse(FILE *fd){
    log_info("beginning parse: %d", fd);

    yyin = fd;
    do { 
        yyparse();
    } while(!feof(yyin));
    
    int num_rules = List_length(spec);
    log_info("found rules: %d", num_rules);
    return spec;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
    exit(1);
}