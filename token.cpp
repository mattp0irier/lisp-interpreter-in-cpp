#include <string>
#include <iostream>

using namespace std;

enum TokenType {
    // 1-Character Tokens
    LEFT_PAREN, RIGHT_PAREN,
    PLUS, MINUS, MULTIPLY, DIVIDE,
    EQUAL, LESS_THAN, GREATER_THAN, T,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    IF, WHILE, SET, BEGIN, CONS, CAR, CDR,
    IS_NUMBER, IS_SYMBOL, IS_LIST, IS_NULL,
    PRINT, DEFINE, CALL,

    END_OF_FILE
};

class Token {
    private:
        TokenType type;
        string value;
        int numValue;
        int line;

    public:
        Token(TokenType type, string value, int line) {
            this->type = type;
            this->value = value;
            this->line = line;
        }
        Token(TokenType type, int numValue, int line) {
            this->type = type;
            this->numValue = numValue;
            this->line = line;
        }
        Token(const Token &src){
            this->type = src.getType();
            this->value = src.getVal();
            this->numValue = src.getNumVal();
            this->line = src.getLine();
        }
        Token &operator= (const Token &src) {
            this->type = src.getType();
            this->value = src.getVal();
            this->numValue = src.getNumVal();
            this->line = src.getLine();
        }

        string getVal() const { return value; }
        int getNumVal() const { return numValue; }
        TokenType getType() const { return type; }
        int getLine() const { return line; }

};
