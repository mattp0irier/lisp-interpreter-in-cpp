#include <string>
#include <vector>
#include "Token.cpp"

using namespace std;

class Scanner {
    private:
        string input;
        int index;
        vector<Token> tokens;
        int currentLine;

        char getNextChar() {
            return input.at(index++);
        }

        void addToken(TokenType type, string value) {
            tokens.push_back(new Token(type, value, currentLine));
        }

        void scanToken() {
            char c = getNextChar();
            switch (c) {
            case '(':
                /* code */
                break;
            
            default:
                break;
            }
        }

    public:
        Scanner(string input) {
            this->input = input;
        }
};