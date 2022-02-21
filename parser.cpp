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

        void doTheParseThing() {
            
        }


};