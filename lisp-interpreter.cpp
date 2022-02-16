#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include interpreter.hpp
#include lisp-class.hpp;

using namespace std;

string read();
void parse();
void eval();

int main() {
    cout << "hello world" << endl;
    cout << "testing changes" << endl;

    while(1){
        eval(parse(read()));
    }
}

string read() {
    string blah;
    return;
}

void parse(string inputLine) {
    int i = 0;
    vector<Token> tokenList;
    while(i < inputLine.length()) {
        if (inputLine[i] == '(') {
            Token t = new Token(leftParen, '(')
            tokenList.push_back(t);
            i++;
        }
        else if(inputLine[i] == ')') {
            Token t = new Token(leftParen, ')')
            tokenList.push_back(t);
            i++;
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

}