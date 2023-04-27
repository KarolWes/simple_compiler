%option noyywrap

%{
#include "example.tab.h."
%}
DIGIT [0-9]
%%
[0-9]+ { yylval = atoi(yytext); return NUMBER; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return TIMES; }
"(" { return LPAR; }
")" { return RPAR; }
"\n" { return CR; }
. { return OTHER; }
%%