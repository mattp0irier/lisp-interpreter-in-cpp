#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <map>
#include <cctype>
#include "token.hpp"

using namespace std;

// Scanner class: used to create tokens from input text
class Scanner {
    private:
        string input;
        int startIndex;
        int index = 0;
        vector<Token> tokens;
        int currentLine; // keeps track of line number being scanned

        // map for keyword matching to TokenType
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
            {"print", PRINT},
            {"define", DEFINE},
            {"call", CALL}
        };

        // map for TokenType matching to number of expected args
        map<TokenType, int> arityMap = {
            {IF, 3},
            {WHILE, 2},
            {SET, 2},
            {BEGIN, -2},
            {CONS, 2},
            {CAR, 1},
            {CDR, 1},
            {IS_NUMBER, 1},
            {IS_SYMBOL, 1},
            {IS_LIST, 1},
            {IS_NULL, 1},
            {PRINT, 1},
            {DEFINE, 2},
            {IDENTIFIER, 0}
        };

        // getNextChar: increment index and get character at that index
        char getNextChar() {
            return input.at(index++);
        }

        // peek: get character at index if not at end of length
        char peek() {
            if (done()) return '\0';
            return input.at(index);
        }

        // peekNext: get next character
        char peekNext() {
            return input.at(index + 1);
        }

        // done: check for out of bounds on string
        bool done() {
            return index >= input.length();
        }

        // addToken: add token with string value to TokenList
        void addToken(TokenType type, string value, int arity) {
            tokens.push_back(Token(type, value, currentLine, arity));
        }

        // addToken: add token with int value to TokenList
        void addToken(TokenType type, int value, int arity) {
            tokens.push_back(Token(type, value, currentLine, arity));
        }

        // addToken: add token with double value to TokenList
        void addToken(TokenType type, double value, int arity) {
            tokens.push_back(Token(type, value, currentLine, arity));
        }

        // scanToken: get next character and create token
        void scanToken() {
            char c = getNextChar();
            switch (c) {
            case '(':
                addToken(LEFT_PAREN, string(1, c), 0);
                break;
            case ')':
                addToken(RIGHT_PAREN, string(1, c), 0);
                break;
            case '+':
                addToken(PLUS, string(1, c), 2);
                break;
            case '-':
                // check for negative number
                if (isdigit(peek())){ 
                    getNumber();
                    break;
                }
                addToken(MINUS, string(1, c), 2);
                break;
            case '*':
                addToken(MULTIPLY, string(1, c), 2);
                break;
            case '/':
                addToken(DIVIDE, string(1, c), 2);
                break;
            case '%':
                addToken(MOD, string(1, c), 2);
                break;
            case '=':
                addToken(EQUAL, string(1, c), 2);
                break;
            case '<':
                // check for <=
                if (peek() == '=') {addToken(LTE, string(2, c), 2); index++;}
                else addToken(LESS_THAN, string(1, c), 2);
                break;
            case '>':
                // check for >=
                if (peek() == '=') {addToken(GTE, string(2, c), 2); index++;}
                else addToken(GREATER_THAN, string(1, c), 2);
                break;
            case 't':
            case 'T':
                // check to ensure not a variable or function name
                if (isalpha(peek())) getIdentifier();
                else addToken(T, string(1, c), 0);
                break;
            // ignore whitespace
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                currentLine++; // increment line counter
                break;
            // string literal handler
            case '\"':
                getString();
                break;
            default:
                // check for number
                if(isdigit(c)) {
                    getNumber();
                }
                // check for identifier
                else if (isalpha(c)) {
                    getIdentifier();
                }
                else {
                    cout << "Invalid character " << input.at(index) << " in line " << currentLine << endl;
                }
                break;
            }
        }

        // getNumber: get int or float from input
        void getNumber() {
            // check for negative number
            bool neg = false;
            if (peek() == '-') { 
                neg = true;
                getNextChar();
            }
            // continue while still digits following the position
            while (isdigit(peek())) {
                getNextChar();
            }
            // check for floats
            if (peek() == '.' && isdigit(peekNext())) {
                getNextChar();
                while (isdigit(peek())) getNextChar();
                if (neg) addToken(FLOAT, -1*(double)stof(input.substr(startIndex, index-startIndex)), 0);
                else addToken(FLOAT, (double)stof(input.substr(startIndex, index-startIndex)), 0);
            }
            else{
                if (neg) addToken(INTEGER, -1*stoi(input.substr(startIndex, index-startIndex)), 0);
                else addToken(INTEGER, stoi(input.substr(startIndex, index-startIndex)), 0);
            }
        }

        // getIdentifier: get variable or function identifier from input
        void getIdentifier() {
            // continue while still characters following the current position
            while (isalpha(peek())) {
                getNextChar();
            }
            
            // Identifier may end with ?
            if (peek() == '?') getNextChar();

            // convert identifier to lower case
            string idName = input.substr(startIndex, index-startIndex);
            for (int i=0; i<idName.length(); i++){
                idName[i] = tolower(idName[i]);
            }
            
            TokenType type;
            // check for builtin keyword using map
            if (keywords.count(idName)){
                type = keywords[idName];
            }
            // otherwise identifier
            else {
                type = IDENTIFIER;
            } 
            addToken(type, idName, arityMap[type]);
        }

        // getString: get string literal from input
        void getString() {
            // while not end of input and not another quote, get next char
            while (peek() != '\"' && !done()) {
                if (peek() == '\n') currentLine++;
                getNextChar();
            }
            if (done()) {
                cout << "Error: Unterminated String" << endl;
                return;
            }
            getNextChar(); // increment position
            addToken(STRING, input.substr(startIndex+1, index-startIndex-2), 0);
        }


    public:
        // Scanner: constructs scanner with input provided as string
        Scanner(string input) {
            this->input = input;
            currentLine = 0;
        }

        // scanTokens: get tokens from input and return token list
        vector<Token> scanTokens() {
            // while not end of input
            while(index < input.length()) {
                startIndex = index;
                scanToken();
            }

            addToken(END_OF_FILE, -1, -1); // add EOF token when input complete
            return tokens;
        }

        // printTokens: output token list
        void printTokens() {
            // loop through tokens
            for (int i=0; i<tokens.size(); i++){
                cout << i << ": ";
                // print correct value based on type
                if(tokens[i].getType() == INTEGER) {
                    cout << "\"" << tokens[i].getIntVal() << "\"";
                }
                else if (tokens[i].getType() == FLOAT) {
                    cout << fixed << setprecision(2);
                    cout << "\"" << tokens[i].getFloatVal() << "\"";
                }
                else if (tokens[i].getType() == END_OF_FILE){
                    cout << "\"eof\"";
                }
                else {
                    cout << "\"" << tokens[i].getVal() << "\"";
                }
                if (i+1 != tokens.size()) cout << " ";
            }
            cout << endl;
        }
};