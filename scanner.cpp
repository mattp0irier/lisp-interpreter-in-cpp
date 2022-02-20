#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cctype>
#include "token.cpp"

using namespace std;

class Scanner {
    private:
        string input;
        int startIndex;
        int index = 0;
        vector<Token> tokens;
        int currentLine;

        map<string, TokenType> keywords = {
            {"if", IF},
            {"while", WHILE},
            {"set", SET},
            {"begin", BEGIN},
            {"cons", CONS},
            {"car", CAR},
            {"cdr", CDR},
            {"number?", IS_NUMBER},
            {"symbol?", IS_SYMBOL},
            {"list?", IS_LIST},
            {"null?", IS_NULL},
            {"print", PRINT}
        };

        char getNextChar() {
            return input.at(index++);
        }

        char peek() {
            if (done()) return '\0';
            return input.at(index);
        }

        bool done() {
            return index >= input.length();
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
            case 'T':
                if (isalpha(peek())) getIdentifier();
                else addToken(T, string(1, c));
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
            // string handler
            case '\"':
                getString();
                break;
            default:
                if(isdigit(c)) {
                    getNumber();
                }
                else if (isalpha(c)) {
                    getIdentifier();
                }
                else {
                    cout << "Invalid character " << input.at(index) << " in line " << currentLine << endl;
                }
                break;
            }
        }

        void getNumber() {
            while (isdigit(peek())) {
                getNextChar();
            }
            // FIXME support for floating point nums
            addToken(NUMBER, stoi(input.substr(startIndex, index-startIndex)));
        }

        void getIdentifier() {
            while (isalpha(peek())) {
                getNextChar();
            }
            
            // Identifier may end with ?
            if (peek() == '?') getNextChar();

            string idName = input.substr(startIndex, index-startIndex);
            for (int i=0; i<idName.length(); i++){
                idName[i] = tolower(idName[i]);
            }
            TokenType type;
            if (keywords.count(idName)){
                type = keywords[idName];
            }
            else {
                type = IDENTIFIER;
            } 
            addToken(type, idName);
        }

        void getString() {
            while (peek() != '\"' && !done()) {
                if (peek() == '\n') currentLine++;
                getNextChar();
            }
            cout << "while loop broke at index " << index << endl;
            if (done()) {
                cout << "Error: Unterminated String" << endl;
                return;
            }
            getNextChar();
            addToken(STRING, input.substr(startIndex+1, index-startIndex-2));
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
                //cout << "scanning token at index " << index << endl;
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