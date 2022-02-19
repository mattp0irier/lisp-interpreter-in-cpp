#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "interpreter.hpp";
#include "lisp-class.hpp";

using namespace std;

string read();
void parse();
void eval();

int main() {
    cout << "hello world" << endl;
    cout << "testing changes" << endl;

    int go = 1;
    while(go){
        go = eval(parse(read()));
    }
}

string read() {
    string line;
    cout << '>';
    cin >> line;
    return line;
}

void parse(string inputLine) {
    int i = 0;
    vector<Token> tokenList;
    while(i < inputLine.length()) {
        if (inputLine[i] == '(') {
            
            // Token t = new Token(leftParen, '(')
            // tokenList.push_back(t);
            // i++;
        }
        else if(inputLine[i] == ')') {
            // Token t = new Token(leftParen, ')')
            // tokenList.push_back(t);
            // i++;
        }
        else if(isalpha(inputLine[i])) {
            
        }
        else {
            // error
        }
    }
    return;
}

void eval() {
    // do something with the tokens
}


/*
(+ 2 (* 3 (+ 5 8)))
(+ 2 (if (< 3 4) (5) (6)))

(if (< 3 4) (print 5) (print 6))

look for leftParen, increment paren depth, increment pos
while(not whitespace) {
    look at chars
}
match to operation type with switch statement, else return error

skip whitespace



*/