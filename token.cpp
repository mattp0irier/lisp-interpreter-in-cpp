#include <string>

using namespace std;

enum TokenType {
    LEFT_PAREN, RIGHT_PAREN,
    PLUS, MINUS, MULTIPLY, DIVIDE,
    EQUAL, LESS_THAN, GREATER_THAN,



    IDENTIFIER, STRING, NUMBER
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
};