//
// Created by Karol on 11.05.2023.
//
#include "printout.h"

/* global variables */

_DATA_TYPE mainType = _INT;
char* mainDef = "int main(int argc, char *argv[])";
int indentLevel = 0;
_DATA_TYPE returnType = 0;
int labelCounter = 0;

/* Utility functions */

/* Function puts on the console and into the file required amount of \t (tabulators)
 * to create clear indentation */
void printIndent() {
    for(int i = 0; i < indentLevel; i++){
        printf("\t");
        fprintf(f, "\t");
    }
}

/* Function takes the enum value of type and returns its string definition */
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

/* Function takes the enum value of operator and returns its string value */
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

char *operatorToAsm(_OPERATOR o) {
    switch (o) {
        case NO_OP:
            return "";
        case EQ_OP:
            return "seq";
        case NEQ_OP:
            return "sne";
        case GT_OP:
            return "sgt";
        case GEQ_OP:
            return "sge";
        case LT_OP:
            return "slt";
        case LEQ_OP:
            return "sle";
        case PLUS_OP:
            return "add";
        case MINUS_OP:
            return "sub";
        case MULTI_OP:
            return "mult";
        case SLASH_OP:
            return "div";
        case DIV_OP:
            return "div";
        case MOD_OP:
            return "div";
        case AND_OP:
            return "andi";
        case OR_OP:
            return "ori";
        case NOT_OP:
            return "nori $t0, $zero";
        default:
            printf("Operator not available %s\n", operatorToStr(o));
            return "";
    }
}

_DATA_TYPE getType(char* id, int fun){
    ENTRY *e;
    if(fun == 1){
        e = funLookup(id);
    }else{
        e = variableLookup(id);
    }
    return e->dataType;
}

/* Print functions */
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
                returnType = input->dataType;
                printf("\n\t/* program: '%s' */\n\n\n", input->id);
                fprintf(f, "\n\t/* program: '%s' */\n\n\n", input->id);
                printf("%s\n", mainDef);
                fprintf(f, "%s\n", mainDef);
                // assembler
                fprintf(f_asm, "# program %s:\n\n", input->id);
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
                // assembler
                fprintf(f_asm, "program_%s:\n\n", input->id);
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
    _DATA_TYPE dt = getType(input->id,0);
    printf("%s", input->id);
    fprintf(f, "%s", input->id);
    if (input->index != NULL) {
        printf("[");
        fprintf(f, "[");
        N_EXPR *i = input->index;
        printExpr(i, "");
        printf("]");
        fprintf(f, "]");
        if(dt == _BOOL || dt == _INT){
            fprintf(f_asm, "\tli $t2, 4 # calculate index offset\n");
        }
        else{
            fprintf(f_asm, "\tli $t2, 8 # calculate index offset\n");
        }
        fprintf(f_asm, "\tmult $t0, $t2\n");
        fprintf(f_asm, "\tmflo $t0\n");
        push(4, 't');
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

/* Printing the expression is separated into two different functions,
 * this one deals with the most of the work
 * parenthesis printing could be probably done better
 * *separator* defines the string used to separate different parts of expression
 * (in general, it is set to , (coma) in case of function calls)*/
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
                if (strcmp(input->description.constant, "true") == 0) { fprintf(f_asm, "1"); }
                else{ fprintf(f_asm, "0"); }
            }
            else{
                num = atof(input->description.constant);
                int tmp = (int)(num * 10000);
                if (tmp % 10000 == 0){
                    tmp  = (int) num;
                    printf("%d", tmp);
                    fprintf(f, "%d", tmp);
                    fprintf(f_asm, "\tli $t0, %d\n", tmp);
                }
                else{
                    printf("%f", num);
                    fprintf(f, "%f", num);
                    fprintf(f_asm, "\tli.s $f0, %f\n", num);
                }
            }
            break;
        case VAR_REF:
            printVarRef(input->description.var_ref);
            //assembler
            _DATA_TYPE dt = getType(input->description.var_ref->id, 0);
            if(input->description.var_ref->index == NULL){
                if (dt == _BOOL || dt == _INT) {
                    fprintf(f_asm, "\tlw $t0, ");
                } else {
                    fprintf(f_asm, "\tlwc1 $f0, ");
                }
                fprintf(f_asm, "%s\n", input->description.var_ref->id);
            }
            else{
                pop(4,'t');
                fprintf(f_asm, "\tla $t4, %s\n",input->description.var_ref->id);
                fprintf(f_asm, "\tadd $t3, $t4, $t1 # set new index in the array\n");
                if(dt == _INT || dt == _BOOL){
                    fprintf(f_asm, "\tsw $t0, 0($t3)\n");
                }
                else{
                    fprintf(f_asm, "\tswc1 $f0, 0($t3)\n");
                }
            }

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

