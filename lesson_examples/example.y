%{
#include <stdio.h>
%}

%token  PLUS MINUS TIMES LPAR RPAR NUMBER CR OTHER
%start  input

%%

input   	:
                | input line
                ;
line            : CR
                | exp CR 		{ printf("%d\n", $1); }
                ;
exp             : exp PLUS term 	{ $$ = $1 + $3; }
                | exp MINUS term 	{ $$ = $1 - $3; }
                | term 			{ $$ = $1; }
                ;
term            : term TIMES factor 	{ $$ = $1 * $3; }
                | factor 		{ $$ = $1; }
                ;
factor  	: NUMBER  		{ $$ = $1; }
                | LPAR exp RPAR 	{ $$ = $2; }
                ;
%%
int main() { return yyparse(); }
int yyerror(char *s) { fprintf(stderr, "%s\n", s); return 0; }