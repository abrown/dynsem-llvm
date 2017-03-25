%option noyywrap

%{

#include <stdio.h>
#include <string.h>
char *strdup(const char *s); // TODO remove this (added to get rid of warnings, should be in string.h)
#define YY_DECL int yylex()
#include "dynsem.tab.h"
#include "logging.h"

%}

%%

[ \t\n]           ; // ignore all whitespace
"rules"         {log_debug("found: rules"); return RULES;}
"native operators"      {log_debug("found: native"); return NATIVE;}
"where"         {log_debug("found: where"); return WHERE;}    
\.      	{log_debug("found: . (rule end)"); return RULE_END;}
;               {log_debug("found: ; (premise end)"); return PREMISE_END;}
[a-zA-Z][a-zA-Z0-9]*    {log_debug("found: symbol %s", yytext); yylval.text = strdup(yytext); return SYMBOL;}
\"(\\.|[^\"])*\"        {log_debug("found: string %s", yytext); yylval.text = strdup(yytext); return STRING;}
[+-]?([0-9]*\.)?[0-9]+  {log_debug("found: number"); yylval.text = strdup(yytext); return NUMBER;}
"=="            {log_debug("found: =="); return EQUALS;}
"!="            {log_debug("found: !="); return NOT_EQUALS;}
"=>"            {log_debug("found: =>"); return MATCH;}
"-->"           {log_debug("found: -->"); return ARROW;}
\{[^{}]*\}      {log_debug("found: code %s", yytext); yylval.text = strdup(yytext); return CODE;}
.               {return yytext[0];}

%%
