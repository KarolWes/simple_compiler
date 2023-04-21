//
// Created by Karol on 21.04.2023.
//

#ifndef SIMPLE_COMPILER_ENUMTYPES_H
#define SIMPLE_COMPILER_ENUMTYPES_H

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
    ID=101, NUM=100,
    EPSILON=201, DOLLAR=0};

const char* translateTT(enum TokenType t);

enum Nonterminal{
    Start=0, VarDec, VarDecList, IdentListType, IdentList,
    Type, SimpleType, SubProgList, SubProgHead, Args,
    ParList, CompStmt, StmtList, Stmt, ProcOrAssign,
    Params, AssignStmt, Index, IfStmt, WhileStmt,
    ExprList, Expr, SimpleExpr, Term, Factor,
    Relop, Mulop, Addop
};

const char* translateNT(enum Nonterminal nt);

int const NONTERMINAL_LEN;

#endif //SIMPLE_COMPILER_ENUMTYPES_H
