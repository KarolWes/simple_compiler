%{
#include <stdio.h>
#include "parser_functions.h"

void yyerror(char *s);
int yylex();
extern int yylineno;

%}

%union {
	int number_i;
	float number_f;
	char *string;
	struct tENTRY *entry;
	struct tN_PROG *prog;
	struct tN_STMT *stmt;
	struct tN_CALL *call_stmt;
	struct tN_ASSIGN *assign_stmt;
	struct tN_IF *if_stmt;
	struct tN_WHILE *while_stmt;
	struct tN_EXPR *expr;
}

%token  AND ARRAY BEG DIV DO
            ELSE END FUNCTION IF MOD
            NOT OF OR PROCEDURE PROGRAM
            THEN VAR WHILE INTEGER
            REAL BOOLEAN TRUE FALSE COMMENT
            BRAC_OPEN BRAC_CLOSE PARENTH_OPEN PARENTH_CLOSE COLON
            SEMICOLON COMA DOT RANGE PLUS
            MINUS MULTI DIV_SIGN NEQ EQ
            LT GT LEQ GEQ ASSIGN
%token <string> ID
%token <number_f> NUM

%type <entry> identListType identList type subProgHead varDecList varDec args parList
%type <prog> subProgList
%type <stmt> compStmt stmtList stmt elsePart
%type <call_stmt> procCall
%type <assign_stmt> assignStmt
%type <if_stmt> ifStmt
%type <while_stmt> whileStmt
%type <expr> params exprList expr index simpleExpr term factor
%type <number_i> simpleType relOp addOp mulOp

%start  start

%%
start		: PROGRAM ID SEMICOLON varDec 	{ ast = (N_PROG*) malloc(sizeof (struct tN_PROG)); fprintf(stdout, "Starting\n"); startFun($2, $4); }
		subProgList 			{
						ast->next = $6;
						printf("subs -> %d\n", ast->next);
						localVars = NULL;
						}
		compStmt DOT			{
						ast->stmt = $8;
						printf("body -> %d\n", ast->stmt);
						printf("FINISHED\n");}
		;
varDec		:			{ $$ = NULL; }
		| VAR varDecList	{ $$ = $2; }
		;
varDecList	: identListType SEMICOLON varDecList	{ $$ = varDecListFun($1, $3); }
		| identListType SEMICOLON		{ $$ = $1; }
		;
identListType	: identList COLON type {$$ = intentListTypeFun($1, $3); }
		;
identList	: ID COMA identList	{ $$ = idListFun($1, $3); }
		| ID			{ $$ = idListFun($1, NULL); }
		;
type		: simpleType							{ $$ = simpleTypeFun($1, 0,0,1); }
		| ARRAY BRAC_OPEN NUM RANGE NUM BRAC_CLOSE OF simpleType	{ $$ = simpleTypeFun($8, 1, $3, $5); }
		;
simpleType	: INTEGER	{$$ = _INT;}
		| REAL		{$$ = _REAL;}
		| BOOLEAN	{$$ = _BOOL;}
		;
subProgList	:	{$$ = NULL; }
		|  subProgHead varDec { $<prog>$ = subProgListFun($1, $2); }
		compStmt SEMICOLON subProgList { $$ = $<prog>$; $$->stmt = $4; $$->next = $6; }
		;
subProgHead	: FUNCTION ID args COLON simpleType SEMICOLON		{ $$ = subHeaderFun($5, $2, $3); }
		| PROCEDURE ID args SEMICOLON				{ $$ = subHeaderFun(_VOID, $2, $3); }
		;
args		: PARENTH_OPEN PARENTH_CLOSE	{ $$ = NULL; };
		| PARENTH_OPEN parList PARENTH_CLOSE	{ $$ = $2; }
		;
parList		: identListType SEMICOLON parList	{ $$ = parListFun($1, $3); }
		| identListType				{ $$ = $1; }
		;
compStmt	: BEG stmtList END { $$ = $2; }
		;
stmtList	: stmt SEMICOLON stmtList	{
						    $1->next =  $3;
                                                    $$ = $1;
                                                }
		| stmt				{$$ = $1;}
		;
