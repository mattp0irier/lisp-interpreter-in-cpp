#include <string>
#include <iostream>

using namespace std;

#ifndef _TOKEN_
#define _TOKEN_

enum TokenType {
    // 1-Character Tokens
    LEFT_PAREN, RIGHT_PAREN,
    PLUS, MINUS, MULTIPLY, DIVIDE,
    EQUAL, LESS_THAN, GREATER_THAN, T, NIL,

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
        int arity = 0;

    public:
        Token(){
            type = END_OF_FILE;
            intValue = -1;
            line = -1;
            arity = -1;
        }
        Token(TokenType type, string value, int line, int arity) {
            this->type = type;
            this->value = value;
            this->line = line;
            this->arity = arity;

        }
        Token(TokenType type, int intValue, int line, int arity) {
            this->type = type;
            this->intValue = intValue;
            this->line = line;
            this->arity = arity;

        }
        Token(TokenType type, double floatValue, int line, int arity) {
            this->type = type;
            this->floatValue = floatValue;
            this->line = line;
            this->arity = arity;
        }
        Token(const Token &src){
            this->type = src.getType();
            this->value = src.getVal();
            this->intValue = src.getIntVal();
            this->floatValue = src.getFloatVal();
            this->line = src.getLine();
            this->arity = src.getArity();
        }
        Token &operator= (const Token &src) {
            this->type = src.getType();
            this->value = src.getVal();
            this->intValue = src.getIntVal();
            this->floatValue = src.getFloatVal();
            this->line = src.getLine();
            this->arity = src.getArity();
            return *this;
        }

        string getVal() const { return value; }
        int getIntVal() const { return intValue; }
        double getFloatVal() const { return floatValue; }
        TokenType getType() const { return type; }
        int getLine() const { return line; }
        int getArity() const { return arity; }

};

#endif