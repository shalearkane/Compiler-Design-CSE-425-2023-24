%{
#include <stdio.h>
#include "y.tab.h"
%}

%option noyywrap

%%

"int" { return INT_KEYWORD; }
"[" { return OPEN_BRACKET; }
"]" { return CLOSE_BRACKET; }
"," { return COMMA; }
";" { return SEMICOLON; }
"=" { return ASSIGN_OP;}
[a-zA-Z][a-zA-Z0-9]* { yylval.str = strdup(yytext); return IDENTIFIER; }
[0-9]+ { yylval.str = strdup(yytext); return INT; }
[ \t\n] { /* ignore whitespace */ }
. { printf("Unexpected character: %s\n", yytext); exit(1); }

%%
