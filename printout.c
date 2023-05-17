//
// Created by Karol on 11.05.2023.
//
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
    if(strcmp(input->id, inFun) == 0){
        printf("result");
    }
    else {
        printf("%s", input->id);
        if (input->index != NULL) {
            printf("[");
            printExpr(input->index, "");
            printf("]");
        }
    }
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
        input = input->next;
    }
    indentLevel-=1;
    if (marker == 1){
        printIndent();
        printf("}\n");
    }
}

void printExpr(N_EXPR *input, char* separator) {
    while(input != NULL){
        printExprInner(input, separator);
        if (input->next != NULL){
            printf("%s", separator);
        }
        input = input->next;
    }

}

void printExprInner(N_EXPR *input, char* separator) {
    switch (input->typ) {
        case CONSTANT:
            printf("%s", input->description.constant);
            break;
        case VAR_REF:
            printVarRef(input->description.var_ref);
            break;
        case FUNC_CALL:
            printCall(input->description.func_call, separator, 1);
            break;
        case OP: // problem
            printOp(input);
            break;
    }
}

void printOp(N_EXPR  *input){
    if(input->description.operation.op == NOT_OP){
        printf("! ");
        printExprInner(input->description.operation.expr, "");
    }
    else{
        N_EXPR *e = input->description.operation.expr;
        while(e!= NULL){
            printExprInner(e, "");
            if (e->next != NULL)
            printf(" %s ", operatorToStr(input->description.operation.op));
            e = e->next;
        }
    }

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
    printExpr(input->rhs_expr, "");
    printf(";\n");
}

void printCall(N_CALL *input, char* separator, int ignore_indent) {
    if(input == NULL){
        return;
    }
    if(ignore_indent == 0){
        printIndent();
    }
    printf("%s(", input->id);
    printExpr(input->par_list, ", ");
    printf(")%s", separator);
}

void printWhile(N_WHILE *input, int ignore_indent) {
    if(input == NULL){
        return;
    }
    if(ignore_indent == 0){
        printIndent();
    }
    printf("while (");
    printExpr(input->expr, "");
    printf(")\n");
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
    printExpr(input->expr, "");
    printf(")\n");
    printStatement(input->then_part, 0, 0);
    if(input->else_part != NULL){
        printIndent();
        printf("else\n");
        printStatement(input->else_part, 0, 0);
    }
}

void printEntry(ENTRY *input) {
    while(input != NULL){
        printIndent();
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
                printf("[%d];\n", input->ext.bounds.high - input->ext.bounds.low+1);
                break;
            case _PROG:
                if (input->dataType == _MAIN){
                    printf("\n\t/* program: '%s' */\n\n\n", input->base.id);
                    inFun = "main";
                }
                returnType = input->dataType;
                if (input->dataType == _MAIN){
                    printf("%s\n", mainDef);
                }
                else{
                    inFun = input->base.id;
                    printf("%s ", typeToStr(input->dataType));
                    printf("%s(", input->base.id);
                    printArgs(input->ext.prog.parList);
                }
                return;
            case _CALL:
                inFun = input->base.id;
                returnType = input->dataType;
                printf("%s ", typeToStr(input->dataType));
                printf("%s(",input->base.id);
                printArgs(input->ext.prog.parList);
                printf(")\n");
                return;
            default:
                printf("Unidentified type: %d\n", input->typ);
        }
        input = input->next;
    }
}

void printArgs(ENTRY *input) {
    while(input != NULL){
        printf("%s ", typeToStr(input->dataType));
        printf("%s", input->base.id);
        if(input->typ == _ARRAY){
            printf("[%d]", input->ext.bounds.high - input->ext.bounds.low+1);
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
        if(input != NULL){
            printProgram(input);
            input = input->next;
        }
        printf("\n");
    }
    printProgram(main);
}

void printProgram(N_PROG *input) {
    indentLevel = 0;
    if (input != NULL){
        printIndent();
        printEntry(input->entry); // header
        printf("{\n");
        indentLevel+=1;
        ENTRY* vars = input->entry->next;
        if (vars != NULL){
            printEntry(vars);
        }
        if(returnType != _VOID && returnType != _MAIN){
            printIndent();
            printf("%s %s;\n", typeToStr(returnType), "result");
        }
        indentLevel-=1;
        printStatement(input->stmt, 1, 0);
        if(returnType != _VOID){
            printIndent();
            if(returnType != _MAIN)
            {
                printf("\treturn result;\n");
            }
            else{
                printf("\treturn 0;\n");
            }
        }
        indentLevel-=1;
        printIndent();
        printf("}\n");
        inFun = "";
    }
}

void printIndent() {
    for(int i = 0; i < indentLevel; i++){
        printf("\t");
    }
}

void run() {
    printf("In main, start parsing...\n");
    yyparse();
    printf("Parsing finished...\n\n\n");
    printProgramBase(ast);
}



