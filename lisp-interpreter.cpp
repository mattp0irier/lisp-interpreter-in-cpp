#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "scanner.cpp"
#include "expression.cpp"
#include "parser.cpp"


using namespace std;

void runFile(string filename);
void runPrompt();
void run(string line);


int main(int argc, char *argv[]){
    if (argc > 2) {
        cout << "Usage: lisp [file]" << endl;
        return -1;
    }
    if (argc == 2) {
        runFile(argv[1]);
    }
    else{
        runPrompt();
    }

    return 1;
}

void runFile(string filename){
    ifstream input(filename);
    string str;
    while (input) {
        getline(input, str);
        if (str.length()) run(str);
    }
}

void runPrompt(){
    string line;
    while (1){
        cout << "> ";
        getline(cin, line);
        if (line.length() == 0) break;
        run(line);
    }

    return;

}

void run(string line){
    Scanner scanner(line);
    vector<Token> tokens = scanner.scanTokens();
    scanner.printTokens();
    Parser parser(tokens);
    Expr expression = parser.parse();
    Expression *exprTree = toExprTree(tokens, 0);
    exprTree->print();
    cout << endl;
}