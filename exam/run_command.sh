#!/bin/bash
yacc -d parser.y
flex scanner.l
gcc -o exam.o parser.tab.c lex.yy.c exam_functions.c
./exam.o < input.dat
$SHELL