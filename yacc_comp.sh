#!/bin/bash
yacc -d parser_yacc.y
flex scanner_yacc.l
gcc -o bin/main.o main.c parser_yacc.tab.c lex.yy.c printout.c parser_functions.c
./bin/main.o "$2" < "$1"
$SHELL