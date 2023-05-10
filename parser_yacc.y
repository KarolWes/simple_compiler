%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int yyerror(char *s);
extern int yylex();
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
%token <string> ID NUM

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
start		: PROGRAM ID SEMICOLON varDec subProgList compStmt DOT	{
                                                                            ast = (N_PROG*) malloc(sizeof (struct tN_PROG));
                                                                            ast->stmt = $6;
                                                                            ast->next = NULL;
                                                                        }
		;
varDec		:
		| VAR varDecList
		;
varDecList	: identListType SEMICOLON varDecList
		| identListType SEMICOLON
		;
identListType	: identList COLON type {}
		;
identList	: ID COMA identList	{
//			$$ = malloc(sizeof (struct tEntry));
//			$$->base.id = $1;
//			$$->next = $3;
}
		| ID
		;
type		: simpleType
		| ARRAY BRAC_OPEN NUM RANGE NUM BRAC_CLOSE OF simpleType
		;
simpleType	: INTEGER	{$$ = 'i';}
		| REAL		{$$ = 'f';}
		| BOOLEAN	{$$ = 'b';}
		;
subProgList	:
		|  subProgHead varDec compStmt SEMICOLON subProgList {
                                                                         N_PROG* prog = malloc(sizeof (struct tN_PROG));
                                                                         prog->stmt = $3;
                                                                         prog->next = $5;
                                                                         $$ = prog;
                                                                     }
		;
subProgHead	: FUNCTION ID args COLON type SEMICOLON		{$$ = NULL;}
		| PROCEDURE ID args SEMICOLON			{$$ = NULL;}
		;
args		:
		| PARENTH_OPEN parList PARENTH_CLOSE
		;
parList		: parList SEMICOLON identListType
		| identListType
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
						assign ->rhs_expr = $3;
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
		| expr			{$$ = $1;}
		;
expr		: simpleExpr relOp simpleExpr	{
                                                    N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                                    expr->typ = OP;
                                                    expr->description.operation.expr = $1;
                                                    switch($2){
                                                        case '=': expr->description.operation.op = EQ_OP; break;
                                                        case '!': expr->description.operation.op = NEQ_OP; break;
                                                        case '<': expr->description.operation.op = LT_OP; break;
                                                        case '>': expr->description.operation.op = GT_OP; break;
                                                        case '{': expr->description.operation.op = LEQ_OP; break;
                                                        case '}': expr->description.operation.op = GEQ_OP; break;
                                                    }
                                                    $1->next = $3;
                                                    expr->next = NULL;
                                                    $$ = expr;
                                                }
		| simpleExpr			{$$ = $1;}
		;
simpleExpr	: term addOp simpleExpr{
                                           N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                           expr->typ = OP;
                                           expr->description.operation.expr = $1;
                                           switch($2){
                                               case '+': expr->description.operation.op = PLUS_OP; break;
                                               case '-': expr->description.operation.op = MINUS_OP; break;
                                               case '|': expr->description.operation.op = OR_OP; break;
                                           }
                                           $1->next = $3;
                                           expr->next = NULL;
                                           $$ = expr;
                                       }
		| term			{$$ = $1;}
		;
term		: factor mulOp term	{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            expr->typ = OP;
                                            expr->description.operation.expr = $1;
                                            switch($2){
                                                case '*':   expr->description.operation.op = MULTI_OP; break;
                                                case '/':   expr->description.operation.op = SLASH_OP; break;
                                                case '\\':  expr->description.operation.op = DIV_OP; break;
                                                case '%':   expr->description.operation.op = MOD_OP; break;
                                                case '&':   expr->description.operation.op = AND_OP; break;
                                            }
                                            $1->next = $3;
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| factor		{$$ = $1;}
		;
factor		: NUM			{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            expr->typ = CONSTANT;
                                            expr->description.constant = $1;
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| FALSE			{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            expr->typ = CONSTANT;
                                            expr->description.constant = "FALSE";
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| TRUE			{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            expr->typ = CONSTANT;
                                            expr->description.constant = "TRUE";
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| ID			{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            N_VAR_REF * var = malloc(sizeof(struct tN_VAR_REF));
                                            var->id = $1;
                                            var->index = NULL;
                                            expr->typ = VAR_REF;
                                            expr->description.var_ref = var;
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
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| NOT factor		{
                                            N_EXPR * expr =  malloc(sizeof(struct tN_EXPR));
                                            expr->typ = OP;
                                            expr->description.operation.op = NOT_OP;
                                            expr->description.operation.expr = $2;
                                            expr->next = NULL;
                                            $$ = expr;
                                        }
		| MINUS factor		{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            N_EXPR * second = malloc(sizeof(struct tN_EXPR));
                                            second->typ = CONSTANT;
                                            second->description.constant = "-1.0";
                                            second->next = NULL;
                                            expr->typ = OP;
                                            expr->description.operation.op = NOT_OP;
                                            expr->description.operation.expr = $2;
                                            expr->description.operation.expr->next = second;
                                            $$ = expr;
                                        }
		| PARENTH_OPEN expr PARENTH_CLOSE {$$ = $2;}
		;
relOp		: LT		{$$ = '<';}
		| GT		{$$ = '>';}
		| EQ		{$$ = '=';}
		| NEQ		{$$ = '!';}
		| GEQ		{$$ = '}';}
		| LEQ		{$$ = '{';}
		;
addOp		: PLUS		{$$ = '+';}
		| MINUS		{$$ = '-';}
		| OR		{$$ = '|';}
		;
mulOp		: MULTI		{$$ = '*';}
		| DIV_SIGN	{$$ = '/';}
		| DIV		{$$ = '\\';}
		| MOD		{$$ = '%';}
		| AND		{$$ = '&';}
		;
%%

int main() { return yyparse(); }
int yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
	printf("error in line %d\n", yylineno);
	return 0; }