#include <cstdio>
#include <stdlib.h>
#include "ast.h"

// identListType SEMICOLON parList
int main() {
    ast = (N_PROG*) malloc(sizeof (struct tN_PROG));
    ENTRY * entry = (ENTRY*) malloc(sizeof (struct tENTRY));
    entry->typ = _PROG;
    entry->dataType = _VOID;
    entry->base.id = $2;
    entry->next= $4;
    ast->stmt = $6;
    ast->next = $5;
}
