#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "scanner.hpp"
#include "expression3.hpp"
#include "parser2.hpp"
#include "interpreter.hpp"


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
    string allInput = "";
    // int i = 1;
    while (getline(input, str)) {
        // cout << "\nLINE " << i << ":\n\tINPUT: " << str << "\n\tOUTPUT: "; 
        if (str.length()) allInput += str;
        // i++;
    }
    run(allInput);
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
    //scanner.printTokens();

    Parser *parser = new Parser(tokens);
    Interpreter interpreter;
    EXP *currentExpression;
    string resultString;
    //scanner.printTokens();
    currentExpression = parser->getNextExpression();
    S_EXP *result;
    while(currentExpression != NULL) {
        result = interpreter.eval(currentExpression, emptyEnv());
        if (result->type == "Number") {
            NUM_SXP* n = (NUM_SXP*)result;
            cout << n->toString() << endl;
        }
        else if (result->type == "Symbol") {
            SYM_SXP* n = (SYM_SXP*)result;
            cout << n->toString() << endl;
        }
        else if (result->type == "List") {
            LIST_SXP* n = (LIST_SXP*)result;
            cout << n->toString() << endl;
        }
        else if (result->type == "TRUE"){
            cout << result->type << endl;
        }
        else if (result->type == "()"){
            cout << result->type << endl;
        }
        else {
            cout << result->type << endl;
            cout << "error" << endl;
        }
        currentExpression = parser->getNextExpression();
    }
}