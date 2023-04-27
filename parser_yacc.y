%{
#include <stdio.h>
%}

%token  AND ARRAY BEG DIV DO
            ELSE END FUNCTION IF MOD
            NOT OF OR PROCEDURE PROGRAM
            THEN TYPE VAR WHILE INTEGER
            REAL BOOLEAN TRUE FALSE COMMENT
            BRAC_OPEN BRAC_CLOSE PARENTH_OPEN PARENTH_CLOSE COLON
            SEMICOLON COMA DOT RANGE PLUS
            MINUS MULTI DIV_SIGN NEQ EQ
            LT GT LEQ GEQ ASSIGN
            ID NUM
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
simpleType	: INTEGER
		| REAL
		| BOOLEAN
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
compStmt	: BEG stmtList END
		;
stmtList	: stmtList SEMICOLON stmt
		| stmt
		;
stmt		: procCall
		| assignStmt
		| compStmt
		| ifStmt
		| whileStmt
		;
procCall	: ID params
		;
params		: PARENTH_OPEN exprList PARENTH_CLOSE
		| PARENTH_OPEN PARENTH_CLOSE
		;
assignStmt	: ID ASSIGN expr
		| ID index ASSIGN expr
		;
index		: BRAC_OPEN expr BRAC_CLOSE
		| BRAC_OPEN expr RANGE expr BRAC_CLOSE
		;
ifStmt		: IF expr THEN stmt elsePart
		;
elsePart	:
		|  ELSE stmt
		;
whileStmt	: WHILE expr DO stmt
		;
exprList	: exprList COMA expr
		| expr
		;
expr		: simpleExpr relOp simpleExpr
		| simpleExpr
		;
simpleExpr	: simpleExpr addOp term
		| term
		;
term		: term mulOp factor
		| factor
		;
factor		: NUM
		| FALSE
		| TRUE
		| ID
		| ID index
		| ID params
		| NOT factor
		| MINUS factor
		| PARENTH_OPEN expr PARENTH_CLOSE
		;
relOp		: LT
		| GT
		| EQ
		| NEQ
		| GEQ
		| LEQ
		;
addOp		: PLUS
		| MINUS
		| OR
		;
mulOp		: MULTI
		| DIV_SIGN
		| DIV
		| MOD
		| AND
		;
%%

int main() { return yyparse(); }
int yyerror(char *s) { fprintf(stderr, "%s\n", s); return 0; }