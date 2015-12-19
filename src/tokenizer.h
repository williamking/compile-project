#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
    END
};

class Token {
    public:
        Token() {

        };
        Token(string str, TYPE type, int row, int col) {
            content = str;
            this->type = type;
            this->row = row;
            this->col = col;
        }
    //构造函数,参数为对应的文本,token类型,token所在的行列

        string content;
    //token文本
        TYPE type;
    //token类型
        int row;
        int col;
    // //在文本的行列位置
    // public int position;
};

class Tokenizer {
public:
    Tokenizer(string fileName) {
        fileStream.open(fileName.c_str(), ifstream::in);

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
        if (tokenString.size() > current) 
            peek = tokenString[current++];
        else
            peek = EOF;     // flag end 
        col++;
        if (peek == '\n') {
            row++;
            col = 0;
        }
    }

    Token getToken() {
        while (isblank(peek)) {
            readch();
        }

        if (isalpha(peek)) {
            std::string s = "";
            do {
                s.append(1u, peek); readch();
            } while (isalpha(peek));
            return Token(s, ID, row, col);
        }
        if (isdigit(peek)) {
            std::string s = "";
            do {
                s.append(1u, peek); readch();
            } while (isdigit(peek));
            return Token(s, NUM, row, col);
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
            Token tok = Token(regex, REG, row, col); peek = ' ';
            return tok;
        }
        // other case
        Token tok = Token(string(1, peek), (TYPE)peek, row, col); peek = ' ';
        return tok;
    }

    // //从文件的文本中提取token字符串,保存到成员tokenString中并返回
    // public Token makeToken(String str, TYPE type);
    //将当前提取的token字符串包装成真正的Token(词法分析使用)
    bool isEnd() {
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
