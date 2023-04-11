gcc -o parser.o -c parser.c
flex scanner.l
gcc -o scanner.o lex.yy.c parser.o
./scanner.o < data/sample.pas
$SHELL