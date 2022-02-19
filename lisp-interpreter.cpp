#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "scanner.cpp"
//#include "lisp-class.hpp";

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
        runFile(argv[0]);
    }
    else{
        runPrompt();
    }

    return 1;
}

void runFile(string filename){
    ifstream input(filename);;
    string str((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    run(str);
}

void runPrompt(){
    string line;
    while (1){
        cout << "> ";
        cin >> line;
//        if (line.length() == 0) break;
        run(line);
    }

    return;

}

void run(string line){
    Scanner scanner(line);
    scanner.scanTokens();
    scanner.printTokens();
}