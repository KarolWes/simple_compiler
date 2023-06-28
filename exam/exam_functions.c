//
// Created by Karol on 23.06.2023.
//

#include "exam_functions.h"
#include <stdio.h>

int t_counter = 1;
int prev_prec = 10;
int current_counter = 0;
int consumed_numbers = 0;

char decode(_OPERATOR op){
    switch (op) {
        case PLUS_OP:
            return '+';
        case MINUS_OP:
            return '-';
        case MULTI_OP:
            return '*';
        case DIV_OP:
            return '/';
    }
}
int simpleExprFun(int left, _OPERATOR op, int right){
    int val = 0;
    switch (op) {
        case PLUS_OP:
            val = left + right;
            break;
        case MINUS_OP:
            val =  left - right;
            break;
        case MULTI_OP:
            val =  left * right;
            break;
        case DIV_OP:
            val =  (int)(left/right);
            break;
    }
    printf("t%d = ", t_counter);
    if(consumed_numbers > 1 || t_counter == 1){
        printf( "%d %c %d\n", left, decode(op), right);
        prev_prec = consumed_numbers;
        consumed_numbers -= 2;
        stac[current_counter] = t_counter;
        current_counter += 1;
    }
    else if (consumed_numbers == 1){
        current_counter -= 1;
        if(prev_prec > 2){
            printf("%d %c t%d\n", left, decode(op), stac[current_counter]);
        } else{
            printf("t%d %c %d\n", stac[current_counter], decode(op), right);
        }
        consumed_numbers -= 1;
        stac[current_counter] = t_counter;
        current_counter += 1;
    }
    else{
        current_counter -= 1;
        printf("t%d %c t%d\n", stac[current_counter-1], decode(op), stac[current_counter]);
        current_counter-=1;
        stac[current_counter] = t_counter;
        current_counter +=1;
    }
    t_counter += 1;
    return val;
}

