#include "Tokenizer.h"
#include <iostream.h>
#include <String.h>
#include <fstream>

using namespace std;

class Lexer {
    Lexer(String filename);
    //构造函数,参数为文件名
    private Tokenizer tokenizer;
    //文本token提取器
    public Token ahead;
    //向前看的token
    void move();
    //向下读取一个token
};
