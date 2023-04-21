#include <stdio.h>
#include "parser.h"
#include "ffsets.h"


void panic(enum Nonterminal n){
//     printf("Debug Log: entering panic recovery mode...\n");
    while(in_set(token, n, 1,1,1)==0){
        printf("Debug Warning: Skipping: %s\n", translateTT(token));
        token = yylex();
    }
//    printf("Debug Log: finished panic recovery mode\n");
}

void checkInput(enum Nonterminal n, int include_first, int include_follow){
    if(in_set(token, n, include_first,include_follow,0) == 0){
        printf("<Line %d> Warning in %s, token %s is not in",yylineno, translateNT(n), translateTT(token));
        if(include_follow*include_first == 1){
            printf(" sum of sets of nonterminal\n");
        }else{
            if(include_first == 1){
                printf(" first set of nonterminal\n");
            }
            if(include_follow == 1){
                printf(" follow set of nonterminal\n");
            }
        }
        panic(n);
    }
}

int match(enum TokenType expected){
    // matches token saved in memory with the expected one
    // result is 1 if found, 0 if not
    if(token == expected){
        token = yylex();
//        printf("Debug Log: matched: \t\t%s\n", translateTT(token));
        return 1;
    }
    else{
//        printf("Debug Log: Tokens doesn't match. Expected: %s, got %s\n",
//               translateTT(expected), translateTT(token));
        return 0;
    }
}

int relOp(){

//    printf("Debug Log: in relOp\n");
    // relative operators
    // 0 if not found, 1 if match is correct
    int result = 0;
    if(in_set(token, Relop, 1, 0, 0)==1){

        if(match(LT) == 1 || match(LEQ) == 1 ||
           match(GT) == 1 || match(GEQ) == 1 ||
           match(EQ) == 1 || match(NEQ) == 1){
            result = 1;
        }
//    printf("Debug Log: exiting relOp\n");
        checkInput(Relop, 0, 1);
    }

    return result;
}

int addOp(){
//    printf("Debug Log: in addOp\n");
    int result = 0;
    if(in_set(token, Addop, 1, 0, 0) == 1){
        if(match(PLUS) == 1 || match(MINUS) == 1 || match(OR) == 1){
            result = 1;
        }
//    printf("Debug Log: exiting addOp\n");
        checkInput(Addop, 0, 1);
    }
    return result;
}

int mulOp(){
//    printf("Debug Log: in mulOp\n");
    int result = 0;
    if(in_set(token, Mulop, 1, 0, 0) == 1){
//        printf("Debug Log: in mulOp, passed in_set check\n");
        if(match(MULTI) == 1 || match(DIV_SIGN) ||
           match(DIV) == 1 || match(MOD) == 1 ||
           match(AND) == 1)
        {
            result = 1;
        }
        checkInput(Mulop, 0, 1);
    }
//    printf("Debug Log: exiting mulOp\n");
    return result;
}

int factor(){
//    printf("Debug Log: in factor\n");
    int result = 0;
    checkInput(Factor, 1, 0);
    if(in_set(token, Factor, 1, 0, 0) == 1){
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
        checkInput(Factor, 0, 1);
    }
    return result;
}

// for all functions output is identical:
// 0 -> not found
// 1 -> found
// -1 -> found, but error while parsing

int term(){
//    printf("Debug Log: in term\n");
    int result = 0;
    checkInput(Term, 1, 0);
    if(in_set(token, Term, 1, 0 ,0) == 1){
        int tmp = factor();
        if(tmp == 1) {
            while (mulOp() == 1) {
                if (factor() != 1){
                    printf("<Line %d> Term doesn't contain second part\n", yylineno);
                    result = -1;
                    break;
                }
            }
//        printf("Debug Log: exiting term\n");
            if (result != -1){
                result = 1;
            }
        }
        checkInput(Term, 0, 1);
    }
    return result;
}

