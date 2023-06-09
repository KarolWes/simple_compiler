//
// Created by Karol on 11.05.2023.
//

#ifndef SCANNER_L_PRINTOUT_H
#define SCANNER_L_PRINTOUT_H

#include "ast.h"
#include "parser_yacc.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yyparse();
extern N_PROG *ast;
extern ENTRY* variableLookup(char* name);
extern ENTRY *funLookup(char* name);

_DATA_TYPE mainType;
char* mainDef;
int indentLevel;
int labelCounter;


FILE *f;
FILE *f_asm;

void printIndent();
char *typeToStr(_DATA_TYPE type);
char *operatorToStr(_OPERATOR o);
char *operatorToAsm(_OPERATOR o);
_DATA_TYPE getType(char* id, int fun);

void printEntry(ENTRY *input);
void printVarRef(N_VAR_REF *input);
void printExpr(N_EXPR *input, char* separator);
void printExprInner(N_EXPR *input, char* separator);
void printAssign(N_ASSIGN *input);
void printIf(N_IF *input);
void printWhile(N_WHILE *input);
void printCall(N_CALL *input, char* separator, int ignore_indent);
void printStatement(N_STMT* input, int ignore_marker);
void printProgramBase(N_PROG *input, int set_global);
void printProgram(N_PROG *input, int set_global);

void printOp(N_EXPR *input);
void printArgs(ENTRY *input);

void cleanSymTable(ENTRY* symTab);

void push(int size, char type);
void pop(int size, char type);
void printVariableListAsm(ENTRY *vars);
void printVarAsm(ENTRY *v);

void run(int set_global);

#endif //SCANNER_L_PRINTOUT_H
