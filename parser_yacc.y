%{
#include <stdio.h>
#include <stdlib.h>
#inlcude <ast.h>

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

%type <entry> identListType identList type subProgHead
%type <prog> subProgList
%type <stmt> compStmt stmtList stmt elsePart
%type <call_stmt> procCall
%type <assign_stmt> assignStmt
%type <if_stmt> ifStmt
%type <while_stmt> whileStmt
%type <expr> params exprList expr index simpleExpr term factor
%type <number_i> simpleType relOp addOp mulOp
%type <string> varDecList varDec

%start  start

%%
start		: PROGRAM ID SEMICOLON varDec subProgList compStmt DOT	{
                                                                            ast = (N_PROG*) malloc(sizeof (N_PROG));
                                                                            ast->stmt = $6;
                                                                            ast->next = nullptr;
                                                                        }
		;
varDec		:			{$$ = nullptr;}
		| VAR varDecList	{$$ = $2;}
		;
varDecList	: identListType SEMICOLON varDecList	{
                                                        	$1->next = $3;
                                                        	$$ = $1;
                                                      	}
		| identListType SEMICOLON		{$$=$1;}
		;
identListType	: identList COLON type
		;
identList	: identList COMA ID
		| ID			{
                                            N_VAR_REF* var = (N_VAR_REF*) malloc(sizeof( N_VAR_REF));
                                            var->id = $1;
                                            var->index = nullptr;
                                            $$ = var;
                                        }
		;
type		: simpleType	{$$ = nullptr;}
		| ARRAY BRAC_OPEN NUM RANGE NUM BRAC_CLOSE OF simpleType	{
                                                                                    N_EXPR* r_left = (N_EXPR*) malloc(sizeof (N_EXPR));
                                                                                    r_left->typ = tN_EXPR::CONSTANT;
                                                                                    r_left->description = $3;
                                                                                    N_EXPR* r_right = (N_EXPR*) malloc(sizeof (N_EXPR));
                                                                                    r_right->typ = tN_EXPR::CONSTANT;
                                                                                    r_right->description = $5;
                                                                                    r_right->next = nullptr;
                                                                                    r_left->next = r_right;
                                                                                    $$ = r_left;
                                                                                }
		;
simpleType	: INTEGER	{$$ = 'i';}
		| REAL		{$$ = 'f';}
		| BOOLEAN	{$$ = 'b';}
		;
subProgList	:
		|  subProgHead varDec compStmt SEMICOLON subProgList {
                                                                         N_PROG* prog = (N_PROG*) malloc(sizeof (N_PROG));
                                                                         prog->stmt = $3;
                                                                         prog->next = $5;
                                                                         $$ = prog;
                                                                     }
		;
subProgHead	: FUNCTION ID args COLON type SEMICOLON		{$$ = nullptr;}
		| PROCEDURE ID args SEMICOLON			{$$ = nullptr;}
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
                                                    $1->next = $3;
                                                    $$ = $1;
                                                }
		| stmt				{$$ = $1;}
		;
stmt		: procCall	{
                                    N_STMT* stmt = (N_STMT*) malloc(sizeof (N_STMT));
                                    stmt->typ = tN_STMT::_PROC_CALL;
                                    stmt->node = $1;
                                    $$ = stmt;
                                }
		| assignStmt	{
                                    N_STMT* stmt = (N_STMT*) malloc(sizeof (N_STMT));
                                    stmt->typ = tN_STMT::_ASSIGN;
                                    stmt->node = $1;
                                    $$ = stmt;
                                }
		| compStmt 	{ $$ = $1; }
		| ifStmt	{
                                    N_STMT* stmt = (N_STMT*) malloc(sizeof (N_STMT));
                                    stmt->typ = tN_STMT::_IF;
                                    stmt->node = $1;
                                    $$ = stmt;
                                }
		| whileStmt	{
                                    N_STMT* stmt = (N_STMT*) malloc(sizeof (N_STMT));
                                    stmt->typ = tN_STMT::_WHILE;
                                    stmt->node = $1;
                                    $$ = stmt;
                                }
		;
procCall	: ID params	{
                                    N_CALL* call = (N_CALL*) malloc(sizeof (N_CALL));
                                    call->id = $1;
                                    call->par_list = $2;
                                    $$ = call;
                                }
		;
params		: PARENTH_OPEN exprList PARENTH_CLOSE 	{$$ = $2;}
		| PARENTH_OPEN PARENTH_CLOSE		{$$ = nullptr;}
		;
assignStmt	: ID ASSIGN expr		{
						N_VAR_REF* var = (N_VAR_REF*) malloc(sizeof(N_VAR_REF));
						var->id = $1;
						var->index = nullptr;
						N_ASSIGN* assign = (N_ASSIGN*) malloc(sizeof (N_ASSIGN));
						assign->var_ref = var;
						assign ->rhs_expr = $3;
						$$ = assign;
						}
		| ID index ASSIGN expr		{
                                                    N_VAR_REF* var = (N_VAR_REF*) malloc(sizeof(N_VAR_REF));
                                                    var->id = $1;
                                                    var->index = $2;
                                                    N_ASSIGN* assign = (N_ASSIGN*) malloc(sizeof (N_ASSIGN));
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
                                                    N_IF *i = (N_IF*) malloc(sizeof (N_IF));
                                                    i->expr = $2;
                                                    i->then_part = $4;
                                                    i->else_part = $5;
                                                    $$ = i;
                                                }
		;
