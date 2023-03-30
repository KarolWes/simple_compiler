%option noyywrap

%{
#include <stdio.h>
#include <string.h>
    typedef enum{AND=1, ARRAY, BEG, CASE, CONST, DIV, DO, DOWNTO, ELSE, END, FIL, FOR,
                 FUNCTION, GOTO, IF, IN, LABEL, MOD, NIL, NOT, OF, OR, PACKED, PROCEDURE,
                 PROGRAM, RECORD, REPEAT, SET, THEN, TO, TYPE, UNTIL, VAR, WHILE, WITH,
				 INTEGER, REAL
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
        "to", "type", "until", "var", "while", "with",
		"integer", "real"
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
    int keyword_collection_size = 37;
    FILE *f;
    int comment_count = 0;
    int max_nest = 0;
    int current_nest = 0;

%}
comment "{"[^"}"]*"}"
whitespace  [ \t]
digit [0-9]
number "-"?{digit}+("."{digit}+)?(E("-")?{digit}+)?
alpha [A-Za-z_]
alpha_digit {alpha}|{digit}
operators ".."|":="|"<>"|"<="|">="|"["|"]"|"-"|[(),.;:=+*/<>]

%x NEST

%%

"(*" {
    BEGIN NEST;
    comment_count++;
    current_nest++;
}

<NEST>"(*" {
    current_nest ++;
}
<NEST>"*)" {
    if(current_nest > max_nest){
        max_nest = current_nest;
    }
    current_nest--;
    if(current_nest == 0){
        BEGIN 0;
    }
}

<NEST>.
<NEST><<EOF>> {
    if (current_nest > 0){
        comment_count--;
        printf("\tError: invalid comment\n");
        BEGIN 0;
    }
}

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

    f = fopen("data/corrected_file.pas", "w");
    yylex();
    fclose(f);
    printf("Number of comments: %d\n", comment_count);
    printf("Maximal nest: %d\n", max_nest);
    return 0;
//    <NEST>"\n" {
//        comment_count--;
//        printf("\tError: invalid comment\n");
//        BEGIN 0;
//    }
}