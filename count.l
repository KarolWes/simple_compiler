%option noyywrap

%{
/* This example counts and displays the
number of words of an input file */
#include <stdio.h>
int w = 0;
int percent = 0;
int plus = 0;
%}
whitespace  [ \t\n]
operators [+*]
%%

words percent++;
[{+}] plus++;
[^"+"]+

%%

int main(int argc, char **argv) {
yylex();
printf("\nNumber of words: %d\n", w);
printf("\nNumber of word \'words\': %d\n", percent);
printf("\nNumber of ++++ pluses': %d\n", plus);
return 0;
}