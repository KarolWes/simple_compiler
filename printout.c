//
// Created by Karol on 11.05.2023.
//


#include "ast.h"
#include <stdio.h>

extern int yyparse();
extern N_PROG *ast;
char* main_type = "int";
char* main_def = "int main(int argc, char *argv[])";

char * typeToStr(_DATA_TYPE type);
char *operatorToStr(_OPERATOR o);
void printVarRef(N_VAR_REF *input);
void printIf(N_IF *input);
void printWhile(N_WHILE *input);
void printCall(N_CALL *input, char* separator);
void printAssign(N_ASSIGN *input);
void printStatement(N_STMT* input, int ignore_marker);
void printExpr(N_EXPR *input, char* separator);
void printEntry(ENTRY *input);
void printProgramBase(N_PROG *input);
void printProgram(N_PROG *input);
void printArgs(ENTRY *input);

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
            return main_type; // this case depends on the language we want to print out
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
            return "/";
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
    printf("%s", input->id);
    if(input->index != NULL){
        printf("[");
        printExpr(input->index, "");
        printf("]");
    }
}

void printStatement(N_STMT *input, int ignore_marker) {
    int marker = 0;
    if(ignore_marker == 0 && input->next != NULL)
    {
        printf("{\n");
        marker = 1;
    }
    while(input != NULL){
        switch(input->typ){
            case _ASSIGN:
                printAssign(input->node.assign_stmt);
                break;
            case _IF:
                printIf(input->node.if_stmt);
                break;
            case _WHILE:
                printWhile(input->node.while_stmt);
                break;
            case _PROC_CALL:
                printCall(input->node.proc_call, ";\n");
                break;
        }
        input = input->next;
    }
    if (marker == 1){
        printf("}\n");
    }
}

void printExpr(N_EXPR *input, char* separator) {
    while(input != NULL){
        switch (input->typ) {
            case CONSTANT:
                printf("%s", input->description.constant);
                printf("%s", separator);
                break;
            case VAR_REF:
                printVarRef(input->description.var_ref);
                printf("%s", separator);
                break;
            case OP:
                if(input->description.operation.op == NOT_OP){
                    printf("! ");
                    printExpr(input->description.operation.expr, "");
                }
                else{
                    printExpr(input->description.operation.expr, "");
                    printf("%s ", operatorToStr(input->description.operation.op));
                }
                printf("%s", separator);
                break;
            case FUNC_CALL:
                printCall(input->description.func_call, separator);
                break;
        }
        input = input->next;
    }

}

void printAssign(N_ASSIGN *input) {
    printVarRef(input->var_ref);
    printf(" = ");
    printExpr(input->rhs_expr, "");
    printf(";\n");
}

void printCall(N_CALL *input, char* separator) {
    printf("%s (", input->id);
    printExpr(input->par_list, ", ");
    printf(")%s", separator);
}

void printWhile(N_WHILE *input) {
    printf("while (");
    printExpr(input->expr, "");
    printf(")\n");
    printStatement(input->stmt, 0);
}

void printIf(N_IF *input) {
    printf("if (");
    printExpr(input->expr, "");
    printf(")\n");
    printStatement(input->then_part, 0);
    if(input->else_part != NULL){
        printf("else\n");
        printStatement(input->else_part, 0);
    }
}

void printEntry(ENTRY *input) {
    while(input != NULL){
        switch (input->typ) {
            case _CONST:
                break;
            case _VAR:
                printf("%s ", typeToStr(input->dataType));
                printf("%s;\n",input->base.id);
                break;
            case _ARRAY:
                printf("%s ", typeToStr(input->dataType));
                printf("%s ",input->base.id);
                printf("[%d];\n", input->ext.bounds.high - input->ext.bounds.low);
                break;
            case _PROG:
                if (input->dataType == _MAIN){
                    printf("%s\n", main_def);
                }
                else{
                    printf("%s ", typeToStr(input->dataType));
                    printf("%s(", input->base.id);
                    printArgs(input->ext.prog.parList);
                }
                return;
            case _CALL:
                printf("%s ", typeToStr(input->dataType));
                printf("%s(",input->base.id);
                printArgs(input->ext.prog.parList);
                printf(");\n");
                break;
        }
        input = input->next;
    }
}

void printArgs(ENTRY *input) {
    while(input != NULL){
        printf("%s ", typeToStr(input->dataType));
        printf("%s", input->base.id);
        if(input->typ == _ARRAY){
            printf("[%d]", input->ext.bounds.high - input->ext.bounds.low);
        }
        if (input->next != NULL){
            printf(", ");
        }
        input = input->next;
    }
}

void printProgramBase(N_PROG *input) {
    N_PROG* main;
    while( input != NULL){
        if(input->entry->dataType == _MAIN){
            main = input;
            input = input->next;
        }
        printProgram(input);
        input = input->next;
    }
    printProgram(main);
}

void printProgram(N_PROG *input) {
    printEntry(input->entry); // header
    printf("{\n");
    ENTRY* vars = input->entry->next;
    if (vars != NULL){
        printEntry(vars);
    }
    printStatement(input->stmt, 1);// to improve
    printf("}\n");
}

int main() {
    yyparse();
    printProgramBase(ast);
}



