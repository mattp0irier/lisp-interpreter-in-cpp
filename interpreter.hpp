// TOKENS
enum TokenType {nil, string, number, leftParen, rightParen};

template <typename T>
class Token {
    TokenType type;
    T value;

    Token(TokenType type, T value) {
        this->type = type;
        this->value = value;
    }
};