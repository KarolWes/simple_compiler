//
// Created by Karol on 11.05.2023.
//
#include <stdlib.h>
#include "printout.h"


_DATA_TYPE mainType = _INT;
char* inFun = "";
char* mainDef = "int main(int argc, char *argv[])";
int indentLevel = 0;
_DATA_TYPE returnType = 0;


char *typeToStr(_DATA_TYPE type) {
    switch (type) {
        case _BOOL:
            return "bool";
        case _INT:
            return "int";
        case _REAL:
            return "float";
        case _VOID:
            return "void";
        case _MAIN:
            return typeToStr(mainType); // this case depends on the language we want to print out
    }
}

char *operatorToStr(_OPERATOR o) {
    switch (o) {
        case NO_OP:
            return "";
        case EQ_OP:
            return "==";
        case NEQ_OP:
            return "!=";
        case GT_OP:
            return ">";
        case GEQ_OP:
            return ">=";
        case LT_OP:
            return "<";
        case LEQ_OP:
            return "<=";
        case PLUS_OP:
            return "+";
        case MINUS_OP:
            return "-";
        case MULTI_OP:
            return "*";
        case SLASH_OP:
            return "* 1.0 /";
        case DIV_OP:
            return "/";
        case MOD_OP:
            return "%";
        case AND_OP:
            return "&&";
        case OR_OP:
            return "||";
        case NOT_OP:
            return "!";
    }
}

void printVarRef(N_VAR_REF *input) {
    if(input == NULL){
        return;
    }
    ENTRY* ans;
    if(strcmp(input->id, inFun) == 0){
        ans = variableLookup("result");
    }else{
        ans = variableLookup(input->id);
    }
    if (ans == NULL)
    {
        printf("Variable '%s' referenced without declaration\n", input->id);
        exit(12);
    }
    if(strcmp(input->id, inFun) == 0){
        printf("result");
        fprintf(f, "result");
    }
    else {
        printf("%s", input->id);
        fprintf(f, "%s", input->id);
        if (input->index != NULL) {
            printf("[");
            fprintf(f, "[");
            N_EXPR* i = input->index;
            if(i->typ == CONSTANT){
                printf("Illegal array subscription: Boolean\n");
                exit(13);
            }
            if(i->typ == VAR_REF){
                ENTRY *e = variableLookup(i->description.var_ref->id);
                if(e->dataType != _INT){
                    printf("Illegal array subscription\n");
                    exit(13);
                }
            }
            printExpr(i, "");
            printf("]");
            fprintf(f, "]");
        }
    }
    free(input);
}

void printStatement(N_STMT *input, int ignore_marker, int ignore_indent) {
    if(input == NULL){
        return;
    }
    int marker = 0;
    if(ignore_marker == 0 && input->next != NULL)
    {
        printIndent();
        printf("{\n");
        fprintf(f, "{\n");
        marker = 1;
    }
    indentLevel+=1;
    while(input != NULL){
        switch(input->typ){
            case _ASSIGN:
                printAssign(input->node.assign_stmt, ignore_indent);
                break;
            case _IF:
                printIf(input->node.if_stmt, ignore_indent);
                break;
            case _WHILE:
                printWhile(input->node.while_stmt, ignore_indent);
                break;
            case _PROC_CALL:
                printCall(input->node.proc_call, ";\n", ignore_indent);
                break;
        }
        N_STMT *old = input;
        input = input->next;
        free(old);
    }
    indentLevel-=1;
    if (marker == 1){
        printIndent();
        printf("}\n");
        fprintf(f, "}\n");
    }
    free(input);
}

void printExpr(N_EXPR *input, char* separator) {
    while(input != NULL){
        printf("in expr\n");
        printExprInner(input, separator);
        if (input->next != NULL){
            printf("%s", separator);
            fprintf(f, "%s", separator);
        }
        N_EXPR *old = input;
        input = input->next;
        free(old);
    }
    free(input);
}

