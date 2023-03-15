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

%}
comment "{"[^"}"]*"}"
whitespace  [ \t]
digit [0-9]
number "-"?{digit}+("."{digit}+)?(E("-")?{digit}+)?
alpha [A-Za-z_]
alpha_digit {alpha}|{digit}
operators ".."|":="|"<>"|"<="|">="|"["|"]"|"-"|[(),.;:=+*/<>]

%%

{comment} {
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
{number} {
    printf("Line %d contains a number: %s\n", number_of_line, yytext);
    fprintf(f, "%s", yytext);// numbers
}

{alpha}{alpha_digit}* {
    int found = 0;
    int cid = 0;
    while(yytext[cid]){
        yytext[cid] = tolower(yytext[cid]);
        cid ++;
    }
    for(int i = 0; i < keyword_collection_size; i++){

        if(strcmp(keywords[i], yytext) == 0){
            printf("Line %d contains a keyword: %s\n", number_of_line, yytext);
            found = 1;
            int cid = 0;
            while(yytext[cid]){
                fprintf(f, "%c", toupper(yytext[cid]));
                cid ++;
            }
            break;
        }
    }
    if (found == 0){
        printf("Line %d contains identifier: %s\n", number_of_line, yytext);
        fprintf(f, "%s", yytext);
    }
}

{operators} {
    printf("Line %d contains an operator %s\n", number_of_line, yytext);
    fprintf(f, "%s", yytext);
}

{whitespace}+ {
    fprintf(f, " ");
}

. printf("\tError: Unrecognised character %s\n", yytext);

%%

int main(int argc, char **argv) {

    f = fopen("corrected_file.pas", "w");
    yylex();
    fclose(f);
    return 0;
}