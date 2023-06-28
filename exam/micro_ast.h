
/* Abstract Syntax Tree (AST) for Mini-Pascal */
typedef enum {PLUS_OP, MINUS_OP, MULTI_OP, DIV_OP} _OPERATOR;


/* 2. Expression */

typedef struct tN_EXPR {
    enum {NUMBER, EXP} type;
    struct tExprOp{
        struct tN_EXPR *expr; /* one or two operands; must not be null! */
        _OPERATOR op; /* operator */
    } operation;
    int value;
    struct tN_EXPR *next; /* in case of expression list */
} N_EXPR;

/* 8. Root node of a program's AST */

N_EXPR *ast;