#include <cstdio>
#include <stdlib.h>
#include "ast.h"


int main()
{
    N_EXPR * expr = (N_EXPR*) malloc(sizeof(N_EXPR));
    expr->typ = tN_EXPR::OP;
    expr->description.operation.expr = $1;
    switch($2){
        case '+': expr->description.operation.op = tN_EXPR::uEXPR::sOP::PLUS_OP; break;
        case '-': expr->description.operation.op = tN_EXPR::uEXPR::sOP::MINUS_OP; break;
        case '|': expr->description.operation.op = tN_EXPR::uEXPR::sOP::OR_OP; break;
    }
    $1->next = $3;
    $$ = expr;
}
