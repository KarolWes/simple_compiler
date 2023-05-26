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

#endif //SCANNER_L_PARSER_FUNCTIONS_H
