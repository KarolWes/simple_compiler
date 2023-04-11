#include <stdio.h>
#include "parser.h"

enum TokenType token;

int match(enum TokenType expected){
    // matches token saved in memory with the expected one
    // result is 1 if found, -1 if not
    if(token == expected){
        token = yylex();
        return 1;
    }
    else{
        printf("Tokens doesn't match. Expected: %d, got %d\n", expected, token);
        return 0;
    }
}

int relOp(){
    printf("Debug Log: in relOp\n");
    // relative operators
    // 0 if not found, 1 if match is correct
    int result = 0;
    if(match(LT) == 1 || match(LEQ) == 1 ||
            match(GT) == 1 || match(GEQ) == 1||
            match(EQ) == 1 || match(NEQ) == 1){
        result = 1;
    }
    return result;
}

int addOp(){
    printf("Debug Log: in addOp\n");
    int result = 0;
    if(match(PLUS) == 1 || match(MINUS) == 1 || match(OR) == 1){
        result = 1;
    }
    return result;
}

int mulOp(){
    printf("Debug Log: in mulOp\n");
    int result = 0;
    if(match(MULTI) == 1 || match(DIV_SIGN) ||
            match(DIV) == 1 || match(MOD) == 1 ||
            match(AND) == 1)
    {
        result = 1;
    }
    return result;
}

int factor(){
    printf("Debug Log: in factor\n");
    int result = 0;

    if(match(NUM) == 1 || match(FALSE) == 1 || match(TRUE) == 1){
        result = 1;
    }
    else if(match(ID) == 1){
        int tmp = ind();
        switch (tmp) {
            case 1:
                result = 1;
                break;
            case -1:
                printf("Factor: error in index expression\n");
                result = -1;
                break;
            case 0:
                tmp = params();
                switch (tmp) {
                    case 1:
                        result = 1;
                        break;
                    case -1:
                        printf("Factor: error in parameters\n");
                        result = -1;
                        break;
                    case 0:
                        result = 1;
                }
        }
    }else if(match(NOT) == 1 || match(MINUS) == 1){
        int tmp = factor();
        if(tmp == 1) {
            result = 1;
        }else{
            printf("factor lacks second part of negation\n");
            result = -1;
        }
    }else if(match(PARENTH_OPEN)){
        if(expr() == 1){
            if(match(PARENTH_CLOSE) == 1){
                result = 1;
            }else{
                printf("Factor lacks closing parenthesis\n");
                result = -1;
            }
        }else{
            printf("Factor lacks expression inside parenthesis\n");
            result = -1;
        }
    }
    return result;
}

// for all functions output is identical:
// 0 -> not found
// 1 -> found
// -1 -> found, but error while parsing

int term(){
    printf("Debug Log: in term\n");
    int result = factor();
    if(result == 1) {
        while (mulOp() == 1) {
            if (factor() != 1){
                printf("Term doesn't contain second part\n");
                return -1;
            }
        }
        return 1;
    }
    else{
        return 0;
    }
}

int simpleExpr(){
    printf("Debug Log: in simpleExpr\n");
    int result = term();
    if(result == 1) {
        while (addOp() == 1) {
            if (term() != 1){
                printf("Simple Expression doesn't contain second part\n");
                return -1;
            }
        }
        return 1;
    }
    else{
        return 0;
    }
}

int expr(){
    printf("Debug Log: in expr\n");
    int result = simpleExpr();
    if(result == 1) {
        while (relOp() == 1) {
            if (simpleExpr() != 1){
                printf("Expression doesn't contain second part\n");
                return -1;
            }
        }
        return 1;
    }
    else{
        return 0;
    }
}

int exprList(){
    printf("Debug Log: in exprList\n");
    int result = expr();
    if(result == 1) {
        while (match(COMA) == 1) {
            if(expr() != 1){
                printf("Expression list contains a conjunction, but lacks following part.\n");
                return -1;
            }
        }
        return 1;
    }
    else{
        return 0;
    }
}

int whileStmt(){
    printf("Debug Log: in whileStmt\n");
    int result = 0;
    if(match(WHILE) == 1){
        if(expr() == 1){
            if(match(DO) == 1){
                if(statement() == 1){
                    result = 1;
                }
                else{
                    printf("While statement lacks execution statement\n");
                    result = -1;
                }
            }
            else{
                printf("While statement lacks 'DO' keyword\n");
                result = -1;
            }
        }
        else{
            printf("While statement lacks condition expression\n");
            result = -1;
        }
    }
    return result;
}

