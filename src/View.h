#include <iostream>
#include <vector>
#include <map>

class View {
    public vector<vector<Token>> groups;
    //保存View中的列,列中元素为符合条件的字符串
    public map<String, int> colIndex;
    //列与其别名的匹配
    public void createGroup(string name) {
        groups.push_back(vector<token>);
        map[name] = groups.size() - 1;
    }
    //创建列,参数为列名
    public bool insert(string name, Token token) {
        int index = map.find(name);
        if (index == map.end()) return false;
        groups[index].push_back(token);
    }
    //往列中插入元素
    public output() {
    }
    //输出View
    public &vector<Token> findByColName(string name) {
        int index = map.find(name);
        if (index == map.end()) return vector<token>;
        else return groups[index];
    }
    //根据列名查询列,返回列
};
