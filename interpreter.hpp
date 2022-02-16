// TOKENS
enum TokenType {nil, name, symbol, number};

template <typename T>
class Token {
    TokenType type;
    T value;

    Token(TokenType type, T value) {
        this->type = type;
        this->value = value;
    }
};