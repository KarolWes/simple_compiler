//
// Created by Karol on 23.06.2023.
//

#include "exam_functions.h"
#include <stdlib.h>
#include <stdio.h>
N_EXPR* simpleExprFun(N_EXPR* left, _OPERATOR op, N_EXPR* right){
    N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
    printf("in simple expr\n");
    expr->type=EXP;


    switch (op) {
        case PLUS_OP:
            expr->value = left->value + right->value;
        case MINUS_OP:
            expr->value = left->value - right->value;
        case MULTI_OP:
            expr->value = left->value * right->value;
        case DIV_OP:
            expr->value = left->value / right->value;
    }
    expr->operation.expr = left;
    expr->operation.op = op;
    expr->operation.expr->next = right;
    expr->next = NULL;
    return expr;
}

N_EXPR* factorFun(int val){
    N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
    expr->type = NUMBER;
    expr->value = val;
    expr->next = NULL;
    return expr;
}