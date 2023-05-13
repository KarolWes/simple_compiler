#!/bin/bash
yacc -d parser_yacc.y
flex scanner_yacc.l
gcc -o bin/main.o parser_yacc.tab.c lex.yy.c printout.c
./bin/main.o < "$1"
$SHELL