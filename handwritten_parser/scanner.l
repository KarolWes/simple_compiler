%option noyywrap

%{
#include <stdio.h>
#include <string.h>
#include "parser.h"

#include "enumTypes.h"
#include "ffsets.h"

int yylineno = 1;
extern int in_set(enum TokenType t, enum Nonterminal n, int include_first, int include_follow, int include_eof);
%}
comment "{"[^"}"]*"}"
whitespace  [ \t]
digit [0-9]
number "-"?{digit}+("."{digit}+)?(E("-")?{digit}+)?
alpha [A-Za-z_]
alpha_digit {alpha}|{digit}


%%

{comment} {}
{whitespace}+ {}
"\n" {yylineno += 1;}

{number} return NUM;


"." return DOT;
"," return COMA;
":" return COLON;
";" return SEMICOLON;
".." return RANGE;
"+" return PLUS;
"-" return MINUS;
"*" return MULTI;
"/" return DIV_SIGN;
"<>" return NEQ;
"<=" return LEQ;
">=" return GEQ;
":=" return ASSIGN;
"=" return EQ;
"<" return LT;
">" return GT;
"[" return BRAC_OPEN;
"]" return BRAC_CLOSE;
"(" return PARENTH_OPEN;
")" return PARENTH_CLOSE;

"and" return AND;
"or" return OR;
"not" return NOT;

"var" return VAR;
"array" return ARRAY;
"of" return OF;
"type" return TYPE;
"integer" return INTEGER;
"boolean" return BOOLEAN;
"real" return REAL;
"true" return TRUE;
"false" return FALSE;

"begin" return BEG;
"end" return END;
"function" return FUNCTION;
"procedure" return PROCEDURE;
"program" return PROGRAM;

"if" return IF;
"then" return THEN;
"else" return ELSE;
"while" return WHILE;
"do" return DO;

"div" return DIV;
"mod" return MOD;

{alpha}{alpha_digit}* {
    // printf("\t%s\n", yytext);
    return ID;
}

. {
	printf("\t<Line %d> Error: Unrecognised character %s\n", yylineno, yytext);
	return -1;
}

%%

int main(int argc, char **argv) {

    int res = start();
    if(res == 1){
        printf("Parsing complete with success\n");
    }
    else{
        printf("Error occurred while parsing\n");
    }

//    int res = yylex();
//    while (res != 0){
//        printf("%d\n", res);
//        res = yylex();
//    }
//    printf("%s", yytext);
//    return 0;
}