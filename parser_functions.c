//
// Created by Karol on 25.05.2023.
//

#include "parser_functions.h"

void startFun(char* id, ENTRY* vars, N_PROG *subs, N_STMT *body){
    ast = (N_PROG*) malloc(sizeof (struct tN_PROG));
    ENTRY * entry = (ENTRY*) malloc(sizeof (struct tENTRY));
    entry->typ = _PROG;
    entry->dataType = _MAIN;
    entry->id = id;
    entry->ext.parList = NULL;
    entry->next= vars;
    ast->entry = entry;
    ast->stmt = body;
    ast->next = subs;
}

ENTRY *varDecListFun(ENTRY *first, ENTRY *second) {
    ENTRY * n = first -> next;
    if(n == NULL){
        first->next = second;
    }
    else{
        while(n->next != NULL){
            n = (ENTRY *)(n->next);
        }
        n->next = second;
    }
    return first;
}

ENTRY *intentListTypeFun(ENTRY *list, ENTRY *type) {
    ENTRY *idListType = list;
    ENTRY *head = idListType;
    while(idListType != NULL){
        idListType->typ = type->typ;
        idListType->dataType = type->dataType;
        if(idListType->typ == _ARRAY){
            idListType->ext.bounds.low = type->ext.bounds.low;
            idListType->ext.bounds.high = type->ext.bounds.high;
        }
        idListType = (ENTRY *)(idListType->next);
    }
    free(type);
    return head;
}

ENTRY *simpleTypeFun(_DATA_TYPE type, int array, float lower, float upper) {
    ENTRY *output = (ENTRY *)(malloc(sizeof(struct tENTRY)));
    output->typ = _VAR;
    output->dataType = type;
    if(array == 1){
        int new_lower = (int)(lower*10000);
        if(new_lower%10000 != 0){
            yyerror("Error: array bound must be an int\n");
            exit(11);
        }
        new_lower = (int)(lower);
        int new_upper = (int)(upper*10000);
        if(new_upper%10000 != 0){
            yyerror("Error: array bound must be an int\n");
            exit(11);
        }
        new_upper = (int)(upper);
        if(new_lower > new_upper){
            yyerror("Error: array declaration with second index lower than the first\\n");
            exit(11);
        }
        output->ext.bounds.low = new_lower;
        output->ext.bounds.high = new_upper;
    }
    output->next = NULL;
    return output;
}



ENTRY *idListFun(char *id, ENTRY *second) {
    ENTRY *idList = (ENTRY *)(malloc(sizeof(struct tENTRY)));
    idList->id = id;
    idList->next = second;
    return idList;
}

N_PROG *subProgListFun(ENTRY *header, ENTRY *vars, N_STMT *body, N_PROG *next) {
    N_PROG* prog = (N_PROG *)(malloc(sizeof(struct tN_PROG)));
    prog->entry = header;
    prog->entry->next = vars;
    prog->stmt = body;
    prog->next = next;
    return prog;
}

ENTRY *subHeaderFun(_DATA_TYPE type, char* id, ENTRY* params){
    ENTRY *func = (ENTRY*) malloc(sizeof (struct tENTRY));
    func->typ = _CALL;
    func->dataType = type;
    func->id = id;
    func->ext.parList = params;
    func->next = NULL;
    return func;
}

ENTRY *parListFun(ENTRY *first, ENTRY *tail){
    ENTRY * n = first -> next;
    if(n == NULL){
        first->next = tail;
    }
    else{
        while(n->next != NULL){
            n = (ENTRY *)(n->next);
        }
        n->next = tail;
    }
    return first;
}
N_EXPR *simpleExprFun(N_EXPR* left, _OPERATOR op, N_EXPR *right){
    N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
    expr->typ = OP;
    expr->description.operation.expr = left;
    expr->description.operation.op = op;
    expr->description.operation.expr->next = right;
    expr->parenthesis = 0;
    expr->next = NULL;
    return expr;
}

N_EXPR *booleans(char* val){
    N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
    expr->typ = CONSTANT;
    expr->description.constant = "false";
    expr->parenthesis = 0;
    expr->next = NULL;
    return expr;
}

