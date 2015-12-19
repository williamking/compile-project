#include <iostream>
#include <vector>
#include <map>
#include <

class View {
    public vector<vector<Token>> groups;
    //保存View中的列,列中元素为符合条件的字符串
    public map<String, int> colIndex;
    //列与其别名的匹配
    public createGroup(string name);
    //创建列,参数为列名
    public insert(string name, Token token);
    //往列中插入元素
    public output();
    //输出View
    public &vector<Token> findByColName(string name);
    //根据列名查询列,返回列
};
