%{
#include <stdio.h>
#inlcude <ast.h>

int yyerror(char *s);
extern int yylex();
extern int yylineno;
%}

%union {
	int number;
	float number;
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
%type <number> simpleType relOp addOp mulOp
%type <string> varDecList varDec

%start  start

%%
start		: PROGRAM ID SEMICOLON varDec subProgList compStmt DOT
		;
varDec		:
		| VAR varDecList
		;
varDecList	: varDecList identListType SEMICOLON
		| identListType SEMICOLON
		;
identListType	: identList COLON type
		;
identList	: identList COMA ID
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
		| subProgList subProgHead varDec compStmt SEMICOLON
		;
subProgHead	: FUNCTION ID args COLON type SEMICOLON
		| PROCEDURE ID args SEMICOLON
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
                                                    N_STMT* stmt = (N_STMT*) malloc(sizeof (N_STMT));
                                                    stmt->typ = $1->typ;
                                                    stmt->node = $1->node;
                                                    stmt->next = $3;
                                                    $$ = stmt;
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
index		: BRAC_OPEN expr BRAC_CLOSE		{
							tN_EXPR* e = (tN_EXPR*)malloc(tN_EXPR);


							}
		| BRAC_OPEN expr RANGE expr BRAC_CLOSE
		;
ifStmt		: IF expr THEN stmt elsePart
		;
elsePart	:
		|  ELSE stmt		{
					tN_STMT* e = (tN_STMT *) malloc(sizeof(tN_STMT));

					}
		;
whileStmt	: WHILE expr DO stmt	{
					    tN_WHILE* w = (tN_WHILE *) malloc(sizeof(tN_WHILE));
                                            w->expr = $2;
                                            w->stmt = $4;
                                            $$ = w;
					}
		;
exprList	: expr COMA exprList
		| expr			{$$ = $1;}
		;
expr		: simpleExpr relOp simpleExpr
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
                                            $$ = expr;
                                        }
		| factor		{$$ = $1;}
		;
factor		: NUM			{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::CONSTANT;
                                            expr->description = $1;
                                            $$ = expr;
                                        }
		| FALSE			{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::CONSTANT;
                                            expr->description.constant = "FALSE";
                                            $$ = expr;
                                        }
		| TRUE			{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::CONSTANT;
                                            expr->description.constant = "TRUE";
                                            $$ = expr;
                                        }
		| ID			{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            N_VAR_REF * var = (N_VAR_REF*) malloc(sizeof(N_VAR_REF));
                                            var->id = $1;
                                            var->index = nullptr;
                                            expr->typ = tN_EXPR::VAR_REF;
                                            expr->description.var_ref = var;
                                            $$ = expr;
                                        }
		| ID index		{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            N_VAR_REF * var = (N_VAR_REF*) malloc(sizeof(N_VAR_REF));
                                            var->id = $1;
                                            var->index = $2;
                                            expr->typ = tN_EXPR::VAR_REF;
                                            expr->description.var_ref = var;
                                            $$ = expr;
                                        }
		| ID params		{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            N_CALL * call = (N_CALL*) malloc(sizeof(N_CALL));
                                            call->id = $1;
                                            call->par_list = $2;
                                            expr->typ = tN_EXPR::FUNC_CALL;
                                            expr->description.func_call = call;
                                            $$ = expr;
                                        }
		| NOT factor		{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            expr->typ = tN_EXPR::OP;
                                            expr->description.operation.op = tN_EXPR::uEXPR::sOP::NOT_OP;
                                            expr->description.operation.expr = $2;
                                            $$ = expr;
                                        }
		| MINUS factor		{
                                            N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            N_EXPR * second = (N_EXPR*) malloc(sizeof(N_EXPR));
                                            second->typ = tN_EXPR::CONSTANT;
                                            second->description.constant = "-1.0";
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