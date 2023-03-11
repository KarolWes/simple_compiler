%option noyywrap

%{
#include <stdio.h>
    typedef enum{PROGRAM, VAR, ARRAY, OF, FUNCTION, BEG,
                  END, WHILE, DO, IF, THEN, ELSE, DIV_WORD, AND, PROCEDURE,
                  COMMENT,
                  BRAC_OPEN, BRAC_CLOSE, PARENTH_OPEN, PARENTH_CLOSE,
                  COLON, SEMICOLON, COMA, DOT, RANGE, PLUS, MINUS,
                  MULTI, DIV_SIGN, NEQ, EQ, LT, GT, LEQ, GEQ, ASSIGN,
                  ID, NUM} TokenType;
    typedef struct {
        TokenType token_val;
        union{
            char *string_val;
            int int_val;
            float float_val;
        } attribute;
    } TokenRecord;
    int number_of_line = 1;
%}

whitespace  [ \t]
available_after_number [ \n\t\r.+-*/();]
available_characters [A-Za-z0-9<>.()[]{},/+=:;*-_ \t\r\n]
numbers [0-9]

%%

"{"[^"}"]*"}" printf("This line %d is a comment\n", number_of_line); // comments

["\n"]+ number_of_line ++; // counting lines

"-"?{numbers}+("."{numbers}+)?(E("-")?{numbers}+)? printf("Line %d contains a number: %s\n", number_of_line, yytext); // numbers

"program" printf("Line %d contains keyword 'program'\n", number_of_line); // keywords
"var" printf("Line %d contains keyword 'var'\n", number_of_line);
"array" printf("Line %d contains keyword 'array'\n", number_of_line);
"of" printf("Line %d contains keyword 'of'\n", number_of_line);
"if" printf("Line %d contains keyword 'if'\n", number_of_line);
"else" printf("Line %d contains keyword 'else'\n", number_of_line);
"then" printf("Line %d contains keyword 'then'\n", number_of_line);
"function" printf("Line %d contains keyword 'function'\n", number_of_line);
"procedure" printf("Line %d contains keyword 'procedure'\n", number_of_line);
"begin" printf("Line %d contains keyword 'begin'\n", number_of_line);
"end" printf("Line %d contains keyword 'end'\n", number_of_line);
"while" printf("Line %d contains keyword 'while'\n", number_of_line);
"do" printf("Line %d contains keyword 'do'\n", number_of_line);
"and" printf("Line %d contains keyword 'and'\n", number_of_line);
"div" printf("Line %d contains keyword 'div'\n", number_of_line);

[A-Za-z_][A-Za-z0-9_]* printf("Line %d contains identifier: %s\n", number_of_line, yytext); // ids
[0-9]+[A-Za-z_][A-Za-z0-9_]* printf("\tError: Line %d: identifier cannot start with number (got %s)\n", number_of_line, yytext);


:= printf("Line %d contains assigment\n", number_of_line); // operators
\<= printf("Line %d contains 'less than or equal' operator\n", number_of_line);
\>= printf("Line %d contains 'grater than or equal' operator\n", number_of_line);
\<\> printf("Line %d contains 'not equal' operator\n", number_of_line);
\.\. printf("Line %d contains 'range' (..) operator\n", number_of_line);
"=" printf("Line %d contains 'equal' operator\n", number_of_line);
">" printf("Line %d contains 'greater than' operator\n", number_of_line);
"<" printf("Line %d contains 'less than' operator\n", number_of_line);
"." printf("Line %d contains 'dot' operator, therefore should be the last in the file\n", number_of_line);
"," printf("Line %d contains 'coma' operator\n", number_of_line);
"+" printf("Line %d contains 'addition' operator\n", number_of_line);
"-" printf("Line %d contains 'subtraction' operator\n", number_of_line);
"*" printf("Line %d contains 'multiplication' operator\n", number_of_line);
"/" printf("Line %d contains 'division' operator\n", number_of_line);
":" printf("Line %d contains 'colon' operator\n", number_of_line);
";" printf("Line %d contains 'semicolon' operator\n", number_of_line);


"(" printf("Line %d contains opening parenthesis\n", number_of_line); // brackets
")" printf("Line %d contains closing parenthesis\n", number_of_line);
"[" printf("Line %d contains opening bracket\n", number_of_line);
"]" printf("Line %d contains closing bracket\n", number_of_line);

{whitespace}+ //pass
[^{A-Za-z0-9<>.()[]{},/+=:;*-_ \t\r\n}]+ printf("\tError: Line %d: unrecognised character(s): %s", number_of_line, yytext);

%%

int main(int argc, char **argv) {
    yylex();

    return 0;
}