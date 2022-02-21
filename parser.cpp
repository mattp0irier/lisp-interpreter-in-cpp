#include <string>
#include <vector>
#include <iostream>
#include "token.cpp"

using namespace std;

class Parser {
    private:
        vector<Token> tokens;

    public:
        Parser(vector<Token> tokens) {
            this->tokens = tokens;
        }

        bool parse() {
            
        }

        bool checkStatement(int index) {
            switch(tokens.at(index).getType()) {
                // Single Char Operations
                case PLUS:
                case MINUS:
                case MULTIPLY:
                case DIVIDE:
                case EQUAL:
                case LESS_THAN:
                case GREATER_THAN:
                    checkMath();
                    break;
                case T:
                    checkTrue();
                    break;
                
                // Literals
                IDENTIFIER

                // Keywords
                case IF:
                    checkIf();
                    break;
                case WHILE:
                    checkWhile();
                    break;
                case SET:
                    checkSet();
                    break;
                case BEGIN:
                    checkBegin();
                    break;
                case CONS:
                    checkCons();
                    break;
                case CAR:
                case CDR:
                    checkCarCdr();
                    break;
                case IS_NUMBER
                case IS_SYMBOL:
                case IS_LIST:
                case IS_NULL:
                    checkIs();
                    break;
                case PRINT:
                    checkPrint();
                    break;
                case DEFINE:
                    checkDefine();
                    break;
                
                case END_OF_FILE:
                    return true;
                    break;

                case IDENTIFIER:
                    checkIdent();
                    break;

                default:
                    cout << "Error: unexpected char at line " << tokens.at(index).getLine() << endl;
                    break;
            }
        }


};