
#include <iostream>
#include <string>
using namespace std;

#include "Tokenizer.h"
#include "Lexer.h"

int main() {
    cerr << "Please input the AQL filename." << endl;
    string filename;
    cin >> filename;

    string[] names = [""]

    Lexer lexer(filename);


    while (lexer.move() != false) {
        Token t = lexer.getAheadToken();

        cout << t.content << endl;
        cout << "Token type: " << t.type << endl;
        cout << "Token row: " << t.row << endl;
        cout << "Token col: " << t.col << endl;
    }

    return 0;
}