stmt		: procCall	{
                                    	N_STMT * stmt = malloc(sizeof(struct tN_STMT));
					stmt->typ = _PROC_CALL;
					stmt->node.proc_call = $1;
					stmt->next = NULL;
					$$ = stmt;
                                }
		| assignStmt	{
                                    	N_STMT * stmt = malloc(sizeof(struct tN_STMT));
					stmt->typ = _ASSIGN;
					stmt->node.assign_stmt = $1;
					stmt->next = NULL;
					$$ = stmt;
                                }
		| compStmt 	{ $$ = $1; }
		| ifStmt	{
                                    	N_STMT * stmt = malloc(sizeof(struct tN_STMT));
					stmt->typ = _IF;
					stmt->node.if_stmt = $1;
					stmt->next = NULL;
					$$ = stmt;
                                }
		| whileStmt	{
                                    	N_STMT * stmt = malloc(sizeof(struct tN_STMT));
					stmt->typ = _WHILE;
					stmt->node.while_stmt = $1;
					stmt->next = NULL;
					$$ = stmt;
				}
		;
procCall	: ID params	{ $$ = procCallFun($1, $2); }
		;
params		: PARENTH_OPEN exprList PARENTH_CLOSE 	{$$ = $2;}
		| PARENTH_OPEN PARENTH_CLOSE		{$$ = NULL;}
		;
assignStmt	: ID ASSIGN expr		{ $$ = assignmentFun($1, NULL, $3);}
		| ID index ASSIGN expr		{ $$ = assignmentFun($1, $2, $4); }
		;
index		: BRAC_OPEN expr BRAC_CLOSE		{$$ = $2;}
		| BRAC_OPEN expr RANGE expr BRAC_CLOSE	{
                                                            $2->next = $4;
                                                            $$ = $2;
                                                        }
		;
ifStmt		: IF expr THEN stmt elsePart	{
                                                    N_IF *i = malloc(sizeof (struct tN_IF));
                                                    if($2->dataType != _BOOL){
                                                    	printf("Error in line %d: condition is not bool\n", yylineno);
                                                    	exit(16);
                                                    }
                                                    i->expr = $2;
                                                    i->then_part = $4;
                                                    i->else_part = $5;
                                                    $$ = i;
                                                }
		;
elsePart	:			{$$ = NULL;}
		|  ELSE stmt		{$$ = $2;}
		;
whileStmt	: WHILE expr DO stmt	{
					    $$ = malloc(sizeof(struct tN_WHILE));
					    if($2->dataType != _BOOL){
						printf("Error in line %d: condition is not bool\n", yylineno);
						exit(16);
					    }
                                            $$->expr = $2;
                                            $$->stmt = $4;
					}
		;
exprList	: expr COMA exprList	{
                                            $1->next = $3;
                                            $$ = $1;
                                        }
		| expr			{ $$ = $1; }
		;
expr		: simpleExpr relOp simpleExpr	{ $$ = simpleExprFun($1,$2,$3); }
		| simpleExpr			{ $$ = $1; }
		;
simpleExpr	: term addOp simpleExpr{ $$ = simpleExprFun($1,$2,$3); }
		| term			{ $$ = $1; }
		;
term		: factor mulOp term	{ $$ = simpleExprFun($1,$2,$3); }
		| factor		{ $$ = $1; }
		;
factor		: NUM			{ $$ = numConversion($1); }
		| FALSE			{ $$ = booleans("false"); }
		| TRUE			{ $$ = booleans("true"); }
		| ID			{ $$ = identifiers($1, NULL, 0); }
		| ID index		{ $$ = identifiers($1, $2, 1); }
		| ID params		{ $$ = identifiers($1, $2, 2); }
		| NOT factor		{ $$ = simpleExprFun($2, NOT_OP, NULL); }
		| PARENTH_OPEN expr PARENTH_CLOSE {
							$$ = $2;
							$$->parenthesis = 1;
						  }
		;
relOp		: LT		{$$ = LT_OP;}
		| GT		{$$ = GT_OP;}
		| EQ		{$$ = EQ_OP;}
		| NEQ		{$$ = NEQ_OP;}
		| GEQ		{$$ = GEQ_OP;}
		| LEQ		{$$ = LEQ_OP;}
		;
addOp		: PLUS		{$$ = PLUS_OP;}
		| MINUS		{$$ = MINUS_OP;}
		| OR		{$$ = OR_OP;}
		;
mulOp		: MULTI		{$$ = MULTI_OP;}
		| DIV_SIGN	{$$ = SLASH_OP;}
		| DIV		{$$ = DIV_OP;}
		| MOD		{$$ = MOD_OP;}
		| AND		{$$ = AND_OP;}
		;
%%

void yyerror(char *s) {
	printf("%s\n", s);
	printf("error in line %d\n", yylineno);
}
//int main() { return yyparse(); }


