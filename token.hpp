#include <string>
#include <iostream>

using namespace std;

#ifndef _TOKEN_
#define _TOKEN_

enum TokenType {
    // 1-Character Tokens
    LEFT_PAREN, RIGHT_PAREN,
    PLUS, MINUS, MULTIPLY, DIVIDE, MOD,
    EQUAL, LESS_THAN, GREATER_THAN, LTE, GTE, T, NIL,

    // Literals
    IDENTIFIER, STRING, INTEGER, FLOAT,

    // Keywords
    IF, WHILE, SET, BEGIN, CONS, CAR, CDR,
    IS_NUMBER, IS_SYMBOL, IS_LIST, IS_NULL, IS_NOT_NULL,
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

        // static Token builtin(String optr) {
        //     Token operation;
        //     switch (optr.toLowerCase()) {
        //         case ";":
        //             return (Token.SEMI);
        //         // this should not occur case "\'": {
        //             return (Token.TIC);
        //         // static final Token TIC = makeBuiltIn("\'");
        //         case "$":
        //             return (Token.EOF);
        //         case "(":
        //             return (Token.OPEN_PAR);
        //         case ")":
        //             return (Token.CLOSE_PAR);
        //         case "+":
        //             return (Token.PLUS);
        //         case "-":
        //             return (Token.MINUS);
        //         case "*":
        //             return (Token.MULT);
        //         case "/":
        //             return (Token.DIV);
        //         case "%":
        //             return (Token.MOD);
        //         case "=":
        //             return (Token.EQ);
        //         case "<=":
        //             return (Token.LTEQ);
        //         case "<":
        //             return (Token.LT);
        //         case ">=":
        //             return (Token.GTEQ);
        //         case ">":
        //             return (Token.GT);
        //         case ("print"):
        //             return (Token.PRINT);
        //         // case ("cond") -> {return foundToken(Token.COND); }
        //         case ("set"):
        //             return (Token.SET);
        //         case ("not"):
        //             return (Token.NOT);
        //         case ("nil?"):
        //             return (Token.NIL_PRED);
        //         case ("atom?"):
        //             return (Token.ATOM_PRED);
        //         case ("list?"): {
        //             return (Token.LIST_PRED);
        //         case ("symbol?"):
        //             return (Token.SYMBOL_PRED);
        //         case ("number?"):
        //             return (Token.NUMBER_PRED);
        //         case ("eq?"):
        //             return (Token.EQ);
        //         case ("quit"):
        //             return (Token.QUIT);
        //         case ("define"):
        //             return (Token.DEFINE);
        //         case ("if"):
        //             return (Token.IF);
        //         case ("while"):
        //             return (Token.WHILE);
        //         case ("begin"):
        //             return (Token.BEGIN);
        //         case ("cons"):
        //             return (Token.CONS);
        //         case ("car"):
        //             return (Token.CAR);
        //         case ("cdr"):
        //             return (Token.CDR);
        //         default:
        //             return NULL;
        //     }
};

void ERROR(string msg, Token token){
    if (token.getLine() == -1){
        cout << "ERROR:: " + msg + " " + to_string(token.getType()) << endl;
    }
    else {
        cout << "ERROR:: Error on line " + to_string(token.getLine()) + " " + msg << endl;
    }
}

void ERROR(string msg) {
    cout << "ERROR:: " + msg << endl;
}

#endif