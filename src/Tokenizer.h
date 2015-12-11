#include <stdio.h>
#include <String.h>
#include <fstream>

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
    DOT, //点号'.'
    PARENLEFT, //左括号
    PARENRIGHT, //右括号
    COMMA, //逗号','
    BRACKETLEFT, //左尖括号
    BRACKETRIGHT, //右尖括号
    BRACESLEFT, //左大括号
    BRACESRIGHT //右大括号
} 

class Token {
    Token(String string, TYPE type, int row, int col);
    //构造函数,参数为对应的文本,token类型,token所在的行列
    public String content;
    //token文本
    public TYPE type;
    //token类型
    public int row;
    public int col;
    //在文本的行列位置
}

class Tokenizer {
    Tokenizer(String fileName);
    //构造函数,参数为文件名
    private ifstream fileStream;
    //读文件流
    private String tokenString;
    //当前提取的token字符串(用于对原文的处理);
    private int row;
    //当前读到第几行
    private int col;
    //当前读到第几列
    public String getTokenString();
    //从文件的文本中提取token字符串,保存到成员tokenString中并返回
    public Token makeToken(String str, TYPE type);
    //将当前提取的token字符串包装成真正的Token(词法分析使用)
    public boolean isEnd();
    //判断文本是否提取到末尾
}
