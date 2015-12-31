#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum TYPE {
    CREATE,
    VIEW,
    AS,
    OUTPUT,
    SELECT,
    FROM,
    EXTRACT,
    REGEX,
    ON,
    RETURN,
    GROUP,
    AND,
    TOKEN,
    PATTERN, //以上为AQL的关键字,见PDF
    REG, //正则表达式
    NUM, //数字
    ID, //AQL中的变量名

    DOT = '.', //点号'.'
    PARENLEFT = '(', //左括号
    PARENRIGHT = ')', //右括号
    COMMA = ',', //逗号','
    BRACKETLEFT = '<', //左尖括号
    BRACKETRIGHT = '>', //右尖括号
    BRACESLEFT = '{', //左大括号
    BRACESRIGHT = '}', //右大括号
    SEMICOLON = ';'
};

class Token {
    public:
        Token() {}
        Token(string str, TYPE type, int row, int col, int position) {
            content = str;
            this->type = type;
            this->row = row;
            this->col = col;
            this->position = position;
        }
    //构造函数,参数为对应的文本,token类型,token所在的行列

        string content;
    //token文本
        TYPE type;
    //token类型
        long row;
        long col;
    // //在文本的行列位置
        long position;
};

class Tokenizer {
public:
    Tokenizer() {}
    ~Tokenizer() {}
    Tokenizer(string &fileName) {
        fileStream.open(fileName.c_str(), ifstream::in);
        if (!fileStream) {
            cerr << "Sorry, No such file ~" << endl;
            exit(0);
        } else {
            //cerr << "Open file succeed!" << endl;
        }

        std::string str;
        tokenString = "";
        while (getline(fileStream, str)) {
            tokenString += str + "\n";
        }
        fileStream.close();

        peek = ' ';
        current = 0;
        row = 1;
        col = 1;
    }
    //构造函数,参数为文件名

    void readch() {
        if (tokenString.size() > current) {
            peek = tokenString[current++];
            if (peek == '\n') {
                row++;
                col = 1;
            } else {
                col++;
            }
        }
        else {
            peek = EOF;     // flag end
        }
    }
    
    Token getTextToken() {
        while (peek == ' ' || peek == '\n' || peek == '\t') {
            readch();
        }
        int pos = current - 1;
        if (isdigit(peek)) {
            std::string s = "";
            do {
                s.append(1u, peek); readch();
            } while (isdigit(peek));
            return Token(s, NUM, 0, 0, pos);
        } else {
             std::string s = "";
            do {
                s.append(1u, peek); readch();
            } while (!(isdigit(peek)) && peek != ' ' && peek != '\n' && peek != '\t');
            return Token(s, ID, 0, 0, pos);           
        }
    }

    Token getToken() {
        //cout << peek << endl;
        while (peek == ' ' || peek == '\n' || peek == '\t') {
            readch();
        }

        int tok_col = col - 1;
        int tok_row = row;

        int pos = current - 1;

        if (isalpha(peek)) {
            std::string s = "";
            do {
                s.append(1u, peek);
                readch();
            } while (isalpha(peek) || isdigit(peek));
            if (peek == '\n') {
                tok_row = row - 1;
            }
            //cout << s << endl;
            if (s == "create") return Token(s, CREATE, tok_row, tok_col, pos);
            if (s == "view") return Token(s, VIEW, tok_row, tok_col, pos);
            if (s == "as") return Token(s, AS, tok_row, tok_col, pos);
            if (s == "output") return Token(s, OUTPUT, tok_row, tok_col, pos);
            if (s == "select") return Token(s, SELECT, tok_row, tok_col, pos);
            if (s == "from") return Token(s, FROM, tok_row, tok_col, pos);
            if (s == "extract") return Token(s, EXTRACT, tok_row, tok_col, pos);
            if (s == "regex") return Token(s, REGEX, tok_row, tok_col, pos);
            if (s == "on") return Token(s, ON, tok_row, tok_col, pos);
            if (s == "return") return Token(s, RETURN, tok_row, tok_col, pos);
            if (s == "group") return Token(s, GROUP, tok_row, tok_col, pos);
            if (s == "and") return Token(s, AND, tok_row, tok_col, pos);
            if (s == "Token" || s == "token") return Token(s, TOKEN, tok_row, tok_col, pos);
            if (s == "pattern") return Token(s, PATTERN, tok_row, tok_col, pos);
            return Token(s, ID, tok_row, tok_col, pos);
        }
        if (isdigit(peek)) {
            std::string s = "";
            do {
                s.append(1u, peek); readch();
            } while (isdigit(peek));
            if (peek == '\n') {
                tok_row = row - 1;
            }
            return Token(s, NUM, tok_row, tok_col, pos);
        }
        if (peek == '/') {
            std::string regex = "";
            // read regex expression
            readch();
            while (peek != '/') {
                regex.append(1u, peek);
                if (peek == '\\') {
                    readch();
                    regex.append(1u, peek);
                }
                readch();
            }
            if (peek == '\n') {
                tok_row = row - 1;
            }
            Token tok = Token(regex, REG, tok_row, tok_col, pos); peek = ' ';
            return tok;
        }
        // other case
        Token tok = Token(string(1, peek), (TYPE)peek, tok_row, tok_col, pos);
        peek = ' ';
        return tok;
    }

    // //从文件的文本中提取token字符串,保存到成员tokenString中并返回
    // public Token makeToken(String str, TYPE type);
    //将当前提取的token字符串包装成真正的Token(词法分析使用)
    bool isEnd() {
        while (peek == ' ' || peek == '\n' || peek == '\t') {
            readch();
        }
        return tokenString.size() <= current;
    }
    //判断文本是否提取到末尾


    private:
        ifstream fileStream;
    //读文件流
        string tokenString;
    //当前提取的token字符串(用于对原文的处理);
        int row;
    //当前读到第几行
        int col;
    //当前读到第几列

    ///********** edited by shuhuang **********///
        int current;
    // current position
        char peek;
    // top char
};

#endif
