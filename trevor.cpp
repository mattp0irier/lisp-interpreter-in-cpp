#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

using namespace std;

void runFile(string filename);
void runPrompt();
void run(string line);


int main(int argc, char *argv[]){
    if (argc > 2) {
        cout << "Usage: lisp [file]" << endl;
        return -1;
    }
    if (argc == 1) {
        runFile(argv[0]);
    }
    else{
        runPrompt();
    }

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
        if (line == "") break;
        run(line);
    }

}

void run(string line){
    cout << "ran something" << endl;
}