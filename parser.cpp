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
            cout << "In expression with token" << peek().getVal() << endl;
            Expr expr = comparison();
            cout << "collapsed!" << endl;
            return expr;
        }

        Expr comparison() {
            cout << "In comparison with token" << peek().getVal() << endl;
            Expr expr;

            if (match(GREATER_THAN) || match(LESS_THAN) || match(EQUAL)) {
                Token op = previous();
                Expr left = term();
                Expr right = term();
                expr = Binary(left, op, right);
            }
            else {
                expr = term();
            }

            return expr;
        }

        Expr term() {
            cout << "In term with token" << peek().getVal() << endl;
            Expr expr;

            if (match(MINUS) || match(PLUS)) {
                Token op = previous();
                Expr left = factor();
                cout << "factor1 created" << endl;
                Expr right = factor();
                cout << "factor2 created" << endl;
                expr = Binary(left, op, right);
                cout << "Binary created" << endl;
            }
            else {
                expr = factor();
            }

            return expr;
        }

        Expr factor() {
            cout << "In factor with token" << peek().getVal() << endl;
            Expr expr;

            if (match(DIVIDE) || match(MULTIPLY)) {
                Token op = previous();
                Expr left = unary();
                Expr right = unary();
                expr = Binary(left, op, right);
            }
            else {
                expr = unary();
            }

            return expr;
        }

        Expr unary() {
            cout << "In unary with token" << peek().getVal() << endl;
            if (match(MINUS)) {
                Token op = previous();
                Expr right = unary();
                return Unary(op, right);
            }

            return primary();
        }

        Expr primary() {
            cout << "In primary with token" << peek().getVal() << endl;
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
        
        bool match(TokenType type) {
            if (check(type)) {
                advance();
                return true;
            }
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