%option noyywrap

%{
#include "parser.tab.h"

int yylineno = 1;
%}

comment "{"[^"}"]*"}"
whitespace  [ \t]
digit [0-9]
number {digit}+

%%

{comment} {}
{whitespace}+ {}
"\n" {yylineno += 1;}

"+" return PLUS;
"-" return MINUS;
"*" return MULTI;
"/" return DIV;
"(" return OPEN;
")" return CLOSE;

{number} {
    float val = atoi(yytext);
    yylval.number = val;
    return NUM;
}


. {
    printf("\t<Line %d> Error: Unrecognised character %s\n", yylineno, yytext);
    return -1;
}
%%

//int main(){
//    int res = yylex();
//    while (res != 0){
//        printf("%d\n", res);
//        res = yylex();
//    }
//    printf("%s", yytext);
//    return 0;
//}