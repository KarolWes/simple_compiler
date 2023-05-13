//
// Created by Karol on 11.05.2023.
//

#ifndef SCANNER_L_PRINTOUT_H
#define SCANNER_L_PRINTOUT_H
#include "ast.h"
#include "parser_yacc.tab.h"
#include <stdio.h>

char * typeToStr(_DATA_TYPE type);
char *operatorToStr(_OPERATOR o);
void printVarRef(N_VAR_REF *input);
void printIf(N_IF *input);
void printWhile(N_WHILE *input);
void printCall(N_CALL *input, char* separator);
void printAssign(N_ASSIGN *input);
void printStatement(N_STMT* input, int ignore_marker);
void printExpr(N_EXPR *input, char* separator);
void printEntry(ENTRY *input);
void printProgramBase(N_PROG *input);
void printProgram(N_PROG *input);
void printArgs(ENTRY *input);
void run();
#endif //SCANNER_L_PRINTOUT_H
