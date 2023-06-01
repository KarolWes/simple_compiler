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

_DATA_TYPE mainType;
char* mainDef;
int indentLevel;


FILE *f;

void printIndent();
char * typeToStr(_DATA_TYPE type);
char *operatorToStr(_OPERATOR o);

void printVarRef(N_VAR_REF *input);
void printIf(N_IF *input, int ignore_indent);
void printWhile(N_WHILE *input, int ignore_indent);
void printCall(N_CALL *input, char* separator, int ignore_indent);
void printAssign(N_ASSIGN *input, int ignore_indent);
void printStatement(N_STMT* input, int ignore_marker, int ignore_indent);
void printExpr(N_EXPR *input, char* separator);
void printEntry(ENTRY *input);
void printProgramBase(N_PROG *input, int set_global);
void printProgram(N_PROG *input, int set_global);
void printArgs(ENTRY *input);
void printOp(N_EXPR *input);
void printExprInner(N_EXPR *input, char* separator);

void cleanSymTable(ENTRY* symTab);



void run(int set_global);

#endif //SCANNER_L_PRINTOUT_H