int simpleExpr(){
//    printf("Debug Log: in simpleExpr\n");
    int result = 0;
    checkInput(SimpleExpr, 1, 0);
    if(in_set(token, SimpleExpr, 1, 0,0) == 1){
        int tmp = term();
        if(tmp == 1) {
            while (addOp() == 1) {
                if (term() != 1){
                    printf("<Line %d> Simple Expression doesn't contain second part\n", yylineno);
                    result = -1;
                    break;
                }
            }
//        printf("Debug Log: exiting simpleExpr\n");
            if(result != -1){
                result = 1;
            }
        }
        checkInput(SimpleExpr, 0, 1);
    }
    return result;
}

int expr(){
//    printf("Debug Log: in expr\n");
    int result = 0;
    checkInput(Expr, 1, 0);
    if(in_set(token, Expr, 1, 0, 0) == 1){
        int tmp = simpleExpr();
        if(tmp == 1) {
            while (relOp() == 1) {
                if (simpleExpr() != 1){
                    printf("<Line %d> Expression doesn't contain second part\n", yylineno);
                    result = -1;
                    break;
                }
            }
//        printf("Debug Log: exiting expr\n");
            if(result != -1){
                result = 1;
            }
        }
        checkInput(Expr, 0, 1);
    }
    return result;
}

int exprList(){
//    printf("Debug Log: in exprList\n");
    int result = 0;
    checkInput(ExprList, 1, 0);
    if(in_set(token, ExprList, 1, 0, 0) == 1){
        int tmp = expr();
        if(tmp == 1) {
            while (match(COMA) == 1) {
                if(expr() != 1){
                    printf("<Line %d> Expression list contains a conjunction, "
                           "but lacks following part.\n", yylineno);
                    result = -1;
                }
            }
            if(result != -1){
                result = 1;
            }
        }
        checkInput(ExprList, 0, 1);
    }
    return result;
}

int whileStmt(){
//    printf("Debug Log: in whileStmt\n");
    int result = 0;
    if(in_set(token, WhileStmt, 1, 0, 0)){
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
        checkInput(WhileStmt, 0, 1);
    }
//    printf("Debug Log: exiting while\n");
    return result;
}

int ifStmt(){
//    printf("Debug Log: in ifStmt\n");
    int result = 0;
    if(in_set(token, IfStmt, 1, 0, 0) == 1){
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
        checkInput(IfStmt, 0, 1);
    }
//    printf("Debug Log: exiting if\n");
    return result;
}

int ind(){
//    printf("Debug Log: in ind\n");
    int result = 0;
    if(in_set(token, Index, 1, 0, 0) == 1){
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
        checkInput(Index, 0, 1);
    }
//    printf("Debug Log: exiting index\n");
    return result;
}

int assignStmt(){
//    printf("Debug Log: in assignStmt\n");
    int result = 0;
    checkInput(AssignStmt, 1, 0);
    if(in_set(token, AssignStmt, 1, 0, 0) == 1){
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
        checkInput(AssignStmt, 0, 1);
    }
//    printf("Debug Log: exiting assignment\n");
    return result;
}

int params(){
//    printf("Debug Log: in params\n");
    int result = 0;
    if(in_set(token, Params, 1, 0, 0) == 1){
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
        checkInput(Params, 0, 1);
    }
//    printf("Debug Log: exiting params\n");
    return result;
}

int procOrAssign() {
//    printf("Debug Log: in procedure call or assignment\n");
    int result = 0;
    if(in_set(token, ProcOrAssign, 1, 0 ,0) == 1){
        int tmp = params();
        switch (tmp) {
            case 1:
                result = 1;
                break;
            case -1:
                printf("<Line %d> Procedure call or assignment: error in parameters\n", yylineno);
                result = -1;
        }
        if(result == 0){
            tmp = assignStmt();
            switch (tmp) {
                case 1:
                    result = 1;
                    break;
                case -1:
                    printf("<Line %d> Procedure call or assignment: error in assignment\n", yylineno);
                    result = -1;

            }
        }
        checkInput(ProcOrAssign, 0, 1);
    }
    return result;
}

