#include <stdio.h>
#include <conio.h>
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
//        printf("Debug Log: Tokens doesn't match. Expected: %d, got %d\n", expected, token);
        return 0;
    }
}

int relOp(){
//    printf("Debug Log: in relOp\n");
    // relative operators
    // 0 if not found, 1 if match is correct
    int result = 0;
    if(match(LT) == 1 || match(LEQ) == 1 ||
            match(GT) == 1 || match(GEQ) == 1||
            match(EQ) == 1 || match(NEQ) == 1){
        result = 1;
    }
//    printf("Debug Log: exiting relOp\n");
    return result;
}

int addOp(){
//    printf("Debug Log: in addOp\n");
    int result = 0;
    if(match(PLUS) == 1 || match(MINUS) == 1 || match(OR) == 1){
        result = 1;
    }
//    printf("Debug Log: exiting addOp\n");
    return result;
}

int mulOp(){
//    printf("Debug Log: in mulOp\n");
    int result = 0;
    if(match(MULTI) == 1 || match(DIV_SIGN) ||
            match(DIV) == 1 || match(MOD) == 1 ||
            match(AND) == 1)
    {
        result = 1;
    }
//    printf("Debug Log: exiting mulOp\n");
    return result;
}

int factor(){
//    printf("Debug Log: in factor\n");
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
                printf("<Line %d> Factor: error in index expression\n", yylineno);
                result = -1;
                break;
            case 0:
                tmp = params();
                switch (tmp) {
                    case 1:
                        result = 1;
                        break;
                    case -1:
                        printf("<Line %d> Factor: error in parameters\n", yylineno);
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
            printf("<Line %d> Factor lacks second part of negation\n", yylineno);
            result = -1;
        }
    }else if(match(PARENTH_OPEN)){
        if(expr() == 1){
            if(match(PARENTH_CLOSE) == 1){
                result = 1;
            }else{
                printf("<Line %d> Factor lacks closing parenthesis\n", yylineno);
                result = -1;
            }
        }else{
            printf("<Line %d> Factor lacks expression inside parenthesis\n", yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting factor\n");
    return result;
}

// for all functions output is identical:
// 0 -> not found
// 1 -> found
// -1 -> found, but error while parsing

int term(){
//    printf("Debug Log: in term\n");
    int result = factor();
    if(result == 1) {
        while (mulOp() == 1) {
            if (factor() != 1){
                printf("<Line %d> Term doesn't contain second part\n", yylineno);
                return -1;
            }
        }
//        printf("Debug Log: exiting term\n");
        return 1;
    }
    else{
//        printf("Debug Log: exiting term\n");
        return 0;
    }
}

int simpleExpr(){
//    printf("Debug Log: in simpleExpr\n");
    int result = term();
    if(result == 1) {
        while (addOp() == 1) {
            if (term() != 1){
                printf("<Line %d> Simple Expression doesn't contain second part\n", yylineno);
                return -1;
            }
        }
//        printf("Debug Log: exiting simpleExpr\n");
        return 1;
    }
    else{
//        printf("Debug Log: exiting simpleExpr\n");
        return 0;
    }
}

int expr(){
//    printf("Debug Log: in expr\n");
    int result = simpleExpr();
    if(result == 1) {
        while (relOp() == 1) {
            if (simpleExpr() != 1){
                printf("<Line %d> Expression doesn't contain second part\n", yylineno);
                return -1;
            }
        }
//        printf("Debug Log: exiting expr\n");
        return 1;
    }
    else{
//        printf("Debug Log: exiting expr\n");
        return 0;
    }
}

int exprList(){
//    printf("Debug Log: in exprList\n");
    int result = expr();
    if(result == 1) {
        while (match(COMA) == 1) {
            if(expr() != 1){
                printf("<Line %d> Expression list contains a conjunction, "
                       "but lacks following part.\n", yylineno);
                return -1;
            }
        }
//        printf("Debug Log: exiting exprList\n");
        return 1;
    }
    else{
//        printf("Debug Log: exiting exprList\n");
        return 0;
    }
}

int whileStmt(){
//    printf("Debug Log: in whileStmt\n");
    int result = 0;
    if(match(WHILE) == 1){
        if(expr() == 1){
            if(match(DO) == 1){
                if(statement() == 1){
                    result = 1;
                }
                else{
                    printf("<Line %d> While statement lacks execution statement\n", yylineno);
                    result = -1;
                }
            }
            else{
                printf("<Line %d> While statement lacks 'DO' keyword\n", yylineno);
                result = -1;
            }
        }
        else{
            printf("<Line %d> While statement lacks condition expression\n", yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting while\n");
    return result;
}

int ifStmt(){
//    printf("Debug Log: in ifStmt\n");
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
                            printf("<Line %d> If statement lacks execution statement in else\n", yylineno);
                            result = -1;
                        }
                    }
                    else{
                        result = 1;
                    }
                }else{
                    printf("<Line %d> If statement lacks execution statement\n", yylineno);
                    result = -1;
                }
            }else{
                printf("<Line %d> If statement lacks 'THEN' keyword\n", yylineno);
                result = -1;
            }
        }else{
            printf("<Line %d> If statement lacks condition expression\n", yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting if\n");
    return result;
}

int ind(){
//    printf("Debug Log: in ind\n");
    int result = 0;
    if(match(BRAC_OPEN) == 1){
        if(expr() == 1){
            if(match(RANGE) == 1){
                if(expr() == 1){
                    result = 1;
                }
                else{
                    printf("<Line %d> Index lacks second expression\n", yylineno);
                    result = -1;
                }
            }
            if(result != -1){
                if(match(BRAC_CLOSE) == 1){
                    result = 1;
                }else{
                    printf("<Line %d> Index lacks closing bracket\n", yylineno);
                    result = -1;
                }
            }
        }else{
            printf("<Line %d> Index lacks expression\n", yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting index\n");
    return result;
}

int assignStmt(){
//    printf("Debug Log: in assignStmt\n");
    int result = 0;
    if (ind() != -1){
        if(match(ASSIGN) == 1){
            if(expr() == 1){
                result = 1;
            }else{
                printf("<Line %d> Assignment lacks expression\n", yylineno);
                result = -1;
            }
        }else{
            printf("<Line %d> Assignment lacks assignment sign\n", yylineno);
            result = -1;
        }
    }else{
        printf("<Line %d> Index returns error\n", yylineno);
        result = -1;
    }
//    printf("Debug Log: exiting assignment\n");
    return result;
}

int params(){
//    printf("Debug Log: in params\n");
    int result = 0;
    if(match(PARENTH_OPEN) == 1){
        if(exprList() == -1){
            printf("<Line %d> Parameter: error in expressions\n", yylineno);
            result = -1;
        }else{
            if(match(PARENTH_CLOSE) == 1){
                result = 1;
            }else{
                printf("<Line %d> Missing closing parenthesis\n", yylineno);
                result = -1;
            }
        }
    }
//    printf("Debug Log: exiting params\n");
    return result;
}

int procCall(){
//    printf("Debug Log: in procCall\n");
    int result = 0;
    if (match(ID) == 1){
        if(params() == -1){
            printf("<Line %d> Procedure call: error in parameters\n", yylineno);
            result = -1;
        }
        else{
            result = 1;
        }
    }
//    printf("Debug Log: exiting procCall\n");
    return result;
}

int procOrAssign() {
//    printf("Debug Log: in procedure call or assignment\n");
    int tmp = params();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("<Line %d> Procedure call or assignment: error in parameters\n", yylineno);
            return -1;
    }
    tmp = assignStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("<Line %d> Procedure call or assignment: error in assignment\n", yylineno);
            return -1;
        default:
            return 0;
    }
}

int statement(){
//    printf("Debug Log: in statement\n");
    int tmp;
    if(match(ID) == 1){
        tmp = procOrAssign();
        switch (tmp) {
            case 1:
                return 1;
            case -1:
                printf("<Line %d> Statement: error in procedure call or assigment\n", yylineno);
                return -1;
        }
    }
    tmp = compStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("<Line %d> Statement: error in computation\n", yylineno);
            return -1;
    }
    tmp = ifStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("<Line %d> Statement: error in if\n", yylineno);
            return -1;
    }
    tmp = whileStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            printf("<Line %d> Statement: error in while\n",yylineno);
            return -1;
    }
    return 0;
}

