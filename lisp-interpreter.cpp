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

    while(1){
        eval(parse(read()));
    }
}

string read() {
    string blah;
    return;
}

void parse() {

    return;
}

void eval() {

}