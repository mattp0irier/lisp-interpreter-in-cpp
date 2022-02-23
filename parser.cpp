#include <string>
#include <vector>
#include <iostream>
#include "token.cpp"
#include "expression2.cpp"

using namespace std;

class Parser {
    private:
        vector<Token> tokens;
        int index;

        bool check(TokenType type) {
            if (isAtEnd()) return false;
            return peek().getType() == type;
        }

        Token advance() {
            if (!isAtEnd())
                index++;
            return previous();
        }
        
        bool isAtEnd() {
            return peek().getType() == EOF;
        }

        Token peek() {
            return tokens.at(index);
        }

        Token previous() {
            return tokens.at(index - 1);
        }

        Expr expression() {
            return comparison();
        }

        Expr comparison() {
            Expr expr = term();

            while (match(GREATER_THAN, LESS_THAN, EQUAL)) {
                Token op = previous();
                Expr right = term();
                expr = Binary(expr, op, right);
            }

            return expr;
        }

        Expr term() {
            Expr expr = factor();

            while (match(MINUS, PLUS)) {
                Token op = previous();
                Expr right = factor();
                expr = Binary(expr, op, right);
            }

            return expr;
        }

        Expr factor() {
            Expr expr = unary();

            while (match(DIVIDE, MULTIPLY)) {
                Token op = previous();
                Expr right = unary();
                expr = Binary(expr, op, right);
            }

            return expr;
        }

        Expr unary() {
            if (match(MINUS)) {
            Token op = previous();
            Expr right = unary();
            return Unary(op, right);
            }

            return primary();
        }

        Expr primary() {
            if (match(T)) return Literal<bool>(true);
            if (match(NIL)) return Literal<bool>(false);

            if (match(INTEGER)) {
                return Literal<int>(previous().getIntVal());
            }
            else if(match(FLOAT)) {
                return Literal<float>(previous().getFloatVal());
            }
            else if(match(STRING)) {
                return Literal<string>(previous().getVal());
            }

            if (match(LEFT_PAREN)) {
                Expr expr = expression();
                consume(RIGHT_PAREN);
                return Grouping(expr);
            }
        }

        bool match(TokenType types...) {
            va_list args;
            va_start(args, types);
            if (check(types)) {
                advance();
                return true;
            }
            va_arg(args, TokenType);
            va_end(args);
            return false;
        }

        Token consume(TokenType type) {
            if (check(type)) return advance();
        }

    public:
        Parser(vector<Token> tokens) {
            this->tokens = tokens;
            index = 0;
        }

        Expr parse() {
            return expression();
        }
        
        // bool checkStatement() {
        //     switch(tokens.at(index).getType()) {
        //         // Expressions
        //         case PLUS:
        //         case MINUS:
        //         case MULTIPLY:
        //         case DIVIDE:
        //         case EQUAL:
        //         case LESS_THAN:
        //         case GREATER_THAN:
        //             if(!(checkExpression(index))) return false;
        //             break;
        //         case T:
        //             if(!(checkTrue())) return false;
        //             break;

        //         // Statements
        //         case IF:
        //             if(!(checkIf())) return false;
        //             break;
        //         case WHILE:
        //             if(!(checkWhile())) return false;
        //             break;
        //         case SET:
        //             if(!(checkSet())) return false;
        //             break;
        //         case BEGIN:
        //             if(!(checkBegin())) return false;
        //             break;
        //         case CONS:
        //             if(!(checkCons())) return false;
        //             break;
        //         case CAR:
        //         case CDR:
        //             if(!(checkCarCdr())) return false;
        //             break;
        //         case IS_NUMBER:
        //         case IS_SYMBOL:
        //         case IS_LIST:
        //         case IS_NULL:
        //             if(!(checkIs())) return false;
        //             break;
        //         case PRINT:
        //             if(!(checkPrint())) return false;
        //             break;
        //         case DEFINE:
        //             if(!(checkDefine())) return false;
        //             break;
                
        //         case END_OF_FILE:
        //             return true;
        //             break;

        //         case IDENTIFIER:
        //             if(!(checkIdent())) return false;
        //             break;

        //         default:
        //             cout << "Error: unexpected char at line " << tokens.at(index).getLine() << endl;
        //             break;
        //     }
        // }


};