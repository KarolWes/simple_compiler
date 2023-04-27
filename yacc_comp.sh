yacc -d parser_yacc.y
flex scanner_yacc.y
gcc -o bin/parser_yacc.o parser_yacc.tab.c lex.yy.c
./bin/parser_yacc.o < data/sample.pas
$SHELL