void printExprInner(N_EXPR *input, char* separator) {

    float num;
    ENTRY *ans = NULL;
    if(input->parenthesis == 1){
        printf("(");
        fprintf(f, "(");
    }
    switch (input->typ) {
        case CONSTANT:
            num = atof(input->description.constant);
            int tmp = (int)(num * 10000);
            if (tmp % 10000 == 0){
                tmp  = (int) num;
                printf("%d", tmp);
                fprintf(f, "%d", tmp);
            }
            else{
                printf("%f", num);
                fprintf(f, "%f", num);
            }
            break;
        case VAR_REF:
            printVarRef(input->description.var_ref);
            break;
        case FUNC_CALL:
            printCall(input->description.func_call, separator, 1);
            break;
        case OP:
            printOp(input);
            break;
    }
    if(input->parenthesis == 1){
        printf(")");
        fprintf(f,")");
    }

}

void printOp(N_EXPR  *input){
    if(input->description.operation.op == NOT_OP){
        printf("! ");
        fprintf(f, "! ");
        printExprInner(input->description.operation.expr, "");
//        free(input->description.operation.expr);
    }
    else{
        N_EXPR *e = input->description.operation.expr;
        while(e!= NULL){
            printExprInner(e, "");
            if (e->next != NULL)
            {
                printf(" %s ", operatorToStr(input->description.operation.op));
                fprintf(f, " %s ", operatorToStr(input->description.operation.op));
            }
            N_EXPR *old = e;
            e = e->next;
//            free(old);
        }
    }
//    free(input);
}

void printAssign(N_ASSIGN *input, int ignore_indent) {
    if(input == NULL){
        return;
    }
    if(ignore_indent == 0){
        printIndent();
    }
    printVarRef(input->var_ref);
    printf(" = ");
    fprintf(f, " = ");
    printExpr(input->rhs_expr, "");
    printf(";\n");
    fprintf(f, ";\n");
}

void printCall(N_CALL *input, char* separator, int ignore_indent) {
    if(input == NULL){
        return;
    }
    if(ignore_indent == 0){
        printIndent();
    }
    ENTRY * fun = funLookup(input->id);
    if(fun == NULL){
        printf("Function referenced without declaration: '%s'\n", input->id);
        exit(13);
    }
    printf("%s(", input->id);
    fprintf(f, "%s(", input->id);
    checkParameters(fun, input->par_list);
    printExpr(input->par_list, ", ");
    printf(")%s", separator);
    fprintf(f, ")%s", separator);
}

void checkParameters(ENTRY* fun, N_EXPR* pars){
    ENTRY* formal = fun->ext.parList;
    int i = 0;
    while(formal != NULL){
        printf("%s\n", formal->id);
        if(pars == NULL){
            printf("Too few arguments provided for function '%s'. Should be more than %d\n", fun->id, i);
            exit(14);
        }
        formal = formal->next;
        pars = pars->next;
        i += 1;
    }
    if(pars != NULL){
        printf("Too many arguments provided for function '%s'. Should be %d\n", fun->id, i);
        exit(14);
    }
}

void printWhile(N_WHILE *input, int ignore_indent) {
    if(input == NULL){
        return;
    }
    if(ignore_indent == 0){
        printIndent();
    }
    printf("while (");
    fprintf(f, "while (");
    printExpr(input->expr, "");
    printf(")\n");
    fprintf(f, ")\n");
    printStatement(input->stmt, 0, 0);
}

void printIf(N_IF *input, int ignore_indent) {
    if(input == NULL){
        return;
    }
    if(ignore_indent == 0){
        printIndent();
    }
    printf("if (");
    fprintf(f, "if (");
    printExpr(input->expr, "");
    printf(")\n");
    fprintf(f, ")\n");
    printStatement(input->then_part, 0, 0);
    if(input->else_part != NULL){
        printIndent();
        printf("else\n");
        fprintf(f, "else\n");
        printStatement(input->else_part, 0, 0);
    }
}

