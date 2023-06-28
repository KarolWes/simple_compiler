//
// Created by Karol on 23.06.2023.
//

#ifndef SCANNER_L_EXAM_FUNCTIONS_H
#define SCANNER_L_EXAM_FUNCTIONS_H


int t_counter;
int consumed_numbers;
int current_counter;
int stac[1000];
typedef enum {PLUS_OP, MINUS_OP, MULTI_OP, DIV_OP} _OPERATOR;

int prev_prec; // precendece of previous operation:
// () = 0, */ = 1, +- = 2


int simpleExprFun(int left, _OPERATOR op, int right);


#endif //SCANNER_L_EXAM_FUNCTIONS_H
