#include <iostream>
#include <string>
#include <fstream>

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

void parse() {
    // return array of tokens
    return;
}

void eval() {
    // do something with the tokens
}