void printEntry(ENTRY *input) {
    ENTRY *check = NULL;
    while(input != NULL){
        printIndent();
        switch (input->typ) {
            case _CONST:
                break;
            case _VAR:
                printf("\t\tFUN: %s\n", inFun);
                if(strcmp(input->id, inFun) == 0){
                    check = variableLookup("result");
                }else{
                    check = variableLookup(input->id);
                }
                if (check == NULL)
                {
                    printf("Variable '%s' referenced without declaration\n", input->id);
                    exit(12);
                }
                printf("%s ", typeToStr(input->dataType));
                fprintf(f, "%s ", typeToStr(input->dataType));
                printf("%s;\n",input->id);
                fprintf(f, "%s;\n",input->id);
                break;
            case _ARRAY:
                printf("%s ", typeToStr(input->dataType));
                fprintf(f, "%s ", typeToStr(input->dataType));
                printf("%s ",input->id);
                fprintf(f, "%s ",input->id);
                printf("[%d];\n", input->ext.bounds.high - input->ext.bounds.low+1);
                fprintf(f, "[%d];\n", input->ext.bounds.high - input->ext.bounds.low+1);
                break;
            case _PROG:
                if (input->dataType == _MAIN){
                    printf("\n\t/* program: '%s' */\n\n\n", input->id);
                    fprintf(f, "\n\t/* program: '%s' */\n\n\n", input->id);
                    inFun = "main";
                }
                returnType = input->dataType;
                if (input->dataType == _MAIN){
                    printf("%s\n", mainDef);
                    fprintf(f, "%s\n", mainDef);
                }
                else{
                    inFun = input->id;
                    printf("%s ", typeToStr(input->dataType));
                    fprintf(f, "%s ", typeToStr(input->dataType));
                    printf("%s(", input->id);
                    fprintf(f, "%s(", input->id);
                    printArgs(input->ext.parList);
                }
                return;
            case _CALL:
                check = funcs;
                if(funcs == NULL){
                    funcs = input;
                }
                else{
                    printf("Checking next\n");
                    while(check->next != NULL){
                        check = check->next;
                        printf("NEXT!");
                    }
                    check->next = input;
                    check = check->next;
                    check->next = NULL;
                }
                findDuplicates(globalVars);
                inFun = input->id;
                printf("\t%s\n", inFun);
                returnType = input->dataType;
                printf("%s ", typeToStr(input->dataType));
                fprintf(f, "%s ", typeToStr(input->dataType));
                printf("%s(",input->id);
                fprintf(f, "%s(",input->id);
                printf("In call:\n");
                if(input->ext.parList != NULL){
                    printf("There is something to be added\n");
                    printScope();
                    ENTRY *vars = localVars;
                    if(vars == NULL){
                        localVars = extractParams(input->ext.parList);
                    }
                    else{
                        printf("Trying to go to next\n");
                        while(vars->next!= NULL){
                            printf("NEXT!\n");
                            vars = vars->next;
                        }
                        vars->next = extractParams(input->ext.parList);
                    }
                }
                printArgs(input->ext.parList);
                printf(")\n");
                fprintf(f, ")\n");
                return;
            default:
                printf("Unidentified type: %d\n", input->typ);
                fprintf(f, "Unidentified type: %d\n", input->typ);
        }
        input = input->next;
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

void printArgs(ENTRY *input) {
    while(input != NULL){
        printf("%s ", typeToStr(input->dataType));
        fprintf(f, "%s ", typeToStr(input->dataType));
        printf("%s", input->id);
        fprintf(f, "%s", input->id);
        if(input->typ == _ARRAY){
            printf("[%d]", input->ext.bounds.high - input->ext.bounds.low+1);
            fprintf(f, "[%d]", input->ext.bounds.high - input->ext.bounds.low+1);
        }
        if (input->next != NULL){
            printf(", ");
            fprintf(f, ", ");
        }
        input = input->next;
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

void printProgramBase(N_PROG *input, int set_global) {
    funcs = constructMain();
    N_PROG* main;
    while( input != NULL){
        localVars = NULL;
        res->next = NULL;
        printf("Local variables should be flushed\n");
        if(input->entry->dataType != _VOID){
            res->typ = input->entry->dataType;
            localVars = res;
        }
        if(input->entry->dataType == _MAIN){
            printf("Type is main. Global: %d\n", set_global);
            main = input;
            input = input->next;
            if(set_global == 1){
                printf("\t/* Global variables */\n");
                fprintf(f, "\t/* Global variables */\n");
                ENTRY* vars = main->entry->next;
                globalVars = vars;
                findDuplicates(vars);
                if (vars != NULL){
                    printEntry(vars);
                }
                printf("\n");
                fprintf(f, "\n");
            }
            else{
                globalVars = NULL;
            }
        }
        if(input != NULL){
            printProgram(input, set_global);
            N_PROG *old = input;
            input = input->next;
//            free(old);
        }
        printf("\n");
        fprintf(f, "\n");
    }
    printProgram(main, set_global);
    globalVars = NULL;
//    free(main);
}

void printProgram(N_PROG *input, int set_global) {
    indentLevel = 0;
    ENTRY* vars = NULL;
    if (input != NULL){
        printIndent();
        vars = input->entry->next;
        printEntry(input->entry); // header
        printf("{\n");
        fprintf(f, "{\n");
        indentLevel+=1;
        if(returnType != _MAIN || set_global != 1){
            printf("Trying to add variables\n");
            printScope();
            ENTRY *tmp = localVars;
            if(localVars == NULL){
                localVars = vars;
            }
            else{
                printf("Checking next\n");
                while(tmp->next != NULL){
                    tmp = tmp->next;
                    printf("NEXT!");
                }
                tmp->next = vars;
            }
            findDuplicates(localVars);
            if (vars != NULL){
                printEntry(vars);
            }
        }
        else{
            localVars = NULL;
        }
        if(returnType != _VOID && returnType != _MAIN){
            printIndent();
            printf("%s %s;\n", typeToStr(returnType), "result");
            fprintf(f, "%s %s;\n", typeToStr(returnType), "result");
        }
        indentLevel-=1;
        printStatement(input->stmt, 1, 0);
        if(returnType != _VOID){
            printIndent();
            if(returnType != _MAIN)
            {
                printf("\treturn result;\n");
                fprintf(f, "\treturn result;\n");
            }
            else{
                printf("\treturn 0;\n");
                fprintf(f, "\treturn 0;\n");
            }
        }
        indentLevel-=1;
        printIndent();
        printf("}\n");
        fprintf(f, "}\n");
        inFun = "";
        localVars = NULL;
//        free(input->entry);
//        free(input->entry->ext.parList);
        //cleanSymTable(vars);
    }
}

void printIndent() {
    for(int i = 0; i < indentLevel; i++){
        printf("\t");
        fprintf(f, "\t");
    }
}

void run(int set_global) {
    res = (ENTRY*)malloc(sizeof(struct tENTRY));
    res->id = "result";
    res->next = NULL;

    if(set_global != 1 && set_global != 0){
        printf("Unavailable value. Try 1 or 0");
        exit(3);
    }
    f = fopen("new_code.c", "w");
    if(f == NULL){
        printf("Error opening file\n");
        exit(10);
    }
    printf("In main, start parsing...\n");
    yyparse();
    printf("Parsing finished...\n\n\n");
    printProgramBase(ast, set_global);
    fprintf(f, "\n\n\t /* Created using parser by Karol Wesolowski */");
    fclose(f);
    free(ast);
}

void cleanSymTable(ENTRY *symTab) {
    ENTRY  *new = NULL;
    while(symTab != NULL){
        new = symTab->next;
        free(symTab);
        symTab = new;
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


ENTRY *append(ENTRY *a, ENTRY *b) {
    if (b!= NULL)
    {
        if (a != NULL){
            printf("Var was not null\n");
            printf("a->next\n");
            printf("%d\n", a->next);
            while(a->next!= NULL){
                printf("NEXT!\n");
                a = a->next;
            }
            a->next = b->ext.parList;
        }
        else{
            printf("Var was null\n");
            a = b->ext.parList;
        }
        printf("appended successfully\n");
    }
    else{
        printf("Nothing to append\n");
    }
    return a;
}

