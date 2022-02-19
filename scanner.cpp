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
                addToken(LEFT_PAREN, c);
                break;
            case ')':
                addToken(RIGHT_PAREN, c);
                break;
            case '+':
                addToken(PLUS, c);
                break;
            case '-':
                addToken(MINUS, c);
                break;
            case '*':
                addToken(MULTIPLY, c);
                break;
            case '/':
                addToken(DIVIDE, c);
                break;
            case '=':
                addToken(EQUAL, c);
                break;
            case '<':
                addToken(LESS_THAN, c);
                break;
            case '>':
                addToken(GREATER_THAN, c);
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
            while(index < input.length()) {
                startIndex = index;
                scanToken();
            }

            addToken(END_OF_FILE, NULL);
            return tokens;
        }

        void printTokens() {
            for (int i=0; i<tokens.size(); i++){
                cout << "\"" << tokens[i].getVal() << "\"";
                if (i+1 != tokens.size()) cout << " ";
            }
            cout << endl;
        }
};