void printAssign(N_ASSIGN *input) {
    if(input == NULL){
        return;
    }
    printIndent();
    printVarRef(input->var_ref);
    printf(" = ");
    fprintf(f, " = ");
    printExpr(input->rhs_expr, "");
    printf(";\n");
    fprintf(f, ";\n");
    //assembler
    _DATA_TYPE  dt = getType(input->var_ref->id, 0);
    if(input->var_ref->index != NULL){
        pop(4,'t');
        fprintf(f_asm, "\tla $t4, %s\n",input->var_ref->id);
        fprintf(f_asm, "\tadd $t3, $t4, $t1 # set new index in the array\n");
        if(dt == _INT || dt == _BOOL){
            fprintf(f_asm, "\tsw $t0, 0($t3)\n");
        }
        else{
            fprintf(f_asm, "\tswc1 $f0, 0($t3)\n");
        }
    }
    else{
        if(dt == _INT || dt == _BOOL){
            fprintf(f_asm, "\tsw $t0, ");
        }
        else{
            fprintf(f_asm, "\tswc1 $f0, ");
        }
        fprintf(f_asm, "%s\n", input->var_ref->id);
    }
}

void printIf(N_IF *input) {
    int currentLabel = labelCounter;
    labelCounter += 1;
    if(input == NULL){
        return;
    }
    printIndent();
    printf("if (");
    fprintf(f, "if (");
    printExpr(input->expr, "");
    printf(")\n");
    fprintf(f, ")\n");
    if(input->else_part != NULL){
        fprintf(f_asm, "\tbeq $t0, $zero, ELSE%d\n", currentLabel);
    }else{
        fprintf(f_asm, "\tbeq $t0, $zero, END%d\n", currentLabel);
    }
    printStatement(input->then_part, 0);
    if(input->else_part != NULL){
        fprintf(f_asm, "\tj END%d\n", currentLabel);
        fprintf(f_asm, "ELSE%d:\n", currentLabel);
        printIndent();
        printf("else\n");
        fprintf(f, "else\n");
        printStatement(input->else_part, 0);
    }
    fprintf(f_asm, "END%d:\n", currentLabel);
}

void printWhile(N_WHILE *input) {
    int currentLabel = labelCounter;
    labelCounter += 1;
    if(input == NULL){
        return;
    }
    printIndent();
    fprintf(f_asm, "WHILE%d:\n", currentLabel);
    printf("while (");
    fprintf(f, "while (");
    printExpr(input->expr, "");
    printf(")\n");
    fprintf(f, ")\n");
    fprintf(f_asm, "\tbeq $t0, $zero, END%d\n", currentLabel);
    printStatement(input->stmt, 0);
    fprintf(f_asm, "\tj WHILE%d\n", currentLabel);
    fprintf(f_asm, "END%d:\n", currentLabel);
}

/* This function deals with printing function calls. It has a special flag
 * *ignore_indent* which does exactly that,
 * used only in cas of calling function as an argument in other function */