int statement(){
//    printf("Debug Log: in statement\n");
    int result = 0;
    checkInput(Stmt, 1, 0);
    if(in_set(token, Stmt, 1, 0, 0) == 1){

        int tmp;
        if(match(ID) == 1){
            tmp = procOrAssign();
            switch (tmp) {
                case 1:
                    result = 1;
                    break;
                case -1:
                    printf("<Line %d> Statement: error in procedure call or assigment\n", yylineno);
                    result = -1;
            }
        }
        if(result == 0){
            tmp = compStmt();
            switch (tmp) {
                case 1:
                    result = 1;
                    break;
                case -1:
                    printf("<Line %d> Statement: error in computation\n", yylineno);
                    result = -1;
            }
            if(result == 0){
                tmp = ifStmt();
                switch (tmp) {
                    case 1:
                        result = 1;
                        break;
                    case -1:
                        printf("<Line %d> Statement: error in if\n", yylineno);
                        result = -1;
                }
                if(result == 0){
                    tmp = whileStmt();
                    switch (tmp) {
                        case 1:
                            result = 1;
                            break;
                        case -1:
                            printf("<Line %d> Statement: error in while\n",yylineno);
                            result -1;
                    }
                }
            }
        }
        checkInput(Stmt, 0, 1);
    }
    return result;
}

int stmtList(){
//    printf("Debug Log: in stmtList\n");
    int result = 0;
    checkInput(StmtList, 1, 0);
    if(in_set(token, StmtList, 1, 0, 0) == 1){
        int tmp = statement();
        if(tmp == 1){
            while(match(SEMICOLON)){
                if(statement() != 1){
                    printf("<Line %d> Statement list: error in statement\n", yylineno);
                    result = -1;
                    break;
                }
            }
            if(result != -1){
                result = 1;
            }
        }
        checkInput(StmtList, 0, 1);
    }
    return result;
}

int compStmt(){
//    printf("Debug Log: in compStmt\n");
    int result = 0;
    if(in_set(token, CompStmt, 1, 0, 0) == 1){
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
        checkInput(CompStmt, 0, 1);
    }
//    printf("Debug Log: exiting compStmt\n");
    return result;
}

int parList(){
//    printf("Debug Log: in parList\n");
    int result = 0;
    checkInput(ParList, 1, 0);
    if(in_set(token, ParList, 1, 0, 0) == 1){
        if(identListType() == 1){
            while(match(SEMICOLON) == 1){
                if(identListType() != 1){
                    printf("<Line %d> Parameter list: error in identifier list type \n", yylineno);
                    result = -1;
                    break;
                }
            }
            if(result != -1){
                result = 1;
            }
        }
        checkInput(ParList, 0, 1);
    }
//    printf("Debug Log: exiting parList\n");
    return result;
}

int args(){
//    printf("Debug Log: in args\n");
    int result = 1;
    checkInput(Args, 1, 0);
    if(in_set(token, Args, 1, 0, 0) == 1){
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
        checkInput(Args, 0, 1);
    }
//    printf("Debug Log: exiting args\n");
    return result;
}

int subProgHead(){
//    printf("Debug Log: in subProgHead\n");
    int f = -1;
    int result = 0;
    checkInput(SubProgHead, 1, 0);
    if(in_set(token, SubProgHead, 1, 0, 0) == 1){
        if(match(FUNCTION) == 1){
            f = 1;
        }
        else if(match(PROCEDURE) == 1){
            f = 0;
        }
        if(f != -1){
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
        }
        checkInput(SubProgHead, 0, 1);
    }
//    printf("Debug Log: exiting subProgHead with result %d\n", result);
    return result;
}

int subProgList(){
//    printf("Debug Log: in subProgList\n");
    int result = 1;
    checkInput(SubProgList, 1, 0);
    if(in_set(token, SubProgList, 1, 0, 0) == 1){
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
        checkInput(SubProgList, 0, 1);
    }
//    printf("Debug Log: exiting subProgList\n");
    return result;
}

