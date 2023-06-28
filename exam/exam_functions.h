//
// Created by Karol on 23.06.2023.
//

#ifndef SCANNER_L_EXAM_FUNCTIONS_H
#define SCANNER_L_EXAM_FUNCTIONS_H


#include "micro_ast.h"

N_EXPR* simpleExprFun(N_EXPR* left, _OPERATOR op, N_EXPR* right);
N_EXPR* factorFun(int val);


#endif //SCANNER_L_EXAM_FUNCTIONS_H
