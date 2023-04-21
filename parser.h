//
// Created by Admin on 07.04.2023.
//

#ifndef SIMPLE_COMPILER_PARSER_H
#define SIMPLE_COMPILER_PARSER_H
#include "enumTypes.h"


extern int yylineno;
enum TokenType token;

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
int procOrAssign();
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

extern int yylex();

int match(enum TokenType expected);

#endif //SIMPLE_COMPILER_PARSER_H
