%{
	//headers
	#include <stdio.h>
	#include "exam_functions.h"
	void yyerror(char *s);
        int yylex();
        extern int yylineno;
%}
%union {
	int number_i;
	float number_f;
	char *string;
}

%token  PROG
%token <string> ID
%token <number_f> NUM
%start  start

%%
start	: PROG {printf("Got program token. Done\n");}
	;


%%

void yyerror(char *s) {
	printf("%s\n", s);
	printf("error in line %d\n", yylineno);
}
int main() { return yyparse(); }