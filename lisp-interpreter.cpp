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
        // Invalid argument count
        cout << "Usage: lisp [file]" << endl;
        return -1;
    }
    if (argc == 2) {
        // Input file given
        runFile(argv[1]);
    }
    else{
        // Pull commands from stdin
        runPrompt();
    }

    return 1;
}

// runFile: run commands from file
void runFile(string filename){
    ifstream input(filename);
    string str;
    string allInput = "";

    while (getline(input, str)) {
        // Read the whole file into one string instead of separating lines into separate strings
        if (str.length()) allInput += str;
    }
    run(allInput);
}

// runPrompt: get commands from user one line at a time
void runPrompt(){
    string line;
    while (1){
        // prompt user for input
        cout << "> ";
        getline(cin, line);

        // terminate if user gives blank command
        if (line.length() == 0) break;
        run(line);
    }
    return;

}

// run a command or series of commands
void run(string line){

    Scanner scanner(line);

    // separate user input into tokens
    vector<Token> tokens = scanner.scanTokens();

    // Objects used for parsing and interpreting expressions as they are read in
    Parser *parser = new Parser(tokens);
    Interpreter interpreter;
    EXP *currentExpression;
    string resultString;

    // First expression is necessary before entering loop
    currentExpression = parser->getNextExpression();
    // Expressions return S-Expressions
    S_EXP *result;

    while(currentExpression != NULL) {
        result = interpreter.eval(currentExpression, emptyEnv());

        // print function returns null and does not need to be printed again
        if (result != NULL) {
            // Use respective toString() function based on result type
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
                // we shouldn't get here
                cout << result->type << endl;
                cout << "error" << endl;
            }
        }
        // returns NULL if no more expressions are found
        currentExpression = parser->getNextExpression();
    }
}