//
// Created by Karol on 25.05.2023.
//

#ifndef SCANNER_L_PARSER_FUNCTIONS_H
#define SCANNER_L_PARSER_FUNCTIONS_H

#include "ast.h"
#include <stdlib.h>

extern void yyerror(char *s);

void startFun(char* id, ENTRY* vars, N_PROG *subs, N_STMT *body);
ENTRY * varDecListFun(ENTRY* first, ENTRY* second);
ENTRY * intentListTypeFun(ENTRY *list, ENTRY *type);
ENTRY * simpleTypeFun(_DATA_TYPE type, int array, float lower, float upper);
ENTRY *idListFun(char* id, ENTRY* second);
N_PROG *subProgListFun(ENTRY *header, ENTRY *vars, N_STMT *body, N_PROG *next);
ENTRY *subHeaderFun(_DATA_TYPE type, char* id, ENTRY* params);
ENTRY *parListFun(ENTRY *first, ENTRY *tail);
N_EXPR *simpleExprFun(N_EXPR* left, _OPERATOR op, N_EXPR *right);
N_EXPR *booleans(char* val);
N_EXPR *numConversion(float num);
N_EXPR *identifiers(char* name, N_EXPR *extension, int type);
N_ASSIGN *assingmentFun(char* name, N_EXPR *index, N_EXPR *rhs);

#endif //SCANNER_L_PARSER_FUNCTIONS_H
