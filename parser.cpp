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
                    if(!(checkMath(index))) return false;
                    break;
                case T:
                    if(!(checkTrue(index))) return false;
                    break;

                // Keywords
                case IF:
                    if(!(checkIf(index))) return false;
                    break;
                case WHILE:
                    if(!(checkWhile(index))) return false;
                    break;
                case SET:
                    if(!(checkSet(index))) return false;
                    break;
                case BEGIN:
                    if(!(checkBegin(index))) return false;
                    break;
                case CONS:
                    if(!(checkCons(index))) return false;
                    break;
                case CAR:
                case CDR:
                    if(!(checkCarCdr(index))) return false;
                    break;
                case IS_NUMBER
                case IS_SYMBOL:
                case IS_LIST:
                case IS_NULL:
                    if(!(checkIs(index))) return false;
                    break;
                case PRINT:
                    if(!(checkPrint(index))) return false;
                    break;
                case DEFINE:
                    if(!(checkDefine(index))) return false;
                    break;
                
                case END_OF_FILE:
                    return true;
                    break;

                case IDENTIFIER:
                    if(!(checkIdent(index))) return false;
                    break;

                default:
                    cout << "Error: unexpected char at line " << tokens.at(index).getLine() << endl;
                    break;
            }
        }


};