//
// Created by Karol on 11.05.2023.
//

#include "printout.h"

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
    if(input->index != nullptr){
        printf("[");
        printExpr(input->index);
        printf("]");
    }
}

void printStatement(N_STMT *input) {
    while(input != nullptr){
        switch(input->typ){
            case tN_STMT::_ASSIGN:
                printAssign(input->node.assign_stmt);
                break;
            case tN_STMT::_IF:
                printIf(input->node.if_stmt);
                break;
            case tN_STMT::_WHILE:
                printWhile(input->node.while_stmt);
                break;
            case tN_STMT::_PROC_CALL:
                printCall(input->node.proc_call);
                break;
        }
        input = input->next;
    }
}

void printExpr(N_EXPR *input) {
    while(input != nullptr){
        switch (input->typ) {
            case tN_EXPR::CONSTANT:
                printf("%s", input->description.constant);
                break;
            case tN_EXPR::VAR_REF:
                printVarRef(input->description.var_ref);
                break;
            case tN_EXPR::OP:
                if(input->description.operation.op == NOT_OP){
                    printf("! ");
                    printExpr(input->description.operation.expr);
                }
                else{
                    printExpr(input->description.operation.expr);
                    printf("%s ", operatorToStr(input->description.operation.op));
                }
                break;
            case tN_EXPR::FUNC_CALL:
                printCall(input->description.func_call);
                break;
        }
        input = input->next;
    }

}

