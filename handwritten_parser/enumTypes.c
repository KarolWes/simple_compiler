//
// Created by Karol on 21.04.2023.
//
#include "enumTypes.h"

const char* translateTT(enum TokenType t) {
    switch(t) {
        case AND: return "AND";
        case ARRAY: return "ARRAY";
        case BEG: return "BEGIN";
        case DIV: return "DIV";
        case DO: return "DO";
        case ELSE: return "ELSE";
        case END: return "END";
        case FUNCTION: return "FUNCTION";
        case IF: return "IF";
        case MOD: return "MOD";
        case NOT: return "NOT";
        case OF: return "OF";
        case OR: return "OR";
        case PROCEDURE: return "PROCEDURE";
        case PROGRAM: return "PROGRAM";
        case THEN: return "THEN";
        case TYPE: return "TYPE";
        case VAR: return "VAR";
        case WHILE: return "WHILE";
        case INTEGER: return "INTEGER";
        case REAL: return "REAL";
        case BOOLEAN: return "BOOLEAN";
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case BRAC_OPEN: return "[";
        case BRAC_CLOSE: return "]";
        case PARENTH_OPEN: return "(";
        case PARENTH_CLOSE: return ")";
        case COLON: return ":";
        case SEMICOLON: return ";";
        case COMA: return ",";
        case DOT: return ".";
        case RANGE: return "..";
        case PLUS: return "+";
        case MINUS: return "-";
        case MULTI: return "*";
        case DIV_SIGN: return "/";
        case NEQ: return "<>";
        case EQ: return "=";
        case LT: return "<";
        case GT: return ">";
        case LEQ: return "<=";
        case GEQ: return ">=";
        case ASSIGN: return ":=";
        case ID: return "IDENTIFIER";
        case NUM: return "NUMBER";
        case EPSILON: return "EMPTY CHARACTER";
        case DOLLAR: return "END OF FILE";
        default: return "UNKNOWN";
    }
}

const char* translateNT(enum Nonterminal nt) {
    switch(nt) {
        case Start: return "Start";
        case VarDec: return "VarDec";
        case VarDecList: return "VarDecList";
        case IdentListType: return "IdentListType";
        case IdentList: return "IdentList";
        case Type: return "Type";
        case SimpleType: return "SimpleType";
        case SubProgList: return "SubProgList";
        case SubProgHead: return "SubProgHead";
        case Args: return "Args";
        case ParList: return "ParList";
        case CompStmt: return "CompStmt";
        case StmtList: return "StmtList";
        case Stmt: return "Stmt";
        case ProcOrAssign: return "ProcOrAssign";
        case Params: return "Params";
        case AssignStmt: return "AssignStmt";
        case Index: return "Index";
        case IfStmt: return "IfStmt";
        case WhileStmt: return "WhileStmt";
        case ExprList: return "ExprList";
        case Expr: return "Expr";
        case SimpleExpr: return "SimpleExpr";
        case Term: return "Term";
        case Factor: return "Factor";
        case Relop: return "Relop";
        case Mulop: return "Mulop";
        case Addop: return "Addop";
        default: return "UNKNOWN";
    }
}

int const NONTERMINAL_LEN=27;