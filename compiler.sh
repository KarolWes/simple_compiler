#!/bin/bash
gcc -o parser.o -c parser.c
flex scanner.l
gcc -o scanner.o lex.yy.c parser.o
./scanner.o < "$1"
$SHELL