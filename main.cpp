#include <cstdio>
#include <stdlib.h>
#include "ast.h"

// PROGRAM ID SEMICOLON varDec subProgList compStmt DOT
int main() {
            N_STMT * stmt = malloc(sizeof(struct tN_STMT));
            stmt->typ = _PROC_CALL;
            stmt->node.proc_call = $1;
            $$ = stmt;
            N_STMT * stmt = malloc(sizeof(struct tN_STMT));
            stmt->typ = _ASSIGN;
            stmt->node.assign_stmt = $1;
            $$ = stmt;
            N_STMT * stmt = malloc(sizeof(struct tN_STMT));
            stmt->typ = _IF;
            stmt->node.if_stmt = $1;
            $$ = stmt;
            N_STMT * stmt = malloc(sizeof(struct tN_STMT));
            stmt->typ = _WHILE;
            stmt->node.while_stmt = $1;
            $$ = stmt;
    };
}
