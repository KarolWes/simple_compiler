//
// Created by Karol on 25.05.2023.
//


#include "parser_functions.h"

char* inFun = "";

/* parsing functions */

void startFun(char* id, ENTRY* vars){
    funcs = constructMain();
    res = (ENTRY*)malloc(sizeof(struct tENTRY));
    res->typ = _VAR;
    res->id = "result";
    res->next = NULL;
    globalVars = vars;
    findDuplicates(globalVars);
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
            printf("Error in line %d: array bound must be an int\n", yylineno);
            free(output);
            exit(11);
        }
        new_lower = (int)(lower);
        int new_upper = (int)(upper*10000);
        if(new_upper%10000 != 0){
            printf("Error in line %d: array bound must be an int\n", yylineno);
            free(output);
            exit(11);
        }
        new_upper = (int)(upper);
        if(new_lower > new_upper){
            printf("Error in line %d: array declaration with second index lower than the first\n", yylineno);
            free(output);
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
        inFun = id;
        ENTRY *func = (ENTRY*) malloc(sizeof (struct tENTRY));
        func->typ = _CALL;
        func->dataType = type;
        if(type != _VOID){
            res->dataType = type;
            res->next = extractParams(params);
            localVars = res;
        }
        else{
            localVars = extractParams(params);
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
        printf("Error in line %d: redeclaration of function '%s'\n", yylineno, id);
        exit(14);
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
        printf("Error in line %d: Empty expression\n", yylineno);
        exit(101);
    }
    N_EXPR * expr = malloc(sizeof(struct tN_EXPR));
    expr->typ = OP;
    if (op == NOT_OP || op == AND_OP || op == OR_OP){
        if(left->dataType != _BOOL || right -> dataType != _BOOL){
            printf("Error in line %d: Logical operator not declared for given data type\n", yylineno);
            free(expr);
            exit(19);
        }
        expr->dataType = _BOOL;
    }
    else if(op == NEQ_OP || op == GEQ_OP || op == LEQ_OP ||
            op == GT_OP || op == LT_OP || op == EQ_OP){
        if(((right->dataType == _INT || right->dataType == _REAL) &&
            (left->dataType == _INT || left->dataType == _REAL))||
            (right->dataType == _BOOL && left->dataType == _BOOL)){
            expr->dataType = _BOOL;
        }
        else{
            printf("Error in line %d: Relation operator not declared for given data type\n", yylineno);
            free(expr);
            exit(19);
        }
    }
    else if (op == DIV_OP){
        if((left->dataType == _INT || left->dataType == _REAL) && right->dataType == _INT){
            expr->dataType = _INT;
        }
        else{
            printf("Error in line %d: DIV operator not declared for given data type\n", yylineno);
            free(expr);
            exit(19);
        }
    }
    else if(op == MOD_OP){
        if(left->dataType == _INT && right->dataType == _INT){
            expr->dataType = _INT;
        }
        else{
            printf("Error in line %d: Modulo operator not declared for given data type\n", yylineno);
            free(expr);
            exit(19);
        }
    }
    else if(op == SLASH_OP){
        if((right->dataType == _INT || right->dataType == _REAL) &&
            (left->dataType == _INT || left->dataType == _REAL)){
            expr->dataType = _REAL;
        }
        else{
            printf("Error in line %d: Division operator not declared for given data type\n", yylineno);
            free(expr);
            exit(19);
        }
    }
    else{
        if((right->dataType == _INT || right->dataType == _REAL) &&
           (left->dataType == _INT || left->dataType == _REAL)){
            if(right->dataType == _REAL || left->dataType == _REAL){
                expr->dataType = _REAL;
            }else{
                expr->dataType = _INT;
            }
        }
        else{
            printf("Error in line %d: Math operator not declared for given data type\n", yylineno);
            free(expr);
            exit(19);
        }
    }
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
    expr->description.constant = val;
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
        ENTRY *ans = funLookup(name);
        if(ans == NULL){
            printf("Error in line %d: function '%s' referenced without declaration\n", yylineno, name);
            free(expr);
            exit(14);
        }
        call->par_list = extension;
        checkParameters(ans, extension);
        expr->typ = FUNC_CALL;
        expr->description.func_call = call;
        expr->dataType = ans->dataType;
    }
    else{
        expr->typ = VAR_REF;
        N_VAR_REF * var = malloc(sizeof(struct tN_VAR_REF));
        if(strcmp(name, inFun) == 0){
            name = "result";
        }
        var->id = name;
        ENTRY *ans = variableLookup(name);
        if(ans == NULL){
            printf("Error in line %d: variable '%s' referenced without declaration\n", yylineno, name);
            free(expr);
            exit(13);
        }
        if(type == 1) //array
        {
            checkIndex(extension, ans);
            var->index = extension;
        }
        else{
            var->index = NULL;
        }
        expr->description.var_ref = var;
        expr->dataType = ans->dataType;
    }
    expr->parenthesis = 0;
    expr->next = NULL;
    return expr;
}

