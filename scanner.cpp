#include <string>
#include <vector>
#include <iostream>
#include "Token.cpp"

using namespace std;

class Scanner {
    private:
        string input;
        int startIndex;
        int index;
        vector<Token> tokens;
        int currentLine = 0;

        char getNextChar() {
            return input.at(index++);
        }

        void addToken(TokenType type, string value) {
            tokens.push_back(*(new Token(type, value, currentLine)));
        }

        void addToken(TokenType type, int value) {
            tokens.push_back(*(new Token(type, value, currentLine)));
        }

        void scanToken() {
            char c = getNextChar();
            switch (c) {
            case '(':
                addToken(LEFT_PAREN, NULL);
                break;
            case ')':
                addToken(RIGHT_PAREN, NULL);
                break;
            case '+':
                addToken(PLUS, NULL);
                break;
            case '-':
                addToken(MINUS, NULL);
                break;
            case '*':
                addToken(MULTIPLY, NULL);
                break;
            case '/':
                addToken(DIVIDE, NULL);
                break;
            case '=':
                addToken(EQUAL, NULL);
                break;
            case '<':
                addToken(LESS_THAN, NULL);
                break;
            case '>':
                addToken(GREATER_THAN, NULL);
                break;

            // ignore whitespace
            case ' ':
                break;
            case '\r':
                break;
            case '\t':
                break;
            case '\n':
                currentLine++; // increment line counter
                break;
            default:
                if(isdigit(c)) {
                    getNumber();
                }
                else if (isalpha(c)) {
                    getIdentifier();
                }
                else {
                    cout << "Invalid character at line " << currentLine << endl;
                }
                break;
            }
        }

        void getNumber() {
            while (isdigit(index + 1)) {
                getNextChar();
            }
            addToken(NUMBER, stoi(input.substr(startIndex, index)));
        }

        void getIdentifier() {
            while (isalpha(index + 1)) {
                getNextChar();
            }
            addToken(IDENTIFIER, input.substr(startIndex, index));
        }


    public:
        Scanner(string input) {
            this->input = input;
        }

        vector<Token> scanTokens() {
            while() {
                startIndex = index;
                scanToken();
            }

            /// FINISH
        }

        void printTokens() {
            for (int i=0; i<tokens.size(); i++){
                cout << "\"" << tokens[i].getVal() << "\"";
                if (i+1 != tokens.size()) cout << " ";
            }
            cout << endl;
        }
};