void printCall(N_CALL *input, char* separator, int ignore_indent) {
    if(input == NULL){
        return;
    }
    if(ignore_indent == 0) { printIndent(); }
    printf("%s(", input->id);
    fprintf(f, "%s(", input->id);
    printExpr(input->par_list, ", ");
    printf(")%s", separator);
    fprintf(f, ")%s", separator);
}

void printStatement(N_STMT *input, int ignore_marker) {
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
                printAssign(input->node.assign_stmt);
                break;
            case _IF:
                printIf(input->node.if_stmt);
                break;
            case _WHILE:
                printWhile(input->node.while_stmt);
                break;
            case _PROC_CALL:
                printCall(input->node.proc_call, ";\n", 0);
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

/* The main function that is called from runner.
 * It analyses the tree and governs the main structure:
 * - global variables
 * - order of functions
 * - main function
 * *set_global* can be used to turn of global variables and treat all as locals in main. */
void printProgramBase(N_PROG *input, int set_global) {
    N_PROG* main;
    while( input != NULL){
        if(input->entry->dataType == _MAIN){
            printf("Type is main. Global: %d\n", set_global);
            // assembler
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

/* prints internal structure of the function:
 * - header
 * - local variables
 * - body
 * - return */
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
        printVariableListAsm(vars);

        if(returnType != _MAIN || set_global != 1){
            if (vars != NULL){
                printEntry(vars);
            }
        }
        if(returnType != _VOID && returnType != _MAIN){
            printIndent();
            printf("%s %s;\n", typeToStr(returnType), "result");
            fprintf(f, "%s %s;\n", typeToStr(returnType), "result");
            // assembler
            fprintf(f_asm, "\tresult:\t.word\t0\n");
        }
        fprintf(f_asm, "\n\n.text\n");
        if(returnType == _MAIN){
            fprintf(f_asm, ".globl main\n");
            fprintf(f_asm, "main:\n");
        }
        indentLevel-=1;
        printStatement(input->stmt, 1);
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
        fprintf(f_asm, "# done\n\n");
        fprintf(f_asm, "#______________\n");
    }
}

/* specialized function for printing operation chains,
 * because they are linked different from other elements of tree */
void printOp(N_EXPR  *input){
    if(input->description.operation.op == NOT_OP){
        printf("! ");
        fprintf(f, "! ");
        printExprInner(input->description.operation.expr, "");
        if(input->dataType == _BOOL || input->dataType == _INT){
            fprintf(f_asm, "\t%s, $t0\n", operatorToAsm(NOT_OP));
        }else{
            fprintf(f_asm, "\t%s, $f0\n", operatorToAsm(NOT_OP));
        }

//        free(input->description.operation.expr);
    }
    else{
        N_EXPR *e = input->description.operation.expr;
        while(e!= NULL){
            printExprInner(e, "");
            if (e->next != NULL)
            {
                _DATA_TYPE dt = input->dataType;
                printf(" %s ", operatorToStr(input->description.operation.op));
                fprintf(f, " %s ", operatorToStr(input->description.operation.op));
                if(dt == _BOOL || dt == _INT){
                    push(4,'t');
                }else{
                    push(8, 'f');
                }
            }
            else{
                _OPERATOR o = input->description.operation.op;
                char* o_asm = operatorToAsm(o);
                if(o == EQ_OP || o == NEQ_OP || o == LEQ_OP || o == GEQ_OP || o == LT_OP || o == GT_OP ||
                    o == AND_OP || o == OR_OP){
                    if(e->dataType == _BOOL || e->dataType == _INT){
                        pop(4, 't');
                        fprintf(f_asm, "\t%s $t0, $t1, $t0\n", o_asm);
                    }
                    else{
                        pop(8, 'f');
                        fprintf(f_asm, "\t%s $t0, $f1, $f0\n", o_asm);
                    }
                }else if(o == MULTI_OP){
                    if(e->dataType == _BOOL || e->dataType == _INT){
                        pop(4, 't');
                        fprintf(f_asm, "\t%s $t1, $t0\n", o_asm);
                        fprintf(f_asm, "\tmflo $t0\n");
                    }
                    else{
                        pop(8, 'f');
                        fprintf(f_asm, "\tmul.s $f0, $f1, $f0\n");
                    }
                }
                else if(o == DIV_OP || o == SLASH_OP) {
                    if(e->dataType == _BOOL || e->dataType == _INT){
                        pop(4, 't');
                        fprintf(f_asm, "\t%s $t1, $t0\n", o_asm);
                        fprintf(f_asm, "\tmflo $t0\n");
                    }
                    else{
                        pop(8, 'f');
                        fprintf(f_asm, "\tdiv.s $f0, $f1, $f0\n");
                    }

                }
                else if(o == MOD_OP) {
                    pop(4, 't');
                    fprintf(f_asm, "\t%s $t1, $t0\n", o_asm);
                    fprintf(f_asm, "\tmfhi $t0\n");
                }
                else{
                    if(e->dataType == _BOOL || e->dataType == _INT){
                        pop(4, 't');
                        fprintf(f_asm, "\t%s $t0, $t1, $t0\n", o_asm);
                    }
                    else{
                        pop(8, 'f');
                        fprintf(f_asm, "\t%s $f0, $f1, $f0\n", o_asm);
                    }
                }
            }
            N_EXPR *old = e;
            e = e->next;
//            free(old);
        }
    }
//    free(input);
}

/* This function prints the arguments of the function call */
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

/* For the future use */
void cleanSymTable(ENTRY *symTab) {
    ENTRY  *new = NULL;
    while(symTab != NULL){
        new = symTab->next;
        free(symTab);
        symTab = new;
    }
}

/* For assembly printouts */
void push(int size, char type) {
    fprintf(f_asm, "\taddi $sp, $sp, -%d\t# push\n", size);
    if(type == 't'){
        fprintf(f_asm, "\tsw $t0, 0($sp)\n");
    }
    else{
        fprintf(f_asm, "\tswc1 $f0, 0($sp)\n");
    }

}

void pop(int size, char type){
    if(type == 't'){
        fprintf(f_asm, "\tlw $t1, 0($sp)\t# pop\n");
    }
    else{
        fprintf(f_asm, "\tlwc1 $f1, 0($sp)\t# pop\n");
    }
    fprintf(f_asm, "\taddi $sp, $sp, %d\n", size);
}

void printVariableListAsm(ENTRY *vars){
    if(vars != NULL){
        fprintf(f_asm, ".data\n");
    }
    while(vars != NULL){
        printVarAsm(vars);
        vars = vars->next;
    }
}

void printVarAsm(ENTRY *v){
    fprintf(f_asm, "\t%s:\t",v->id);
    if(v->typ == _VAR){
        if(v->dataType == _INT || v->dataType == _BOOL){
            fprintf(f_asm, ".word\t");
            fprintf(f_asm, "0\n");
        }
        else if(v->dataType == _REAL){
            fprintf(f_asm, ".float\t");
            fprintf(f_asm, "0.0\n");
        }
    }else if (v->typ == _ARRAY){
        fprintf(f_asm, ".space\t");
        if(v->dataType == _INT || v->dataType == _BOOL){
            fprintf(f_asm, "%d\t", (v->ext.bounds.high - v->ext.bounds.low + 1) *4);
            fprintf(f_asm, "# array of integers\n");
        }
        else if(v->dataType == _REAL){
            fprintf(f_asm, "%d\t", (v->ext.bounds.high - v->ext.bounds.low + 1) * 8);
            fprintf(f_asm, "# array of floats\n");
        }
    }
}




/* runner function to be executed
 * should also do the memory cleanup */
void run(int set_global) {
    if(set_global != 1 && set_global != 0){
        printf("Unavailable value. Try 1 or 0");
        exit(3);
    }
    f = fopen("new_code.c", "w");
    f_asm = fopen("code.asm", "w");
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
    fclose(f_asm);
    //free(ast);
}
