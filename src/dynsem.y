%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cii/list.h>
#include "types.h"
#include "logging.h"

// generated in dynsem.l
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

static Specification *spec = NULL;

Rule *rule_allocate(char *from, char *to, List_T premises){
    log_info("allocating rule: %s --> %s (%d premises)", from, to, List_length(premises));

    Rule *rule = malloc(sizeof(Rule));
    rule->from = ATmake(from);
    rule->to = ATmake(to);
    rule->premise_list = premises;
    rule->premises_length = 0;
    rule->premises = NULL;
    return rule;
}

Native *native_allocate(char *name, char *code) {
    log_info("allocating native operator: %s", name);
    Native *native = malloc(sizeof(Native));
    native->name = name;
    native->code = code;
    return native;
}

Premise *premise_allocate(char *left, char *right, PremiseType type){
    char *ts;
    if(type == EQUALITY) ts = "==";
    else if(type == INEQUALITY) ts = "!=";
    else if(type == REDUCTION) ts = "=>";
    else ts = "?";
    log_info("allocating premise: %s %s %s", left, ts, right);

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

%code requires {
    #include <cii/list.h>
    #include "types.h"
}

%union {
    char *text;
    Rule *rule;
    Native *native;
    Premise *premise;
    List_T list;
}

%start specifications

%token RULES WHERE NATIVE RULE_END PREMISE_END
%token ARROW MATCH EQUALS NOT_EQUALS 
%token <text> SYMBOL NUMBER STRING CODE

%type <text> term term_split terms term_constr term_list term_string term_number;
%type <list> rules natives optional_premises premises 
%type <premise> premise equality_premise inequality_premise match_premise
%type <rule> rule
%type <native> native

%%

specifications: %empty {} | specifications specification {};
specification: RULES rules { spec->rules = List_append(spec->rules, $2); } 
    | NATIVE natives { spec->natives = List_append(spec->natives, $2); };

rules: rule { $$ = List_list($1, NULL); } 
    | rules rule { $$ = List_append($1, List_list($2, NULL)); };
rule: term ARROW term optional_premises RULE_END { $$ = rule_allocate($1, $3, $4); };

natives: native { $$ = List_list($1, NULL); } 
    | natives native { $$ = List_append($1, List_list($2, NULL)); };
native: SYMBOL CODE RULE_END { $$ = native_allocate($1, $2); };

optional_premises: %empty { $$ = List_list(NULL); } 
    | WHERE premises { $$ = $2; };
premises: premise { $$ = List_list($1, NULL); } 
    | premises premise { $$ = List_append($1, List_list($2, NULL)); }
premise: equality_premise | inequality_premise | match_premise;
equality_premise: term EQUALS term PREMISE_END { $$ = premise_allocate($1, $3, EQUALITY); };
inequality_premise: term NOT_EQUALS term PREMISE_END { $$ = premise_allocate($1, $3, INEQUALITY); };
match_premise: term MATCH term PREMISE_END { $$ = premise_allocate($1, $3, REDUCTION); };

term: term_split                                { $$ = $1; log_debug("found term: %s", $1); };
term_split: term_constr | term_list | term_string | term_number;
terms: term | terms ',' term                    { $$ = concat(3, $1, ",", $3); };
term_constr: SYMBOL | SYMBOL '('  terms ')'     { $$ = concat(4, $1, "(", $3, ")"); };
term_list: '[' terms ']'                        { $$ = concat(3, "(", $2, ")"); };
term_string: STRING;
term_number: NUMBER;


%%


Specification *dynsem_parse(FILE *fd){
    log_info("beginning parse: fd == %d", fd);
    spec = malloc(sizeof(Specification));
    spec->rules = NULL;
    spec->natives = NULL;

    yyin = fd;
    do { 
        yyparse();
    } while(!feof(yyin));
    
    log_info("parsed spec: %d rules, %d native operators", List_length(spec->rules), List_length(spec->natives));
    return spec;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
    exit(1);
}