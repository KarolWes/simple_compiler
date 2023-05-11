//
// Created by Karol on 11.05.2023.
//

#ifndef SCANNER_L_PRINTOUT_H
#define SCANNER_L_PRINTOUT_H
#include "ast.h"
#include <stdio.h>

char * typeToStr(_DATA_TYPE type);
char *operatorToStr(_OPERATOR o);
void printVarRef(N_VAR_REF *input);
void printIf(N_IF *input); // undeclared
void printWhile(N_WHILE *input); // undeclared
void printCall(N_CALL *input); // undeclared
void printAssign(N_ASSIGN *input); // undeclared
void printStatement(N_STMT* input);
void printExpr(N_EXPR *input);
#endif //SCANNER_L_PRINTOUT_H
