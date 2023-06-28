%{
	//headers
	#include <stdio.h>
	#include "exam_functions.h"
	void yyerror(char *s);
        int yylex();
        extern int yylineno;
        extern int prev_prec;
%}
%union {
	int number;
	char *string;
}

%token  PLUS MINUS DIV MULTI OPEN CLOSE
%token <number> NUM
%start  start
%type <number> expr term factor
%type <number> addOp mulOp

%%
start	: expr {printf("Value of the given expression = %d\n", $1);}
	;

expr		: term addOp expr{ $$ = simpleExprFun($1,$2,$3); }
		| term			{ $$ = $1; }
		;
term		: factor mulOp term	{ $$ = simpleExprFun($1,$2,$3); }
		| factor		{ $$ = $1; }
		;
factor		: NUM			{
						$$ = $1;
						consumed_numbers += 1;
					}
		| OPEN expr CLOSE {
							$$ = $2;
							prev_prec = 0;
						  }
addOp		: PLUS		{$$ = PLUS_OP;}
		| MINUS		{$$ = MINUS_OP;}
		;
mulOp		: MULTI		{$$ = MULTI_OP;}
		| DIV		{$$ = DIV_OP;}
		;


%%

void yyerror(char *s) {
	printf("%s\n", s);
	printf("error in line %d\n", yylineno);
}
int main() { return yyparse(); }