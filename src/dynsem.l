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

[ \t]           ; // ignore all whitespace
"rules"         {log_info("found: rules"); return RULES;}
"where"         {log_info("found: where"); return WHERE;}    
\.      	{log_info("found: . (rule end)"); return RULE_END;}
;               {log_info("found: ; (premise end)"); return PREMISE_END;}
[a-zA-Z][a-zA-Z0-9]*    {log_info("found: symbol %s", yytext); yylval.text = strdup(yytext); return SYMBOL;}
\"(\\.|[^\"])*\"        {log_info("found: string %s", yytext); yylval.text = strdup(yytext); return STRING;}
[+-]?([0-9]*\.)?[0-9]+  {log_info("found: number"); yylval.text = strdup(yytext); return NUMBER;}
==              {log_info("found: =="); return EQUALS;}
"!="            {log_info("found: !="); return NOT_EQUALS;}
"=>"            {log_info("found: =>"); return MATCH;}
"-->"           {log_info("found: -->"); return ARROW;}
.               {return yytext[0];}

%%
