#!/bin/bash
gcc -o bin/enumTypes.o -c enumTypes.c
gcc -o bin/ffsets.o -c ffsets.c
gcc -o bin/parser.o -c parser.c
flex scanner.l
gcc -o bin/scanner.o bin/parser.o bin/enumTypes.o bin/ffsets.o lex.yy.c
./bin/scanner.o < "$1"
$SHELL