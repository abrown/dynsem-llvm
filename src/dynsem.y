%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cii/list.h>
#include "types.h"
#include "allocation.h"
#include "logging.h"

// generated in dynsem.l
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

// helper for string concatenation
char *concat(int num_strings, ...);

// the final specification is saved here
static Specification *spec = NULL;

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

specifications: %empty | specifications specification;
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

    // setup specification
    spec = malloc(sizeof(Specification));
    spec->rules = NULL;
    spec->natives = NULL;

    // parse
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