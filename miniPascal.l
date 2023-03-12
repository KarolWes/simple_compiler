%option noyywrap

%{
#include <stdio.h>
#include <string.h>
    typedef enum{AND, ARRAY, BEG, CASE, CONST, DIV, DO, DOWNTO, ELSE, END, FIL, FOR,
                 FUNCTION, GOTO, IF, IN, LABEL, MOD, NIL, NOT, OF, OR, PACKED, PROCEDURE,
                 PROGRAM, RECORD, REPEAT, SET, THEN, TO, TYPE, UNTIL, VAR, WHILE, WITH,
                  COMMENT,
                  BRAC_OPEN, BRAC_CLOSE, PARENTH_OPEN, PARENTH_CLOSE,
                  COLON, SEMICOLON, COMA, DOT, RANGE, PLUS, MINUS,
                  MULTI, DIV_SIGN, NEQ, EQ, LT, GT, LEQ, GEQ, ASSIGN,
                  ID, NUM} TokenType;
    const char* keywords[] = {
        "and", "array", "begin", "case", "const", "div",
        "do", "downto", "else", "end", "file", "for",
        "function", "goto", "if", "in", "label", "mod",
        "nil", "not", "of", "or", "packed", "procedure",
        "program", "record", "repeat", "set", "then",
        "to", "type", "until", "var", "while", "with"
    };
    typedef struct {
        TokenType token_val;
        union{
            char *string_val;
            int int_val;
            float float_val;
        } attribute;
    } TokenRecord;
    int number_of_line = 1;
    int keyword_collection_size = 35;
    FILE *f;
    int depth = 0;

%}

whitespace  [ \t]
available_after_number [ \n\t\r.+-*/();]
available_characters [A-Za-z0-9<>.()[]{},/+=:;*-_ \t\r\n]
numbers [0-9]

%%

"{"[^"}"]*"}" {
    printf("This line %d is a comment\n", number_of_line);
    // comments
    fprintf(f, "(*");
    int cid = 0;
    while(yytext[cid]){
        cid++;
    }
    for(int i = 1; i < cid -1; i++){
        fprintf(f, "%c", yytext[i]);
    }
    fprintf(f, "*)");
}

["\n"]+ {
    number_of_line ++; // counting lines
    fprintf(f, "\n");
}

"-"?{numbers}+("."{numbers}+)?(E("-")?{numbers}+)? {
    printf("Line %d contains a number: %s\n", number_of_line, yytext);
    fprintf(f, "%s", yytext);// numbers
}

[A-Za-z_][A-Za-z0-9_]* {
    int found = 0;
    int cid = 0;
    while(yytext[cid]){
        yytext[cid] = tolower(yytext[cid]);
        cid ++;
    }
    printf("\tlowercased\n");
    for(int i = 0; i < keyword_collection_size; i++){

        if(strcmp(keywords[i], yytext) == 0){
            printf("Line %d contains a keyword: %s\n", number_of_line, yytext);
            if(strcmp(yytext, "begin") == 0){
                depth ++;
            }
            if (strcmp(yytext, "end")== 0){
                depth --;
            }
            found = 1;
            int cid = 0;
            while(yytext[cid]){
                fprintf(f, "%c", toupper(yytext[cid]));
                cid ++;
            }
            printf("\tuppercased\n");
            break;
        }
    }
    if (found == 0){
        printf("Line %d contains identifier: %s\n", number_of_line, yytext);
        fprintf(f, "%s", yytext);
    }
}

[0-9]+[A-Za-z_][A-Za-z0-9_]* printf("\tError: Line %d: identifier cannot start with number (got %s)\n", number_of_line, yytext);


:= printf("Line %d contains assigment\n", number_of_line); fprintf(f, "%s", yytext);// operators
\<= printf("Line %d contains 'less than or equal' operator\n", number_of_line); fprintf(f, "%s", yytext);
\>= printf("Line %d contains 'grater than or equal' operator\n", number_of_line); fprintf(f, "%s", yytext);
\<\> printf("Line %d contains 'not equal' operator\n", number_of_line); fprintf(f, "%s", yytext);
\.\. printf("Line %d contains 'range' (..) operator\n", number_of_line); fprintf(f, "%s", yytext);
"=" printf("Line %d contains 'equal' operator\n", number_of_line); fprintf(f, "%s", yytext);
">" printf("Line %d contains 'greater than' operator\n", number_of_line); fprintf(f, "%s", yytext);
"<" printf("Line %d contains 'less than' operator\n", number_of_line); fprintf(f, "%s", yytext);
"." printf("Line %d contains 'dot' operator, therefore should be the last in the file\n", number_of_line); fprintf(f, "%s", yytext);
"," printf("Line %d contains 'coma' operator\n", number_of_line); fprintf(f, "%s", yytext);
"+" printf("Line %d contains 'addition' operator\n", number_of_line); fprintf(f, "%s", yytext);
"-" printf("Line %d contains 'subtraction' operator\n", number_of_line); fprintf(f, "%s", yytext);
"*" printf("Line %d contains 'multiplication' operator\n", number_of_line); fprintf(f, "%s", yytext);
"/" printf("Line %d contains 'division' operator\n", number_of_line); fprintf(f, "%s", yytext);
":" printf("Line %d contains 'colon' operator\n", number_of_line); fprintf(f, "%s", yytext);
";" printf("Line %d contains 'semicolon' operator\n", number_of_line); fprintf(f, "%s", yytext);


"(" printf("Line %d contains opening parenthesis\n", number_of_line); fprintf(f, "%s", yytext); // brackets
")" printf("Line %d contains closing parenthesis\n", number_of_line); fprintf(f, "%s", yytext);
"[" printf("Line %d contains opening bracket\n", number_of_line); fprintf(f, "%s", yytext);
"]" printf("Line %d contains closing bracket\n", number_of_line); fprintf(f, "%s", yytext);

[" "]/[^" "] {
    fprintf(f, " ");
}
[" ""\t"]+ {
    for(int i = 0; i < depth; i++){
        fprintf(f, "\t");
    }
}

%%

int main(int argc, char **argv) {

    f = fopen("corrected_file.pas", "w");
    yylex();
    fclose(f);
    return 0;
}