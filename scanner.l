%option noyywrap

%{
#include <stdio.h>
#include <string.h>
    enum TokenType{AND=1, ARRAY, BEG, DIV, DO, ELSE, END,
                 FUNCTION, IF, MOD, NOT, OF, OR, PROCEDURE,
                 PROGRAM,  THEN, TYPE, VAR, WHILE,
				 INTEGER, REAL, BOOLEAN,
				 TRUE, FALSE,
                  COMMENT,
                  BRAC_OPEN, BRAC_CLOSE, PARENTH_OPEN, PARENTH_CLOSE,
                  COLON, SEMICOLON, COMA, DOT, RANGE, PLUS, MINUS,
                  MULTI, DIV_SIGN, NEQ, EQ, LT, GT, LEQ, GEQ, ASSIGN,
                  ID=101, NUM=100};

%}
comment "{"[^"}"]*"}"
whitespace  [ \t\n]
digit [0-9]
number "-"?{digit}+("."{digit}+)?(E("-")?{digit}+)?
alpha [A-Za-z_]
alpha_digit {alpha}|{digit}


%%

{comment} {}
{whitespace}+ {}

{number} return NUM;


"." return DOT;
"," return COMA;
";" return COLON;
":" return SEMICOLON;
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

{alpha}{alpha_digit}* return ID;

. {
	printf("\tError: Unrecognised character %s\n", yytext);
	return -1;
}

%%

int main(int argc, char **argv) {

//    int res = yylex();
//    while (res != 0){
//        printf("%d\n", res);
//        res = yylex();
//    }
//    printf("%s", yytext);
//    return 0;
    yylex();
}