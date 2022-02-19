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
           // Token *newToken = new Token(type, value, currentLine);
           // tokens.push_back(*newToken);
           Token test_token(LEFT_PAREN, "(", 0);
           cout << "adding token 1" << endl;
           tokens.push_back(test_token);
            cout << "added string token" << endl;
        }

        void addToken(TokenType type, int value) {
            cout << "adding token 2" << endl;
            tokens.push_back((Token(type, value, currentLine)));
            cout <<"pushback complete"<<endl;
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
        }

        vector<Token> scanTokens() {
            while(index < input.length()) {
                startIndex = index;
                scanToken();
            }

            addToken(END_OF_FILE, -1);
            return tokens;
        }

        void printTokens() {
            for (int i=0; i<tokens.size(); i++){
                cout << "\"" << tokens[i].getType() << "\"";
                if (i+1 != tokens.size()) cout << " ";
            }
            cout << endl;
        }
};