/*************************************************************************
 > File Name: tet.cpp
 > Author: zhushh
 > Mail: sysuzhushh@163.com
 > Created Time: Wed 16 Dec 2015 05:35:19 PM CST
*************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include "tokenizer.h"

int main() {
    string str;
    cin >> str;

    Tokenizer tok(str);
    while (!tok.isEnd()) {
        Token t = tok.getToken();
        cout << t.content << endl;
        cout << "Token type: " << t.type << endl;
        cout << "Token row: " << t.row << endl;
        cout << "Token col: " << t.col << endl;
    }

    return 0;
}
