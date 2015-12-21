#ifndef LEXER_H
#define LEXER_H

#include <string.h>
#include <fstream>
#include "Tokenizer.h"

using namespace std;

class Lexer {

public:
    Lexer(string filename) {
        ahead = Token();
        if (filename != "") {
            tokenizer = new Tokenizer(filename);
            if (!tokenizer) {
                cerr << "Tokenizer:Out of memory!" << endl;
                exit(0);
            }
        }
    }
    //构造函数,参数为文件名

    bool move() {
        if (!(*tokenizer).isEnd()) {
            ahead = (*tokenizer).getToken();
            return true;
        } else {
            cerr << "Well, EOF." << endl;
            return false;
        }
    }
    //向下读取一个token
    Token getAheadToken() {
        return ahead;
    }

private:
    Tokenizer* tokenizer;
    //文本token提取器
    Token ahead;
    //向前看的token
};

#endif
