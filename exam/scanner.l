%option noyywrap

%{
#include "parser.tab.h"

int yylineno = 1;
%}

comment "{"[^"}"]*"}"
whitespace  [ \t]
digit [0-9]
number "-"?{digit}+("."{digit}+)?
alpha [A-Za-z_]
alpha_digit {alpha}|{digit}

%%

{comment} {}
{whitespace}+ {}
"\n" {yylineno += 1;}

"program" {return PROG;}

{number} {
    float val = atof(yytext);
    yylval.number_f = val;
    return NUM;
}

{alpha}{alpha_digit}* {
    yylval.string = strdup(yytext);
    return ID;
}

. {
    printf("\t<Line %d> Error: Unrecognised character %s\n", yylineno, yytext);
    return -1;
}
%%