int simpleType(){
//    printf("Debug Log: in simpleType\n");
    int result = 0;
    checkInput(SimpleType, 1, 0);
    if(in_set(token, SimpleType, 1, 0, 0) == 1){
        if (match(INTEGER) == 1 || match(REAL) == 1 || match(BOOLEAN) == 1){
            result = 1;
        }
        checkInput(SimpleType, 0, 1);
    }
//    printf("Debug Log: exiting simpleType\n");
    return result;
}

int type(){
//    printf("Debug Log: in type\n");
    int result = 0;
    int innerCheck = 0;
    checkInput(Type, 1, 0);
    if(in_set(token, Type, 1, 0, 0) == 1){
        if(match(ARRAY) == 1) {
            if (match(BRAC_OPEN) == 1) {
                if (match(NUM) == 1) {
                    if (match(RANGE) == 1) {
                        if (match(NUM) == 1) {
                            if (match(BRAC_CLOSE) == 1) {
                                if (match(OF) == 1) {
                                    innerCheck = 1;
                                }else{
                                    printf("<Line %d> Type lacks OF keyword\n", yylineno);
                                    result = -1;
                                }
                            }
                            else {
                                printf("<Line %d> Type lacks closing bracket\n", yylineno);
                                result = -1;
                            }
                        } else {
                            printf("<Line %d> Type lacks second number\n", yylineno);
                            result = -1;
                        }
                    } else {
                        printf("<Line %d> Type lacks range operator\n", yylineno);
                        result = -1;
                    }
                } else {
                    printf("<Line %d> Type lacks first number\n", yylineno);
                    result = -1;
                }
            } else {
                printf("<Line %d> Type lacks opening bracket\n", yylineno);
                result = -1;
            }
        }
        if(result != -1)
        {
            int tmp = simpleType();
            if(tmp == 1){
                result = 1;
            }
            else if(tmp == 0 && innerCheck == 1){
                printf("<Line %d> Type lacks type definition\n", yylineno);
                result = -1;
            }
            else if(tmp == -1){
                printf("<Line %d> Error in simpleType\n", yylineno);
                result = -1;
            }
        }
        checkInput(Type,0,1);
    }
//    printf("Debug Log: exiting type\n");
    return result;
}

int identList(){
//    printf("Debug Log: in identList\n");
    int result = 0;
    checkInput(IdentList, 1, 0);
    if(in_set(token, IdentList, 1, 0, 0) == 1){
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
        checkInput(IdentList, 0, 1);
    }
//    printf("Debug Log: exiting identList\n");
    return result;
}

int identListType(){
//    printf("Debug Log: in identListType\n");
    int result = 0;
    checkInput(IdentListType, 1, 0);
    if(in_set(token, IdentListType, 1, 0, 0) == 1){
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
        checkInput(IdentListType, 0, 1);
    }
//    printf("Debug Log: exiting identListType\n");
    return result;
}

int varDecList(){
//    printf("Debug Log: in varDecList\n");
    int result = 0;
    checkInput(VarDecList, 1, 0);
    if(in_set(token, VarDecList, 1, 0, 0) == 1){
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
        checkInput(VarDecList, 0, 1);
    }
//    printf("Debug Log: exiting varDecList\n");
    return result;
}

int varDec(){
//    printf("Debug Log: in varDec\n");
    int result = 1;
    checkInput(VarDec, 1, 0);
    if(in_set(token, VarDec, 1, 0, 0) == 1){
        if(match(VAR) == 1){
            if(varDecList() == 1){
                result = 1;
            }else{
                printf("<Line %d> Variables declaration lacks variable declaration list\n", yylineno);
                result = -1;
            }
        }
        checkInput(VarDec, 0, 1);
    }
//    printf("Debug Log: exiting varDec\n");
    return result;
}

int start(){
    token = yylex();
//    printf("Debug Log: in start\n");
    int result = -1;
    checkInput(Start, 1, 0);
    if(in_set(token, Start, 1, 0, 0) == 1){
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
        checkInput(Start, 0, 1);
    }
    return result;
}
