#include <string>
#include <vector>
#include <iostream>
#include "token.cpp"

using namespace std;

class Scanner {
    private:
        string input;
        int startIndex;
        int index;
        vector<Token> tokens;
        int currentLine;

        char getNextChar() {
            return input.at(index++);
        }

        void addToken(TokenType type, string value) {
            tokens.push_back(Token(type, value, currentLine));
            // cout << "added string token" << endl;
        }

        void addToken(TokenType type, int value) {
            // cout << "adding token 2" << endl;
            tokens.push_back(Token(type, value, currentLine));
            // cout <<"pushback complete"<<endl;
        }

        void scanToken() {
            char c = getNextChar();
            switch (c) {
            case '(':
                addToken(LEFT_PAREN, string(1, c));
                break;
            case ')':
                addToken(RIGHT_PAREN, string(1, c));
                break;
            case '+':
                addToken(PLUS, string(1, c));
                break;
            case '-':
                addToken(MINUS, string(1, c));
                break;
            case '*':
                addToken(MULTIPLY, string(1, c));
                break;
            case '/':
                addToken(DIVIDE, string(1, c));
                break;
            case '=':
                addToken(EQUAL, string(1, c));
                break;
            case '<':
                addToken(LESS_THAN, string(1, c));
                break;
            case '>':
                addToken(GREATER_THAN, string(1, c));
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
            currentLine = 0;
        }

        vector<Token> scanTokens() {
            // cout << "beginning scan" << endl;
            while(index < input.length()) {
                startIndex = index;
                cout << "scanning token at index " << index << endl;
                scanToken();
            }

            addToken(END_OF_FILE, -1);
            return tokens;
        }

        void printTokens() {
            for (int i=0; i<tokens.size(); i++){
                if(tokens[i].getType() == NUMBER) {
                    cout << "\"" << tokens[i].getNumVal() << "\"";
                }
                else {
                    cout << "\"" << tokens[i].getVal() << "\"";
                }
                if (i+1 != tokens.size()) cout << " ";
            }
            cout << endl;
        }
};