int ifStmt(){
    printf("Debug Log: in ifStmt\n");
    int result = 0;
    if(match(IF) == 1){
        if(expr() == 1){
            if(match(THEN) == 1){
                if (statement() == 1){
                    if(match(ELSE) == 1){
                        if(statement() == 1){
                            result = 1;
                        }
                        else{
                            printf("If statement lacks execution statement in else\n");
                            result = -1;
                        }
                    }
                    else{
                        result = 1;
                    }
                }else{
                    printf("If statement lacks execution statement\n");
                    result = -1;
                }
            }else{
                printf("If statement lacks 'THEN' keyword\n");
                result = -1;
            }
        }else{
            printf("If statement lacks condition expression\n");
            result = -1;
        }
    }
    return result;
}

int ind(){
    printf("Debug Log: in ind\n");
    int result = 0;
    if(match(BRAC_OPEN) == 1){
        if(expr() == 1){
            if(match(RANGE) == 1){
                if(expr() == 1){
                    result = 1;
                }
                else{
                    printf("Index lacks second expression\n");
                    result = -1;
                }
            }
            if(result == 1){
                if(match(BRAC_CLOSE) == 1){
                    result = 1;
                }else{
                    printf("Index lacks closing bracket\n");
                    result = -1;
                }
            }
        }else{
            printf("Index lacks expression\n");
            result = -1;
        }
    }
    return result;
}

int assignStmt(){
    printf("Debug Log: in assignStmt\n");
    int result = 0;
    if(match(ID) == 1){
        if (ind() != -1){
            if(match(ASSIGN) == 1){
                if(expr() == 1){
                    result = 1;
                }else{
                    printf("Assignment lacks expression\n");
                    result = -1;
                }
            }else{
                printf("Assignment lacks assignment sign\n");
                result = -1;
            }
        }else{
            printf("Index returns error\n");
            result = -1;
        }
    }
    return result;
}

int params(){
    printf("Debug Log: in params\n");
    int result = 0;
    if(match(PARENTH_OPEN) == 1){
        if(exprList() == 1){
            if(match(PARENTH_CLOSE) == 1){
                result = 1;
            }else{
                printf("Missing closing parenthesis\n");
                result = -1;
            }
        }else{
            printf("Parameter lacks expressions\n");
            result = -1;
        }
    }
    return result;
}

int procCall(){
    printf("Debug Log: in procCall\n");
    int result = 0;
    if (match(ID) == 1){
        if(params() == -1){
            printf("Procedure call: error in parameters\n");
            result = -1;
        }
        else{
            result = 1;
        }
    }
    return result;
}

int statement(){
    printf("Debug Log: in statement\n");
    int tmp = procCall();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("Statement: error in procedure call\n");
            return -1;
    }
    tmp = assignStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("Statement: error in procedure call\n");
            return -1;
    }
    tmp = compStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("Statement: error in computation\n");
            return -1;
    }
    tmp = ifStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("Statement: error in if\n");
            return -1;
    }
    tmp = whileStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("Statement: error in while\n");
            return -1;
    }
    return 0;
}

int stmtList(){
    printf("Debug Log: in stmtList\n");
    int tmp = statement();
    if(tmp == 1){
        while(match(COLON)){
            if(statement() != 1){
                printf("Statement list: error in statement\n");
                return -1;
            }
        }
        return 1;
    }
    else{
        return tmp;
    }
}

int compStmt(){
    printf("Debug Log: in compStmt\n");
    int result = 0;
    if(match(BEG) == 1){
        if(stmtList() == 1){
            if(match(END) == 1){
                result = 1;
            }else{
                printf("Computation statement lacks END keyword\n");
                result = -1;
            }
        }else{
            printf("Computation statement lacks statements list\n");
            result = -1;
        }
    }
    return result;
}

int parList(){
    printf("Debug Log: in parList\n");
    int result = 0;
    if(identListType() == 1){
        while(match(COLON) == 1){
            if(identListType() != 1){
                printf("Parameter list: error in identifier list type \n");
                return  -1;
            }
        }
        result = 1;
    }
    return result;
}

int args(){
    printf("Debug Log: in args\n");
    int result = 1;
    if(match(PARENTH_OPEN) == 1){
        if(parList()==1){
            if(match(PARENTH_CLOSE) == 1){
                result = 1;
            }else{
                printf("Arguments lack closing parenthesis\n");
                result = -1;
            }
        }else{
            printf("Arguments lack parameters list\n");
            result = -1;
        }
    }
    return result;
}

