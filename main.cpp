#include <cstdio>
#include "ffsets.h"

int main(){
    TokenType t = SEMICOLON;
    printf("%d", in(t, CompStmt, 1,0,0));
    printf("%d", in(t, CompStmt, 1,1,0));
}