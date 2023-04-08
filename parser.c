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
        printf("Tokens doesn't match\n");
        return -1;
    }
}

int relOp(){
    // TODO: refactor
    // relative operators
    // 0 if not found, 1 if match is correct
    int result = 0;
    switch (token) {
        case LT:
            result = match(LT);
            break;
        case LEQ:
            result = match(LEQ);
            break;
        case GT:
            result = match(GT);
            break;
        case GEQ:
            result = match(GEQ);
            break;
        case EQ:
            result = match(EQ);
            break;
        case NEQ:
            result = match(NEQ);
            break;
        default:
            result = 0;
    }
    return result;
}

int addOp(){
    // TODO: refactor
    int result = 0;
    switch (token) {
        case PLUS:
            result = match(PLUS);
            break;
        case MINUS:
            result = match(MINUS);
            break;
        case OR:
            result = match(OR);
            break;
        default:
            result = -1;
    }
    return result;
}

int mulOp(){
    // TODO: refactor
    int result = 0;
    switch (token) {
        case MULTI:
            result = match(MULTI);
            break;
        case DIV_SIGN:
            result = match(DIV_SIGN);
            break;
        case DIV:
            result = match(DIV);
            break;
        case MOD:
            result = match(MOD);
            break;
        case AND:
            result = match(AND);
            break;
        default:
            result = -1;
    }
    return result;
}

int factor(){
    // TODO: refactor
    int result = 0;
    switch (token) {
        case NUM:
            result = match(NUM);
            break;
        case FALSE:
            result = match(FALSE);
            break;
        case TRUE:
            result = match(TRUE);
            break;
        case ID:
            result = match(ID);
            if(result == 1 && (ind() == 1 || params() == 1)){
                result = 1;
            }
            else{
                result = -1;
            }
            break;
        case NOT:
            if(match(NOT) == 1)
                result = factor();
            break;
        case MINUS:
            if(match(MINUS))
                result = factor();
            break;
        case PARENTH_OPEN:
            if(match(PARENTH_OPEN) == 1) {
                int tmp = expr();
                if(tmp == 1 && match(PARENTH_CLOSE) == 1){
                    result = 1;
                }
                else{
                    result = -1;
                }
            }
            else{
                result = -1;
            }
            break;
        default:
            result = -1;
    }
    return result;
}

// for all functions output is identical:
// 0 -> not found
// 1 -> found
// -1 -> found, but error while parsing

int term(){
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
    if (match(INTEGER) == 1 || match(REAL) == 1 || match(BOOLEAN) == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int type(){
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







