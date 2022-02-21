#include <vector>
#include "token.cpp"

enum exprType {
    LITERAL, UNARY, BINARY, GROUPING
};

class Expression {
    public:
    Expression(){
        op = Token(END_OF_FILE, -1, -1);
    }
    Expression(Token token) {
        TokenType type = token.getType();
        op = token;
        switch(type){

            // BINARY OPERATORS
            case PLUS:
            case MINUS:
            case MULTIPLY:
            case DIVIDE:
            case EQUAL:
            case LESS_THAN:
            case GREATER_THAN:
            case CONS:
                this->type = BINARY;
                break;

            //case IDENTIFIER:
            case INTEGER:
            case STRING:
            case FLOAT:
            case T:
                this->type = LITERAL;
                break;
            
            case CAR:
            case CDR:
            case IS_NUMBER:
            case IS_SYMBOL:
            case IS_LIST:
            case IS_NULL:
                this->type = UNARY;
                break;

            case LEFT_PAREN:
                this->type = GROUPING;
                break;

            default:
                cout << "Expression type error" << endl;
                break;
        }
    }
    
    void setLeft(Expression *expr) { left = expr; }
    void setRight(Expression *expr) { right = expr; }
    exprType getType() { return type; }

    void print() {
        cout << op.getType() << " ";
        if (left) left->print();
        if (right) right->print();
    }
    
    private:
        Expression *left;
        Expression *right;
        Token op;
        exprType type;

};

Expression *toExprTree(vector<Token> tokens, int index) {
    Expression *node = new Expression(tokens[index]);
    switch(node->getType()) {
        case GROUPING:
            delete node;
            return toExprTree(tokens, index + 1);
            break;
        case LITERAL:
            node->setLeft(NULL);
            node->setRight(NULL);
            break;
        case UNARY:
            node->setRight(NULL);
            node->setLeft(toExprTree(tokens, index + 1));
            break;
        case BINARY:
            node->setLeft(toExprTree(tokens, index + 1));
            node->setRight(toExprTree(tokens, index + 2));
            break;
        default:
            cout << "Unexpected Behavior" << endl;
            break;
    }
    return node;
}