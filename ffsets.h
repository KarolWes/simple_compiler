//
// Created by Karol on 21.04.2023.
//

#ifndef SIMPLE_COMPILER_FFSETS_H
#define SIMPLE_COMPILER_FFSETS_H
#include "enumTypes.h"

int first[][7];
int first_size[];
int follow[][30];
int follow_size[];

int in_set(enum TokenType t, enum Nonterminal n, int include_first, int include_follow, int include_eof);



#endif //SIMPLE_COMPILER_FFSETS_H
