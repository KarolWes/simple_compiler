//
// Created by Karol on 25.05.2023.
//

#include <stdio.h>
#include <string.h>
#include "parser_functions.h"

void startFun(char* id, ENTRY* vars){
    funcs = constructMain();
    res = (ENTRY*)malloc(sizeof(struct tENTRY));
    res->typ = _VAR;
    res->id = "result";
    res->next = NULL;
    globalVars = vars;
    findDuplicates(globalVars);
    ast = (N_PROG*) malloc(sizeof (struct tN_PROG));
    ENTRY * entry = (ENTRY*) malloc(sizeof (struct tENTRY));
    entry->typ = _PROG;
    entry->dataType = _MAIN;
    entry->id = id;
    entry->ext.parList = NULL;
    entry->next= vars;
    ast->entry = entry;
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
        output->typ = _ARRAY;
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
            yyerror("Error: array declaration with second index lower than the first\n");
            exit(11);
        }
        printf("\t%d\t%d\n", new_lower, new_upper);
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

N_PROG *subProgListFun(ENTRY *header, ENTRY *vars) {
    N_PROG* prog = (N_PROG *)(malloc(sizeof(struct tN_PROG)));
    prog->entry = header;
    prog->entry->next = vars;
    localVars = append(vars, localVars);
    findDuplicates(localVars);
    return prog;
}

ENTRY *subHeaderFun(_DATA_TYPE type, char* id, ENTRY* params){
    ENTRY *check = funLookup(id);
    if(check == NULL){
        ENTRY *func = (ENTRY*) malloc(sizeof (struct tENTRY));
        func->typ = _CALL;
        func->dataType = type;
        if(type != _VOID){
            res->dataType = type;
            res->next = params;
            localVars = res;
        }
        else{
            localVars = params;
        }
        func->id = id;
        func->ext.parList = params;
        func->next = NULL;
        ENTRY *toSave = (ENTRY*) malloc(sizeof (struct tENTRY));
        toSave->dataType = type;
        toSave->typ = _CALL;
        toSave->id = id;
        toSave->next = NULL;
        toSave->ext.parList = params;
        funcs = append(toSave, funcs);
        findDuplicates(globalVars);
        return func;
    }
    else{
        printf("Error: function '%s' already declared\n", id);
        exit(14);
    }
}

ENTRY* append(ENTRY* node, ENTRY* goal){
    ENTRY* head = goal;
    if(head == NULL){
        return node;
    }
    else{
        while(head->next != NULL){
            head = head->next;
        }
        head->next = node;
        return goal;
    }
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
    if(left == NULL){
        printf("Error: Empty expression\n");
        exit(101);
    }
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
    expr->dataType = _BOOL;
    expr->description.constant = "false";
    expr->parenthesis = 0;
    expr->next = NULL;
    return expr;
}

N_EXPR *numConversion(float num){
    N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
    expr->typ = CONSTANT;
    int tmp = (int)(10000*num)%10000;
    if(tmp == 0){
        expr->dataType = _INT;
    }
    else{
        expr->dataType = _REAL;
    }
    int len = snprintf(NULL, 0, "%f", num);
    char* res = (char *)(malloc(len + 1));
    snprintf(res, len+1, "%f", num);
    expr->description.constant = res;
    expr->parenthesis = 0;
    expr->next = NULL;
    return expr;
}

N_EXPR *identifiers(char* name, N_EXPR *extension, int type){
    N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
    if (type == 2) // function call
    {
        N_CALL * call = malloc(sizeof(struct tN_CALL));
        call->id = name;
        call->par_list = extension;
        expr->typ = FUNC_CALL;
        expr->description.func_call = call;
    }
    else{
        expr->typ = VAR_REF;
        N_VAR_REF * var = malloc(sizeof(struct tN_VAR_REF));
        var->id = name;
        if(type == 1) //array
        {
            var->index = extension;
        }
        else{
            var->index = NULL;
        }
        expr->description.var_ref = var;
    }
    expr->dataType = _VOID;
    expr->parenthesis = 0;
    expr->next = NULL;
    return expr;
}

N_ASSIGN *assingmentFun(char* name, N_EXPR *index, N_EXPR *rhs){
    N_VAR_REF* var = malloc(sizeof(struct tN_VAR_REF));
    var->id = name;
    var->index = index;
    N_ASSIGN* assign = malloc(sizeof (struct tN_ASSIGN));
    assign->var_ref = var;
    assign->rhs_expr = rhs;
    return assign;
}

void findDuplicates(ENTRY *scope){
    printScope();
    int res = 0;
    ENTRY *current = scope;
    ENTRY *next;
    while(current != NULL){
        next = current->next;
        while(next != NULL){
            if (strcmp(current->id, next->id) == 0){
                printf("Redeclaration of variable/function '%s' within one scope\n", current->id);
                res += 1;
            }
            next = next->next;
        }
        next = funcs;
        while(next != NULL){
            if (strcmp(current->id, next->id) == 0){
                printf("Redeclaration of variable/function '%s' within one scope\n", current->id);
                res += 1;
            }
            next = next->next;
        }
        current = current->next;
    }
    if(res != 0){
        printf("%d variable(s) duplicated\n", res);
        exit(12);
    }
}
ENTRY *funLookup(char* name){
    printf("in Lookup; goal -> %s\n", name);
    ENTRY *output = NULL;
    ENTRY *current = funcs;
    while(current != NULL){
        printf("%s\n", current->id);
        if(strcmp(current->id, name) == 0){
            output = current;
            break;
        }
        current = current->next;
    }
    return output;
}
ENTRY* variableLookup(char* name){
    printScope();
    printf("in Lookup; goal -> %s\n", name);
    ENTRY *output = NULL;
    ENTRY *current = globalVars;
    while(current != NULL){
        printf("%s\n", current->id);
        if(strcmp(current->id, name) == 0){
            output = current;
            break;
        }
        current = current->next;
    }
    current = localVars;
    printf("locals:\n");
    while(current != NULL){
        printf("%s\n", current->id);
        if(strcmp(current->id, name) == 0){
            output = current;
            break;
        }
        current = current->next;
    }
    return output;
}
void printScope(){
    printf("\tSCOPE\n");
    ENTRY *current = globalVars;
    while (current != NULL){
        printf("\tglobal: %s\n", current->id);
        current = current->next;
    }
    current = funcs;
    while (current != NULL){
        printf("\tfunctions: %s\n", current->id);
        current = current->next;
    }
    current = localVars;
    while (current != NULL){
        printf("\tlocal: %s\n", current->id);
        current = current->next;
    }
}
ENTRY *constructMain(){
    ENTRY *f = (ENTRY*) malloc(sizeof (struct tENTRY));
    f->typ = _CALL;
    f->id = "main";
    f->dataType = _MAIN;
    f->ext.parList = NULL;
    f->next = NULL;
    return f;
}