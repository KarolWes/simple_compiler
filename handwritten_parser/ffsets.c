//
// Created by Karol on 21.04.2023.
//

#include "ffsets.h"
#include "enumTypes.h"

int first[][7] = {
        {PROGRAM},
        {VAR, EPSILON},
        {ID},
        {ID},
        {ID},
        {INTEGER, REAL, BOOLEAN, ARRAY},
        {INTEGER, REAL, BOOLEAN},
        {FUNCTION, PROCEDURE, EPSILON},
        {FUNCTION, PROCEDURE},
        {PARENTH_OPEN, EPSILON},
        {ID},
        {BEG},
        {ID, IF, WHILE, BEG},
        {ID, IF, WHILE, BEG},
        {PARENTH_OPEN, ASSIGN, BRAC_OPEN},
        {PARENTH_OPEN},
        {ASSIGN, BRAC_OPEN},
        {BRAC_OPEN},
        {IF},
        {WHILE},
        {NUM, FALSE, TRUE, ID, NOT, MINUS, PARENTH_OPEN},
        {NUM, FALSE, TRUE, ID, NOT, MINUS, PARENTH_OPEN},
        {NUM, FALSE, TRUE, ID, NOT, MINUS, PARENTH_OPEN},
        {NUM, FALSE, TRUE, ID, NOT, MINUS, PARENTH_OPEN},
        {NUM, FALSE, TRUE, ID, NOT, MINUS, PARENTH_OPEN},
        {LT, LEQ, GT, GEQ, NEQ, EQ},
        {MULTI, DIV_SIGN, DIV, MOD, AND},
        {PLUS, MINUS, OR},
};

int first_size[] = {1, 2, 1, 1, 1, 4, 3, 3, 2, 2, 1,
                    1, 4, 4, 3, 1, 2, 1, 1, 1, 6,
                    7, 7, 7,7, 6, 5, 3};


int follow[][30] = {
        {DOLLAR},
        {FUNCTION, PROCEDURE, BEG},
        {FUNCTION, PROCEDURE, BEG},
        {SEMICOLON, PARENTH_CLOSE},
        {COLON},
        {SEMICOLON, PARENTH_CLOSE},
        {SEMICOLON, PARENTH_CLOSE},
        {BEG},
        {VAR, BEG},
        {COLON, SEMICOLON},
        {PARENTH_CLOSE},
        {DOT, SEMICOLON, END, ELSE},
        {END},
        {SEMICOLON, END, ELSE},
        {SEMICOLON, END, ELSE},
        {BRAC_CLOSE, RANGE,  THEN, DO, COMA, PARENTH_CLOSE, SEMICOLON, END, ELSE, LT, LEQ, GT, GEQ, EQ, NEQ, PLUS, MINUS, OR, MULTI, DIV_SIGN, DIV, MOD, AND},
        {SEMICOLON, ELSE, END},
        {ASSIGN, BRAC_CLOSE, RANGE,  THEN, DO, COMA, PARENTH_CLOSE, SEMICOLON, END, ELSE, LT, LEQ, GT, GEQ, EQ, NEQ, PLUS, MINUS, OR, MULTI, DIV_SIGN, DIV, MOD, AND},
        {SEMICOLON, ELSE, END},
        {SEMICOLON, END, ELSE},
        {PARENTH_CLOSE},
        {BRAC_CLOSE, RANGE,  THEN, DO, COMA, PARENTH_CLOSE, SEMICOLON, END, ELSE},
        {BRAC_CLOSE, RANGE,  THEN, DO, COMA, PARENTH_CLOSE, SEMICOLON, END, ELSE, LT, LEQ, GT, GEQ, EQ, NEQ, PLUS, MINUS, OR, MULTI, DIV_SIGN, DIV, MOD, AND},
        {BRAC_CLOSE, RANGE,  THEN, DO, COMA, PARENTH_CLOSE, SEMICOLON, END, ELSE, LT, LEQ, GT, GEQ, EQ, NEQ, PLUS, MINUS, OR, MULTI, DIV_SIGN, DIV, MOD, AND},
        {BRAC_CLOSE, RANGE,  THEN, DO, COMA, PARENTH_CLOSE, SEMICOLON, END, ELSE, LT, LEQ, GT, GEQ, EQ, NEQ, PLUS, MINUS, OR, MULTI, DIV_SIGN, DIV, MOD, AND},
        {NUM, FALSE, TRUE, ID, NOT, MINUS, PARENTH_OPEN},
        {NUM, FALSE, TRUE, ID, NOT, MINUS, PARENTH_OPEN},
        {NUM, FALSE, TRUE, ID, NOT, MINUS, PARENTH_OPEN}
};

int follow_size[] ={1, 3,3,2,1,2,2,1,2,2,
                    1,4,1,3,3,23, 3,24, 3,3,
                    1,9, 23, 23, 23, 7,7,7};


int in_set(enum TokenType t, enum Nonterminal n, int include_first, int include_follow, int include_eof) {
    int res = 0;
    if(include_eof == 1){
        if (t == DOLLAR){
            res = 1;
        }
    }
    if(include_first == 1){
        for(int i = 0; i < first_size[n]; i++){
            if(t == first[n][i]){
                res = 1;
            }
        }
    }
    if(include_follow == 1){
        for(int i = 0; i < follow_size[n]; i++){
            if(t == follow[n][i]){
                res = 1;
            }
        }
    }
    return res;
}
