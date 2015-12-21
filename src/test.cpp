
#include <iostream>
#include <string>
using namespace std;

#include "Tokenizer.h"
#include "Lexer.h"

int main() {
    cerr << "Please input the AQL filename." << endl;
    string filename;
    cin >> filename;

    string names[] = {"create", "view", "as", "output", "select", "from", "extract", "regex", "on", "return",
        "group", "and", "token", "pattern", "reg", "num", "id"};

    Lexer lexer(filename);


    while (lexer.move() != false) {
        Token t = lexer.getAheadToken();

        cout << t.content << endl;
        if (t.type < 17)
            cout << "Token type: " << names[t.type] << endl;
        else cout << "Token type: " << t.type << endl;
        cout << "Token row: " << t.row << endl;
        cout << "Token col: " << t.col << endl;
    }

    return 0;
}