int stmtList(){
//    printf("Debug Log: in stmtList\n");
    int tmp = statement();
    if(tmp == 1){
        while(match(SEMICOLON)){
            if(statement() != 1){
                printf("<Line %d> Statement list: error in statement\n", yylineno);
                return -1;
            }
        }
//        printf("Debug Log: exiting stmtList\n");
        return 1;
    }
    else{
//        printf("Debug Log: exiting stmtList\n");
        return tmp;
    }
}

int compStmt(){
//    printf("Debug Log: in compStmt\n");
    int result = 0;
    if(match(BEG) == 1){
        if(stmtList() == 1){
            if(match(END) == 1){
                result = 1;
            }else{
                printf("<Line %d> Computation statement lacks END keyword\n", yylineno);
                result = -1;
            }
        }else{
            printf("<Line %d> Computation statement lacks statements list\n", yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting compStmt\n");
    return result;
}

int parList(){
//    printf("Debug Log: in parList\n");
    int result = 0;
    if(identListType() == 1){
        while(match(SEMICOLON) == 1){
            if(identListType() != 1){
                printf("<Line %d> Parameter list: error in identifier list type \n", yylineno);
                return  -1;
            }
        }
        result = 1;
    }
//    printf("Debug Log: exiting parList\n");
    return result;
}

int args(){
//    printf("Debug Log: in args\n");
    int result = 1;
    if(match(PARENTH_OPEN) == 1){
        if(parList()==1){
            if(match(PARENTH_CLOSE) == 1){
                result = 1;
            }else{
                printf("<Line %d> Arguments lack closing parenthesis\n", yylineno);
                result = -1;
            }
        }else{
            printf("<Line %d> Arguments lack parameters list\n",yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting args\n");
    return result;
}

int subProgHead(){
//    printf("Debug Log: in subProgHead\n");
    int f;
    int result = 0;
    if(match(FUNCTION) == 1){
        f = 1;
    }
    else if(match(PROCEDURE) == 1){
        f = 0;
    }
    else{
//        printf("Debug Log: exiting subProgHead with state 0\n");
        return result;
    }
    if(match(ID) == 1){
//        printf("Debug Log: Func/Proc: Id matched\n");
        if(args() == 1){
//            printf("Debug Log: Func/Proc: args matched\n");
            if(f == 1){
//                printf("Debug Log: Func: proceeding\n");
                if(match(COLON) == 1){
//                    printf("Debug Log: Func: colon matched\n");
                    if(type() == 1){
//                        printf("Debug Log: Func: return type matched\n");
                        result = 1;
                    }else{
                        printf("<Line %d> Subprogram header lacks type\n", yylineno);
                        result = -1;
                    }
                }else{
                    printf("<Line %d> Subprogram header lacks colon\n", yylineno);
                    result = -1;
                }
            }
            if(result != -1){
                result = match(SEMICOLON);
            }
        }else{
            printf("<Line %d> Subprogram header lacks arguments\n", yylineno);
            result = -1;
        }
    }else{
        printf("<Line %d> Subprogram header lacks identifier\n", yylineno);
        result = -1;
    }
//    printf("Debug Log: exiting subProgHead with result %d\n", result);
    return result;
}

int subProgList(){
//    printf("Debug Log: in subProgList\n");
    int result = 1;
    int test = subProgHead();
    while(test == 1){
        if(varDec() == 1){
            if(compStmt() == 1){
                if(match(SEMICOLON) == 1){
                    test = subProgHead();
                }else{
                    printf("<Line %d> Subprogram list lacks semicolon\n", yylineno);
                    test = -1;
                    result = -1;
                }
            }else{
                printf("<Line %d> Subprogram list lacks computation statement\n",yylineno);
                test = -1;
                result = -1;
            }
        }else{
            printf("<Line %d> Subprogram list: error in variable declaration\n",yylineno);
            test = -1;
            result = -1;
        }
    }
    if(test == -1){
        printf("<Line %d> Subprogram list: error in subprogram header\n", yylineno);
        result = -1;
    }
//    printf("Debug Log: exiting subProgList\n");
    return result;
}

int simpleType(){
//    printf("Debug Log: in simpleType\n");
    if (match(INTEGER) == 1 || match(REAL) == 1 || match(BOOLEAN) == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int type(){
//    printf("Debug Log: in type\n");
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
                                        printf("<Line %d> Type lacks simple type\n",yylineno);
                                        result = -1;
                                    }
                                }else{
                                    printf("<Line %d> Type lacks OF keyword\n",yylineno);
                                    result = -1;
                                }
                            }else{
                                printf("<Line %d> Type lacks closing bracket\n",yylineno);
                                result = -1;
                            }
                        }else{
                            printf("<Line %d> Type lacks second number\n",yylineno);
                            result = -1;
                        }
                    }else{
                        printf("<Line %d> Type lacks range operator\n",yylineno);
                        result = -1;
                    }
                }else{
                    printf("<Line %d> Type lacks first number\n",yylineno);
                    result = -1;
                }
            }else{
                printf("<Line %d> Type lacks opening bracket\n",yylineno);
                result = -1;
            }
        }
    }
    else{
        printf("<Line %d> Type: error in simple type\n",yylineno);
        result = -1;
    }
