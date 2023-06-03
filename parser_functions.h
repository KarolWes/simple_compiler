//
// Created by Karol on 25.05.2023.
//

#ifndef SCANNER_L_PARSER_FUNCTIONS_H
#define SCANNER_L_PARSER_FUNCTIONS_H

#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int yylineno;

ENTRY *globalVars;
ENTRY *localVars;
ENTRY *res;
ENTRY *funcs;
char* inFun;
_DATA_TYPE returnType;

void startFun(char* id, ENTRY* vars);
ENTRY * varDecListFun(ENTRY* first, ENTRY* second);
ENTRY * intentListTypeFun(ENTRY *list, ENTRY *type);
ENTRY * simpleTypeFun(_DATA_TYPE type, int array, float lower, float upper);
ENTRY *idListFun(char* id, ENTRY* second);
N_PROG *subProgListFun(ENTRY *header, ENTRY *vars);
ENTRY *subHeaderFun(_DATA_TYPE type, char* id, ENTRY* params);
ENTRY *parListFun(ENTRY *first, ENTRY *tail);
N_EXPR *simpleExprFun(N_EXPR* left, _OPERATOR op, N_EXPR *right);
N_EXPR *booleans(char* val);
N_EXPR *numConversion(float num);
N_EXPR *identifiers(char* name, N_EXPR *extension, int type);
N_ASSIGN *assignmentFun(char* name, N_EXPR *index, N_EXPR *rhs);
N_CALL *procCallFun(char* name, N_EXPR *params);

ENTRY* variableLookup(char* name);
ENTRY *funLookup(char* name);
void findDuplicates(ENTRY *scope);
void printScope();

void checkType(_DATA_TYPE lhs, int array, N_EXPR *rhs);
void checkIndex(N_EXPR *index, ENTRY *def);
void checkParameters(ENTRY* fun, N_EXPR* pars);

ENTRY* extractParams(ENTRY* params);
ENTRY *constructMain();
ENTRY* append(ENTRY* node, ENTRY* goal);

#endif //SCANNER_L_PARSER_FUNCTIONS_H
