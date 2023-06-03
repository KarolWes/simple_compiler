//
// Created by Karol on 11.05.2023.
//
#include "printout.h"

_DATA_TYPE mainType = _INT;
char* mainDef = "int main(int argc, char *argv[])";
int indentLevel = 0;
_DATA_TYPE returnType = 0;

void printIndent() {
    for(int i = 0; i < indentLevel; i++){
        printf("\t");
        fprintf(f, "\t");
    }
}

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

void printEntry(ENTRY *input) {
    while(input != NULL){
        printIndent();
        switch (input->typ) {
            case _CONST:
                break;
            case _VAR:
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
                }
                returnType = input->dataType;
                if (input->dataType == _MAIN){
                    printf("%s\n", mainDef);
                    fprintf(f, "%s\n", mainDef);
                }
                else{
                    printf("%s ", typeToStr(input->dataType));
                    fprintf(f, "%s ", typeToStr(input->dataType));
                    printf("%s(", input->id);
                    fprintf(f, "%s(", input->id);
                    printArgs(input->ext.parList);
                }
                return;
            case _CALL:
                returnType = input->dataType;
                printf("%s ", typeToStr(input->dataType));
                fprintf(f, "%s ", typeToStr(input->dataType));
                printf("%s(",input->id);
                fprintf(f, "%s(",input->id);
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

void printVarRef(N_VAR_REF *input) {
    if(input == NULL){
        return;
    }
    printf("%s", input->id);
    fprintf(f, "%s", input->id);
    if (input->index != NULL) {
        printf("[");
        fprintf(f, "[");
        N_EXPR *i = input->index;
        printExpr(i, "");
        printf("]");
        fprintf(f, "]");
    }
    //free(input);
}

void printExpr(N_EXPR *input, char* separator) {
    while(input != NULL){
        printExprInner(input, separator);
        if (input->next != NULL){
            printf("%s", separator);
            fprintf(f, "%s", separator);
        }
        N_EXPR *old = input;
        input = input->next;
        //free(old);
    }
    //free(input);
}

void printExprInner(N_EXPR *input, char* separator) {
    float num;
    if(input->parenthesis == 1){
        printf("(");
        fprintf(f, "(");
    }
    switch (input->typ) {
        case CONSTANT:
            if(strcmp(input->description.constant, "true") == 0 || strcmp(input->description.constant, "false") == 0 ){
                printf("%s", input->description.constant);
                fprintf(f, "%s", input->description.constant);
            }
            else{
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

void printCall(N_CALL *input, char* separator, int ignore_indent) {
    if(input == NULL){
        return;
    }
    if(ignore_indent == 0){
        printIndent();
    }
    printf("%s(", input->id);
    fprintf(f, "%s(", input->id);
    printExpr(input->par_list, ", ");
    printf(")%s", separator);
    fprintf(f, ")%s", separator);
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
            default:
                printf("Type not recognised\n");
                exit(102);
        }
        N_STMT *old = input;
        input = input->next;
        //free(old);
    }
    indentLevel-=1;
    if (marker == 1){
        printIndent();
        printf("}\n");
        fprintf(f, "}\n");
    }
    //free(input);
}

void printProgramBase(N_PROG *input, int set_global) {
    N_PROG* main;
    while( input != NULL){
        if(input->entry->dataType == _MAIN){
            printf("Type is main. Global: %d\n", set_global);
            main = input;
            input = input->next;
            if(set_global == 1){
                printf("\t/* Global variables */\n");
                fprintf(f, "\t/* Global variables */\n");
                ENTRY* vars = main->entry->next;
                if (vars != NULL){
                    printEntry(vars);
                }
                printf("\n");
                fprintf(f, "\n");
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
//    free(main);
}

void printProgram(N_PROG *input, int set_global) {
    indentLevel = 0;
    ENTRY* vars = NULL;
    if (input != NULL){
        printIndent();
        printEntry(input->entry); // header
        vars = input->entry->next;
        printf("{\n");
        fprintf(f, "{\n");
        indentLevel+=1;
        if(returnType != _MAIN || set_global != 1){
            if (vars != NULL){
                printEntry(vars);
            }
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
//        free(input->entry);
//        free(input->entry->ext.parList);
        //cleanSymTable(vars);
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

void cleanSymTable(ENTRY *symTab) {
    ENTRY  *new = NULL;
    while(symTab != NULL){
        new = symTab->next;
        free(symTab);
        symTab = new;
    }
}

void run(int set_global) {
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
    //free(ast);
}
