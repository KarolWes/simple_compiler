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
start		: PROGRAM ID SEMICOLON varDec subProgList compStmt DOT	{ startFun($2, $4, $5, $6); }
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
type		: simpleType							{ $$ = simpleTypeFun($1, 0,0,0); }
		| ARRAY BRAC_OPEN NUM RANGE NUM BRAC_CLOSE OF simpleType	{ $$ = simpleTypeFun($8, 1, $3, $5); }
		;
simpleType	: INTEGER	{$$ = _INT;}
		| REAL		{$$ = _REAL;}
		| BOOLEAN	{$$ = _BOOL;}
		;
subProgList	:	{$$ = NULL; }
		|  subProgHead varDec compStmt SEMICOLON subProgList { $$ = subProgListFun($1, $2, $3, $5); }
		;
subProgHead	: FUNCTION ID args COLON simpleType SEMICOLON		{ $$ = subHeaderFun($5, $2, $3); }
		| PROCEDURE ID args SEMICOLON				{ $$ = subHeaderFun(_VOID, $2, $3); }
		;
args		:					{ $$ = NULL; };
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
					$$ = stmt;
                                }
		| assignStmt	{
                                    	N_STMT * stmt = malloc(sizeof(struct tN_STMT));
					stmt->typ = _ASSIGN;
					stmt->node.assign_stmt = $1;
					$$ = stmt;
                                }
		| compStmt 	{ $$ = $1; }
		| ifStmt	{
                                    	N_STMT * stmt = malloc(sizeof(struct tN_STMT));
					stmt->typ = _IF;
					stmt->node.if_stmt = $1;
					$$ = stmt;
                                }
		| whileStmt	{
                                    	N_STMT * stmt = malloc(sizeof(struct tN_STMT));
					stmt->typ = _WHILE;
					stmt->node.while_stmt = $1;
					$$ = stmt;
				}
		;
procCall	: ID params	{
                                    N_CALL* call = malloc(sizeof (struct tN_CALL));
                                    call->id = $1;
                                    call->par_list = $2;
                                    $$ = call;
                                }
		;
params		: PARENTH_OPEN exprList PARENTH_CLOSE 	{$$ = $2;}
		| PARENTH_OPEN PARENTH_CLOSE		{$$ = NULL;}
		;
assignStmt	: ID ASSIGN expr		{
						N_VAR_REF* var = malloc(sizeof(struct tN_VAR_REF));
						var->id = $1;
						var->index = NULL;
						N_ASSIGN* assign = malloc(sizeof (struct tN_ASSIGN));
						assign->var_ref = var;
						assign->rhs_expr = $3;
						$$ = assign;
						}
		| ID index ASSIGN expr		{
                                                    N_VAR_REF* var = malloc(sizeof(struct tN_VAR_REF));
                                                    var->id = $1;
                                                    var->index = $2;
                                                    N_ASSIGN* assign = malloc(sizeof (struct tN_ASSIGN));
                                                    assign->var_ref = var;
                                                    assign ->rhs_expr = $4;
                                                    $$ = assign;
                                                }
		;
index		: BRAC_OPEN expr BRAC_CLOSE		{$$ = $2;}
		| BRAC_OPEN expr RANGE expr BRAC_CLOSE	{
                                                            $2->next = $4;
                                                            $$ = $2;
                                                        }
		;
ifStmt		: IF expr THEN stmt elsePart	{
                                                    N_IF *i = malloc(sizeof (struct tN_IF));
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
factor		: NUM			{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            expr->typ = CONSTANT;
                                            float num = $1;
					    int len = snprintf(NULL, 0, "%f", num);
					    char* res = (char *)(malloc(len + 1));
					    snprintf(res, len+1, "%f", num);
                                            expr->description.constant = res;
                                            expr->parenthesis = 0;
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| FALSE			{ $$ = booleans("false"); }
		| TRUE			{ $$ = booleans("true"); }
		| ID			{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            N_VAR_REF * var = malloc(sizeof(struct tN_VAR_REF));
                                            var->id = $1;
                                            var->index = NULL;
                                            expr->typ = VAR_REF;
                                            expr->description.var_ref = var;
                                            expr->parenthesis = 0;
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| ID index		{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            N_VAR_REF * var = malloc(sizeof(struct tN_VAR_REF));
                                            var->id = $1;
                                            var->index = $2;
                                            expr->typ = VAR_REF;
                                            expr->description.var_ref = var;
                                            expr->parenthesis = 0;
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| ID params		{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            N_CALL * call = malloc(sizeof(struct tN_CALL));
                                            call->id = $1;
                                            call->par_list = $2;
                                            expr->typ = FUNC_CALL;
                                            expr->description.func_call = call;
                                            expr->parenthesis = 0;
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| NOT factor		{
                                            N_EXPR * expr =  malloc(sizeof(struct tN_EXPR));
                                            expr->typ = OP;
                                            expr->description.operation.op = NOT_OP;
                                            expr->description.operation.expr = $2;
                                            expr->parenthesis = $2->parenthesis;
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
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
	fprintf(stderr, "%s\n", s);
	printf("error in line %d\n", yylineno);
}