//    printf("Debug Log: exiting type\n");
    return result;
}

int identList(){
//    printf("Debug Log: in identList\n");
    int result = 0;
    if(match(ID) == 1){
        while(match(COMA) == 1){
            if(match(ID) != 1){
                printf("<Line %d> Identifier list lacks following identifier\n",yylineno);
                result = -1;
            }
        }
        if(result == 0){
            result = 1;
        }
    }
//    printf("Debug Log: exiting identList\n");
    return result;
}

int identListType(){
//    printf("Debug Log: in identListType\n");
    int result = 0;
    if(identList() == 1){
        if(match(COLON) == 1){
            if(type() == 1){
                result = 1;
            }else{
                printf("<Line %d> Identifier list type lacks type\n", yylineno);
                result = -1;
            }
        }else{
            printf("<Line %d> Identifier list type lacks colon\n",yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting identListType\n");
    return result;
}

int varDecList(){
//    printf("Debug Log: in varDecList\n");
    int result = 0;
    if(identListType() == 1){
        if(match(SEMICOLON) == 1){
            int tmp = identListType();
            while(tmp == 1){
                if(match(SEMICOLON) == 1) {
                    tmp = identListType();
                }else{
                    printf("<Line %d> Variable declaration list lacks semicolon\n", yylineno);
                    result = -1;
                }
            }
            if (result == 0 && tmp == 0){
                result = 1;
            }else if(tmp == -1){
                result = -1;
                printf("<Line %d> Variable declaration list: "
                       "error in following identifier list type\n", yylineno);
            }
        }else{
            printf("<Line %d> Variable declaration list lacks semicolon\n",yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting varDecList\n");
    return result;
}

int varDec(){
//    printf("Debug Log: in varDec\n");
    int result = 1;
    if(match(VAR) == 1){
        if(varDecList() == 1){
            result = 1;
        }else{
            printf("<Line %d> Variables declaration lacks variable declaration list\n", yylineno);
            result = -1;
        }
    }
//    printf("Debug Log: exiting varDec\n");
    return result;
}

int start(){
    token = yylex();
//    printf("Debug Log: in start\n");
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