N_ASSIGN *assignmentFun(char* name, N_EXPR *index, N_EXPR *rhs){
    N_VAR_REF* var = malloc(sizeof(struct tN_VAR_REF));
    if(strcmp(name, inFun) == 0){
        name = "result";
    }
    var->id = name;
    ENTRY* e = variableLookup(name);
    if(e == NULL){
        printf("Error in line %d: variable '%s' referenced without declaration\n", yylineno, name);
        free(var);
        exit(13);
    }
    var->index = index;
    if(index != NULL){
        checkIndex(index, e);
    }
    N_ASSIGN* assign = malloc(sizeof (struct tN_ASSIGN));
    assign->var_ref = var;
    assign->rhs_expr = rhs;
    if(e->typ == _ARRAY && index == NULL){
        if(rhs->typ == VAR_REF){
            ENTRY *r = variableLookup(rhs->description.var_ref->id);
            if(!(e->ext.bounds.low == r->ext.bounds.low && e->ext.bounds.high == r->ext.bounds.high)){
                printf("Error in line %d: arrays' bounds not matching\n", yylineno);
                free(var);
                free(assign);
                exit(11);
            }
        }
    }
    int test;
    if (e->typ == _ARRAY){
        test = 1;
    }
    else{
        test = 0;
    }
    checkType(e->dataType, test, rhs);
    return assign;
}

N_CALL *procCallFun(char* name, N_EXPR *params){
    N_CALL* call = malloc(sizeof (struct tN_CALL));
    call->id = name;
    ENTRY *e = funLookup(name);
    if(e == NULL){
        printf("Error in line %d: Function '%s' referenced without declaration\n", yylineno, name);
        free(call);
        exit(14);
    }
    call->par_list = params;
    checkParameters(e, params);
    return call;
}

// utilities

ENTRY* variableLookup(char* name){
    ENTRY *output = NULL;
    ENTRY *current = globalVars;
    while(current != NULL){
        if(strcmp(current->id, name) == 0){
            output = current;
            break;
        }
        current = current->next;
    }
    current = localVars;
    while(current != NULL){
        if(strcmp(current->id, name) == 0){
            output = current;
            break;
        }
        current = current->next;
    }
    return output;
}

ENTRY *funLookup(char* name){
    ENTRY *output = NULL;
    ENTRY *current = funcs;
    while(current != NULL){
        if(strcmp(current->id, name) == 0){
            output = current;
            break;
        }
        current = current->next;
    }
    return output;
}