int subProgHead(){
    printf("Debug Log: in subProgHead\n");
    int f;
    int result = 0;
    if(match(FUNCTION) == 1){
        f = 1;
    }
    else if(match(PROCEDURE) == 1){
        f = 0;
    }
    else{
        return result;
    }
    if(match(ID) == 1){
        if(args() == 1){
            if(f == 1){
                if(match(COLON) == 1){
                    if(type() == 1){
                        result = 1;
                    }else{
                        printf("Subprogram header lacks type\n");
                        result = -1;
                    }
                }else{
                    printf("Subprogram header lacks colon\n");
                    result = -1;
                }
            }
            if(result == 1){
                result = match(SEMICOLON);
            }
        }else{
            printf("Subprogram header lacks arguments\n");
            result = -1;
        }
    }else{
        printf("Subprogram header lacks identifier\n");
        result = -1;
    }
    return result;
}

int subProgList(){
    printf("Debug Log: in subProgList\n");
    int result = 1;
    int test = subProgHead();
    while(test == 1){
        if(varDec() == 1){
            if(compStmt() == 1){
                if(match(SEMICOLON) == 1){
                    test = subProgHead();
                }else{
                    printf("Subprogram list lacks semicolon\n");
                    result = -1;
                }
            }else{
                printf("Subprogram list lacks computation statement\n");
                result = -1;
            }
        }else{
            printf("Subprogram list: error in variable declaration\n");
            result = -1;
        }
    }
    if(test == -1){
        printf("Subprogram list: error in subprogram header\n");
        result = -1;
    }
    return result;
}

int simpleType(){
    printf("Debug Log: in simpleType\n");
    if (match(INTEGER) == 1 || match(REAL) == 1 || match(BOOLEAN) == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int type(){
    printf("Debug Log: in type\n");
    int tmp = simpleType();
    int result = 0;
    if(tmp == 1){
        result = 1;
    }
    else if (tmp == 0){
        if(match(ARRAY) == 1){
            if(match(BRAC_OPEN) == 1){
                if(match(NUM) == 1){
                    if(match(RANGE) == 1){
                        if(match(NUM) == 1){
                            if(match(BRAC_CLOSE) == 1){
                                if(match(OF) == 1){
                                    if(simpleType() == 1){
                                        result = 1;
                                    }else{
                                        printf("Type lacks simple type\n");
                                        result = -1;
                                    }
                                }else{
                                    printf("Type lacks OF keyword\n");
                                    result = -1;
                                }
                            }else{
                                printf("Type lacks closing bracket\n");
                                result = -1;
                            }
                        }else{
                            printf("Type lacks second number\n");
                            result = -1;
                        }
                    }else{
                        printf("Type lacks range operator\n");
                        result = -1;
                    }
                }else{
                    printf("Type lacks first number\n");
                    result = -1;
                }
            }else{
                printf("Type lacks opening bracket\n");
                result = -1;
            }
        }
    }
    else{
        printf("Type: error in simple type\n");
        result = -1;
    }
    return result;
}

int identList(){
    printf("Debug Log: in identList\n");
    int result = 0;
    if(match(ID) == 1){
        while(match(COMA) == 1){
            if(match(ID) != 1){
                printf("Identifier list lacks following identifier\n");
                result = -1;
            }
        }
        if(result == 0){
            result = 1;
        }
    }
    return result;
}

int identListType(){
    printf("Debug Log: in identListType\n");
    int result = 0;
    if(identList() == 1){
        if(match(COLON) == 1){
            if(type() == 1){
                result = 1;
            }else{
                printf("Identifier list type lacks type\n");
                result = -1;
            }
        }else{
            printf("Identifier list type lacks colon\n");
            result = -1;
        }
    }
    return result;
}

int varDecList(){
    printf("Debug Log: in varDecList\n");
    int result = 0;
    if(parList() == 1){
        if(match(SEMICOLON) == 1){
            result = 1;
        }else{
            printf("Variable declaration list lacks semicolon\n");
            result = -1;
        }
    }
    return result;
}

int varDec(){
    printf("Debug Log: in varDec\n");
    int result = 1;
    if(match(VAR) == 1){
        if(varDecList() == 1){
            result = 1;
        }else{
            printf("Variables declaration lacks variable declaration list\n");
            result = -1;
        }
    }
    return result;
}

int start(){
    token = yylex();
    printf("Debug Log: in start\n");
    int result = -1;
    if(match(PROGRAM) == 1){
        if(match(ID) == 1){
            if(match(SEMICOLON) == 1){
                if(varDec() == 1){
                    if(subProgList() == 1){
                        if (compStmt() == 1){
                            if(match(DOT) == 1){
                                result = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}







