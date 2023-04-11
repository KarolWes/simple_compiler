//
// Created by Admin on 07.04.2023.
//

#ifndef SIMPLE_COMPILER_PARSER_H
#define SIMPLE_COMPILER_PARSER_H

enum TokenType{
    AND=1, ARRAY, BEG, DIV, DO,
    ELSE, END, FUNCTION, IF, MOD,
    NOT, OF, OR, PROCEDURE, PROGRAM,
    THEN, TYPE, VAR, WHILE, INTEGER,
    REAL, BOOLEAN, TRUE, FALSE, COMMENT,
    BRAC_OPEN, BRAC_CLOSE, PARENTH_OPEN, PARENTH_CLOSE, COLON,
    SEMICOLON, COMA, DOT, RANGE, PLUS,
    MINUS, MULTI, DIV_SIGN, NEQ, EQ,
    LT, GT, LEQ, GEQ, ASSIGN,
    ID=101, NUM=100};

int start();
int varDec();
int varDecList();
int identListType();
int identList();
int type();
int simpleType();
int subProgList();
int subProgHead();
int args();
int parList();
int compStmt();
int stmtList();
int statement();
int procCall();
int params();
int assignStmt();
int ind();
int ifStmt();
int whileStmt();
int exprList();
int expr();
int simpleExpr();
int term();
int factor();
int relOp();
int addOp();
int mulOp();

int match(enum TokenType expected);

#endif //SIMPLE_COMPILER_PARSER_H
