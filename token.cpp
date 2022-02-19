#include <string>

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
    PRINT, DEFINE, CALL
};

class Token {
    private:
        TokenType type;
        string value;
        int line;

    public:
        Token(TokenType type, string value, int line) {
            this->type = type;
            this->value = value;
            this->line = line;
        }

        string getVal() { return value; }
        TokenType getType() { return type; }
        int getLine() { return line; }

};