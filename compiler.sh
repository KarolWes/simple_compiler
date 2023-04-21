#!/bin/bash
gcc -o enumTypes.o -c enumTypes.c
gcc -o ffsets.o -c ffsets.c
gcc -o parser.o -c parser.c
flex scanner.l
gcc -o scanner.o parser.o enumTypes.o ffsets.o lex.yy.c -ggdb
./scanner.o < "$1"
$SHELL