void findDuplicates(ENTRY *scope){
//    printScope();
    int res = 0;
    ENTRY *current = scope;
    ENTRY *next;
    while(current != NULL){
        next = current->next;
        while(next != NULL){
            if (strcmp(current->id, next->id) == 0){
                printf("Error in line %d: Redeclaration of variable/function '%s' within one scope\n", yylineno, current->id);
                res += 1;
            }
            next = next->next;
        }
        next = funcs;
        while(next != NULL){
            if (strcmp(current->id, next->id) == 0){
                printf("Error in line %d: Redeclaration of variable/function '%s' within one scope\n", yylineno, current->id);
                res += 1;
            }
            next = next->next;
        }
        current = current->next;
    }
    if(res != 0){
        printf("\t%d variable(s) duplicated\n", res);
        exit(12);
    }
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

void checkType(_DATA_TYPE lhs, int array,  N_EXPR *rhs) {
    if(rhs->typ == CONSTANT) {
        if (lhs == _BOOL) {
            if (strcmp(rhs->description.constant, "true") != 0 &&
                strcmp(rhs->description.constant, "false") != 0) {
                printf("Error in line %d: Type mismatch: bool <- other\n", yylineno);
                exit(17);
            }
        } else if (lhs == _INT) {
            if (strcmp(rhs->description.constant, "true") == 0 ||
                strcmp(rhs->description.constant, "false") == 0) {
                printf("Error in line %d: Type mismatch: int <- bool\n", yylineno);
                exit(17);
            }
            float num = atof(rhs->description.constant);
            int decimal = (int) (num * 10000) % 10000;
            if (decimal != 0) {
                printf("Error in line %d: Type mismatch: int <- float\n", yylineno);
                exit(17);
            }
        } else if (lhs == _REAL) {
            if (strcmp(rhs->description.constant, "true") == 0 ||
                strcmp(rhs->description.constant, "false") == 0) {
                printf("Error in line %d: Type mismatch: float <- bool\n", yylineno);
                exit(17);
            }
        }
    }
    else {
        if ((rhs->dataType == _INT || rhs->dataType == _REAL) && lhs == _BOOL) {
            printf("Error in line %d: Type mismatch: bool <- other\n", yylineno);
            exit(17);
        }
        if ((lhs == _INT || lhs == _REAL) && rhs->dataType == _BOOL) {
            printf("Error in line %d: Type mismatch: other <- bool\n", yylineno);
            exit(17);
        }
        if (rhs->typ == VAR_REF) {
            ENTRY *e = variableLookup(rhs->description.var_ref->id);
            if (e->typ == _ARRAY && rhs->description.var_ref->index == NULL && array == 0) {
                printf("Error in line %d: Type mismatch: scalar <- array\n", yylineno);
                exit(17);
            }
        } else if (rhs->typ == FUNC_CALL) {
            if (rhs->dataType == _VOID) {
                printf("Error in line %d: Type mismatch: other <- NONE\n", yylineno);
                exit(17);
            }
        }
    }
}

void checkIndex(N_EXPR *index, ENTRY *def){
    if (index->typ == CONSTANT) {
        if (strcmp(index->description.constant, "true") == 0 ||
        strcmp(index->description.constant, "false") == 0) {
            printf("Error in line %d: Illegal array subscription: Boolean\n", yylineno);
            exit(11);
        } else {
            float num = atof(index->description.constant);
            int decimal = (int) (num * 10000) % 10000;
            if (decimal != 0) {
                printf("Error in line %d: Illegal array subscription: Float\n", yylineno);
                exit(11);
            } else {
                int num_i = (int) (num);
                if (num_i < def->ext.bounds.low || num_i > def->ext.bounds.high) {
                    printf("Error in line %d: Illegal array subscription: Index out of bound", yylineno);
                    exit(11);
                }
            }
        }
    }
    if (index->typ == VAR_REF) {
        ENTRY *e = variableLookup(index->description.var_ref->id);
        if (e->dataType != _INT) {
            printf("Error in line %d: Illegal array subscription: Variable is not int\n", yylineno);
            exit(11);
        }
    }
    if (index->typ == FUNC_CALL) {
        ENTRY *e = funLookup(index->description.var_ref->id);
        if (e->dataType != _INT) {
            printf("Error in line %d: Illegal array subscription: Function is not int\n", yylineno);
            exit(11);
        }
    }
    if(index->typ == OP){
        if(index->dataType != _INT){
            printf("Error in line %d: Illegal array subscription: Expression is not int\n", yylineno);
            exit(11);
        }
    }
}

void checkParameters(ENTRY* fun, N_EXPR* pars){
    ENTRY* formal = fun->ext.parList;
    int i = 0;
    while(formal != NULL){
        if(pars == NULL){
            printf("Error in line %d: Too few arguments provided for function '%s'. "
                   "Should be more than %d\n", yylineno, fun->id, i);
            exit(15);
        }
        int test;
        if (formal->typ == _ARRAY){
            test = 1;
        }
        else{
            test = 0;
        }
        checkType(formal->dataType, test, pars);
        formal = formal->next;
        pars = pars->next;
        i += 1;
    }
    if(pars != NULL){
        printf("Error in line %d: Too many arguments provided for function '%s'. "
               "Should be %d\n", yylineno, fun->id, i);
        exit(15);
    }
}

ENTRY* extractParams(ENTRY* params) {
    ENTRY *head = NULL;
    ENTRY *current = NULL;
    while(params != NULL){
        ENTRY *entry = (ENTRY*) malloc(sizeof (struct tENTRY));
        entry->typ = params->typ;
        entry->id = params ->id;
        entry->dataType = params->dataType;
        if(entry->typ == _ARRAY){
            entry->ext = params->ext;
        }
        entry->next = NULL;
        if(head == NULL){
            head = entry;
            current = head;
        }
        else{
            current->next = entry;
            current = current->next;
        }
        params = params->next;
    }
    return head;
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