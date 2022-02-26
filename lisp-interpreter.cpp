#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "scanner.cpp"
#include "expression3.cpp"
#include "parser2.cpp"
#include "interpreter.cpp"


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

    Parser *parser = new Parser(tokens);
    cout << "parser created" << endl;
    Interpreter interpreter;
    cout << "interpreter created" << endl;
    EXP *currentExpression = parser->getNextExpression();
    cout << "expression created" << endl;
    S_EXP *result;
    while(currentExpression != NULL) {
        result = interpreter.eval(currentExpression, emptyEnv());
        cout << result->toString() << endl;
        currentExpression = parser->getNextExpression();
    }
    // Expr expression = parser.parse();
//     cout << boost::any_cast<double>(interpreter.evaluate(parser.parse())) << endl;
//    Expression *exprTree = toExprTree(tokens, 0);
//    exprTree->print();
//     cout << endl;
}