#include <stdio.h>

enum TokenType{AND=1, ARRAY, BEG, DIV, DO, ELSE, END,
    FUNCTION, IF, MOD, NOT, OF, OR, PROCEDURE,
    PROGRAM,  THEN, TYPE, VAR, WHILE,
    INTEGER, REAL, BOOLEAN,
    TRUE, FALSE,
    COMMENT,
    BRAC_OPEN, BRAC_CLOSE, PARENTH_OPEN, PARENTH_CLOSE,
    COLON, SEMICOLON, COMA, DOT, RANGE, PLUS, MINUS,
    MULTI, DIV_SIGN, NEQ, EQ, LT, GT, LEQ, GEQ, ASSIGN,
    ID=101, NUM=100};

enum TokenType token;

int statement();

int match(enum TokenType expected){
    if(token == expected){
        token = yylex();
        return 1;
    }
    else{
        return -1;
    }
}

int relOp(){
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
            result = -1;
    }
    return result;
}

int addOp(){
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
            if(result == 1 && (index() == 1 || params() == 1)){
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

int term(){
    int result = factor();
    if(result == 1) {
        while (mulOp() == 1) {
            if (factor() != 1){
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
                    result = -1;
                }
            }
            else{
                result = -1;
            }
        }
        else{
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
                    int tmp = match(ELSE);
                    if(tmp == 1){
                        if(statement() == 1){
                            result = 1;
                        }
                        else{return -1;}
                    }
                    else if(tmp == 0){
                        result = 1;
                    }else{result = -1;}
                }else{result = -1;}
            }else{result = -1;}
        }else{result = -1;}
    }
    return result;
}

int index(){
    int result = 0;
    if(match(BRAC_OPEN) == 1){
        if(expr() == 1){
            if(match(RANGE) == 1){
                if(expr() == 1){
                    result = 1;
                }
                else{
                    return -1;
                }
            }
            if(match(BRAC_CLOSE) == 1){
                result = 1;
            }
            else{result = -1;}
        }
        else{result = -1;}
    }
    return result;
}

int assignStmt(){
    int result = 0;
    if(match(ID) == 1){
        if (index() != -1){
            if(match(ASSIGN) == 1){
                if(expr() == 1){
                    result = 1;
                }else{result=-1;}
            }else{result=-1;}
        }else{result=-1;}
    }
    return result;
}

int params(){
    int result = 0;
    if(match(PARENTH_OPEN) == 1){
        if(exprList() == 1){
            if(match(PARENTH_CLOSE) == 1){
                result = 1;
            }
        }else{result=-1;}
    }
    return result;
}

int procCall(){
    int result = 0;
    if (match(ID) == 1){
        if(params() == -1){
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
            return -1;
    }
    tmp = assignStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            return -1;
    }
    tmp = compStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            return -1;
    }
    tmp = ifStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            return -1;
    }
    tmp = whileStmt();
    switch (tmp) {
        case 1:
            return 1;
        case -1:
            return -1;
    }
    return 0;
}

int stmtList(){
    int tmp = statement();
    if(tmp == 1){
        while(match(COLON)){
            if(statement() != 1){
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
            }else{result = -1;}
        }else{result = -1;}
    }
    return result;
}

int start(){
}







