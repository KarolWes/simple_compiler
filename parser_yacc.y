%{
#include <stdio.h>
#include "ast.h"

void yyerror(char *s);
int yylex();
extern int yylineno;
N_PROG *ast;

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
start		: PROGRAM ID SEMICOLON varDec subProgList compStmt DOT	{
										printf("starting -> start\n");
                                                                            ast = (N_PROG*) malloc(sizeof (struct tN_PROG));
                                                                            ENTRY * entry = (ENTRY*) malloc(sizeof (struct tENTRY));
                                                                            entry->typ = _PROG;
                                                                            entry->dataType = _MAIN;
                                                                            entry->base.id = $2;
                                                                            entry->ext.prog.parList = NULL;
                                                                            entry->next= $4;
                                                                            ast->entry = entry;
                                                                            ast->stmt = $6;
                                                                            ast->next = $5;
                                                                        }
		;
varDec		:			{ $$ = NULL; }
		| VAR varDecList	{ $$ = $2; }
		;
varDecList	: identListType SEMICOLON varDecList	{
                                                            ENTRY * n = $1 -> next;
                                                            if(n == NULL){
                                                                $1->next = $3;
                                                            }
                                                            else{
                                                                while(n->next != NULL){
                                                                    n = (ENTRY *)(n->next);
                                                                }
                                                                n->next = $3;
                                                            }
                                                            $$ = $1;
                                                        }
		| identListType SEMICOLON		{ $$ = $1; }
		;
identListType	: identList COLON type {
                                           ENTRY *idListType = $1;
                                           ENTRY *returnVal = idListType;
                                           while(idListType != NULL){
                                               idListType->typ = $3->typ;
                                               idListType->dataType = $3->dataType;
                                               if(idListType->typ == _ARRAY){
                                                   idListType->ext.bounds.low = $3->ext.bounds.low;
                                                   idListType->ext.bounds.high = $3->ext.bounds.high;
                                               }
                                               idListType = (ENTRY *)(idListType->next);
                                           }
                                           $$ = returnVal;
                                       }
		;
identList	: ID COMA identList	{
					ENTRY *idList = (ENTRY *)(malloc(sizeof(struct tENTRY)));
					idList->base.id = $1;
					idList->next = $3;
					$$ = idList;
					}
		| ID			{
					ENTRY *idList = (ENTRY *)(malloc(sizeof(struct tENTRY)));
					idList->base.id = $1;
					idList->next = NULL;
					$$ = idList;
					}
		;
type		: simpleType							{
                                                                                    ENTRY *type = (ENTRY *)(malloc(sizeof(struct tENTRY)));
                                                                                    type->typ = _VAR;

                                                                                    type->dataType = $1;
                                                                                    type->next = NULL;
                                                                                    $$ = type;
                                                                                }
		| ARRAY BRAC_OPEN NUM RANGE NUM BRAC_CLOSE OF simpleType	{
                                                                                    ENTRY *type = (ENTRY *)(malloc(sizeof(struct tENTRY)));
                                                                                    type->typ = _ARRAY;
                                                                                    type->dataType = $8;
                                                                                    type->ext.bounds.low = atoi($3);
                                                                                    type->ext.bounds.high = atoi($5);
                                                                                    type->next = NULL;
                                                                                    $$ = type;
                                                                                }
		;
simpleType	: INTEGER	{$$ = _INT;}
		| REAL		{$$ = _REAL;}
		| BOOLEAN	{$$ = _BOOL;}
		;
subProgList	:	{$$ = NULL; }
		|  subProgHead varDec compStmt SEMICOLON subProgList {
                                                                         N_PROG* prog = (N_PROG *)(malloc(sizeof(struct tN_PROG)));
                                                                         prog->entry = $1;
                                                                         printf("subprog type -> %d\n)", prog->entry->typ);
                                                                         prog->entry->next = $2;
                                                                         prog->stmt = $3;
                                                                         prog->next = $5;
                                                                         $$ = prog;
                                                                     }
		;
subProgHead	: FUNCTION ID args COLON simpleType SEMICOLON		{
                                                                            ENTRY * func = (ENTRY*) malloc(sizeof (struct tENTRY));
                                                                            func->typ = _CALL;
                                                                            func->dataType = $5;
                                                                            func->base.id = $2;
                                                                            func->ext.prog.parList = $3;
                                                                            func->next = NULL;
                                                                            $$ = func;
                                                                        }
		| PROCEDURE ID args SEMICOLON			{
                                                                    ENTRY * func = (ENTRY*) malloc(sizeof (struct tENTRY));
                                                                    func->typ = _CALL;
                                                                    func->dataType = _VOID;
                                                                    func->base.id = $2;
                                                                    func->ext.prog.parList = $3;
                                                                    func->next = NULL;
                                                                    $$ = func;
                                                                }
		;
args		:	{ $$ = NULL; };
		| PARENTH_OPEN parList PARENTH_CLOSE	{ $$ = $2; }
		;
parList		: identListType SEMICOLON parList	{
							    ENTRY * n = $1 -> next;
							    if(n == NULL){
								$1->next = $3;
							    }
							    else{
								while(n->next != NULL){
								    n = (ENTRY *)(n->next);
								}
								n->next = $3;
							    }
							    $$ = $1;
							}
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
                                                    expr->next = $3;
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
                                           expr->next = $3;
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
                                            expr->next = $3;
                                            $$ = expr;
                                        }
		| factor		{
					$$ = $1;
					}
		;
factor		: NUM			{
                                            N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
                                            expr->typ = CONSTANT;
                                            float num = $1;
					    int len = snprintf(NULL, 0, "%f", num);
					    char* res = (char *)(malloc(len + 1));
					    snprintf(res, len+1, "%f", num);
                                            expr->description.constant = res;
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

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
	printf("error in line %d\n", yylineno);
	}