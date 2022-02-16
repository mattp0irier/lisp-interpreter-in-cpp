// TOKENS
enum TokenType {nil, string, number, leftParen, rightParen};
enum OperationType {ifOp, whileOp, setOp, beginOp, plusOp, minusOp, multOp, divOp, eqOp, ltOp, gtOp, consOp, carOp, cdrOp, isNumOp, isSymOp, isListOp, isNullOp, printOp, tOp}

template <typename T>
class Token {
    TokenType type;
    T value;

    Token(TokenType type, T value) {
        this->type = type;
        this->value = value;
    }
};