elsePart	:			{$$ = nullptr;}
		|  ELSE stmt		{$$ = $2;}
		;
whileStmt	: WHILE expr DO stmt	{
					    tN_WHILE* w = (tN_WHILE *) malloc(sizeof(tN_WHILE));
                                            w->expr = $2;
                                            w->stmt = $4;
                                            $$ = w;
					}
		;
exprList	: expr COMA exprList	{
                                            $1->next = $3;
                                            $$ = $1
                                        }
		| expr			{$$ = $1;}
		;
expr		: simpleExpr relOp simpleExpr	{
                                                    N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                                    expr->typ = tN_EXPR::OP;
                                                    expr->description.operation.expr = $1;
                                                    switch($2){
                                                        case '=': expr->description.operation.op = tN_EXPR::uEXPR::sOP::EQ_OP; break;
                                                        case '!': expr->description.operation.op = tN_EXPR::uEXPR::sOP::NEQ_OP; break;
                                                        case '<': expr->description.operation.op = tN_EXPR::uEXPR::sOP::LT_OP; break;
                                                        case '>': expr->description.operation.op = tN_EXPR::uEXPR::sOP::GT_OP; break;
                                                        case '{': expr->description.operation.op = tN_EXPR::uEXPR::sOP::LEQ_OP; break;
                                                        case '}': expr->description.operation.op = tN_EXPR::uEXPR::sOP::GEQ_OP; break;
                                                    }
                                                    $1->next = $3;
                                                    expr->next = nullptr;
                                                    $$ = expr;
                                                }
		| simpleExpr			{$$ = $1;}
		;
simpleExpr	: term addOp simpleExpr{
                                           N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                           expr->typ = tN_EXPR::OP;
                                           expr->description.operation.expr = $1;
                                           switch($2){
                                               case '+': expr->description.operation.op = tN_EXPR::uEXPR::sOP::PLUS_OP; break;
                                               case '-': expr->description.operation.op = tN_EXPR::uEXPR::sOP::MINUS_OP; break;
                                               case '|': expr->description.operation.op = tN_EXPR::uEXPR::sOP::OR_OP; break;
                                           }
                                           $1->next = $3;
                                           expr->next = nullptr;
                                           $$ = expr;
                                       }
		| term			{$$ = $1;}
		;
term		: factor mulOp term	{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::OP;
                                            expr->description.operation.expr = $1;
                                            switch($2){
                                                case '*':   expr->description.operation.op = tN_EXPR::uEXPR::sOP::MULTI_OP; break;
                                                case '/':   expr->description.operation.op = tN_EXPR::uEXPR::sOP::SLASH_OP; break;
                                                case '\\':  expr->description.operation.op = tN_EXPR::uEXPR::sOP::DIV_OP; break;
                                                case '%':   expr->description.operation.op = tN_EXPR::uEXPR::sOP::MOD_OP; break;
                                                case '&':   expr->description.operation.op = tN_EXPR::uEXPR::sOP::AND_OP; break;
                                            }
                                            $1->next = $3;
                                            expr->next = nullptr;
                                            $$ = expr;
                                        }
		| factor		{$$ = $1;}
		;
factor		: NUM			{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::CONSTANT;
                                            expr->description = $1;
                                            expr->next = nullptr;
                                            $$ = expr;
                                        }
		| FALSE			{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::CONSTANT;
                                            expr->description.constant = "FALSE";
                                            expr->next = nullptr;
                                            $$ = expr;
                                        }
		| TRUE			{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::CONSTANT;
                                            expr->description.constant = "TRUE";
                                            expr->next = nullptr;
                                            $$ = expr;
                                        }
		| ID			{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            N_VAR_REF * var = (N_VAR_REF*) malloc(sizeof(N_VAR_REF));
                                            var->id = $1;
                                            var->index = nullptr;
                                            expr->typ = tN_EXPR::VAR_REF;
                                            expr->description.var_ref = var;
                                            expr->next = nullptr;
                                            $$ = expr;
                                        }
		| ID index		{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            N_VAR_REF * var = (N_VAR_REF*) malloc(sizeof(N_VAR_REF));
                                            var->id = $1;
                                            var->index = $2;
                                            expr->typ = tN_EXPR::VAR_REF;
                                            expr->description.var_ref = var;
                                            expr->next = nullptr;
                                            $$ = expr;
                                        }
		| ID params		{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            N_CALL * call = (N_CALL*) malloc(sizeof(N_CALL));
                                            call->id = $1;
                                            call->par_list = $2;
                                            expr->typ = tN_EXPR::FUNC_CALL;
                                            expr->description.func_call = call;
                                            expr->next = nullptr;
                                            $$ = expr;
                                        }
		| NOT factor		{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::OP;
                                            expr->description.operation.op = tN_EXPR::uEXPR::sOP::NOT_OP;
                                            expr->description.operation.expr = $2;
                                            expr->next = nullptr;
                                            $$ = expr;
                                        }
		| MINUS factor		{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            N_EXPR * second = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            second->typ = tN_EXPR::CONSTANT;
                                            second->description.constant = "-1.0";
                                            second->next = nullptr;
                                            expr->typ = tN_EXPR::OP;
                                            expr->description.operation.op = tN_EXPR::uEXPR::sOP::NOT_OP;
                                            expr->description.operation.expr = $2;
                                            expr->description.operation.expr->next = second;
                                            $$ = expr;
                                        }
		| PARENTH_OPEN expr PARENTH_CLOSE {$$ = $2}
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