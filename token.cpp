#include <string>
#include <iostream>

using namespace std;

enum TokenType {
    // 1-Character Tokens
    LEFT_PAREN, RIGHT_PAREN,
    PLUS, MINUS, MULTIPLY, DIVIDE,
    EQUAL, LESS_THAN, GREATER_THAN, T,

    // Literals
    IDENTIFIER, STRING, INTEGER, FLOAT,

    // Keywords
    IF, WHILE, SET, BEGIN, CONS, CAR, CDR,
    IS_NUMBER, IS_SYMBOL, IS_LIST, IS_NULL,
    PRINT, DEFINE, CALL,

    END_OF_FILE
};

class Token {
    private:
        TokenType type = END_OF_FILE;
        string value = "this should not be the string";
        int intValue = 0;
        double floatValue = 0;
        int line = 0;

    public:
        Token(TokenType type, string value, int line) {
            this->type = type;
            this->value = value;
            this->line = line;
        }
        Token(TokenType type, int intValue, int line) {
            this->type = type;
            this->intValue = intValue;
            this->line = line;
        }
        Token(TokenType type, double floatValue, int line) {
            this->type = type;
            this->floatValue = floatValue;
            this->line = line;
        }
        Token(const Token &src){
            this->type = src.getType();
            this->value = src.getVal();
            this->intValue = src.getIntVal();
            this->floatValue = src.getFloatVal();
            this->line = src.getLine();
        }
        Token &operator= (const Token &src) {
            this->type = src.getType();
            this->value = src.getVal();
            this->intValue = src.getIntVal();
            this->floatValue = src.getFloatVal();
            this->line = src.getLine();
        }

        string getVal() const { return value; }
        int getIntVal() const { return intValue; }
        double getFloatVal() const { return floatValue; }
        TokenType getType() const { return type; }
        int getLine() const { return line; }

};
