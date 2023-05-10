#!/bin/bash
gcc -o bin/ast.o ast.h
yacc -d parser_yacc.y
flex scanner_yacc.l
gcc -o bin/parser_yacc.o parser_yacc.tab.c lex.yy.c
./bin/parser_yacc.o < "$1"
$SHELL