#include <iostream>
#include <vector>
#include <map>

using namespace std;

class View {
public:
    vector< vector<Token> > groups;
    //保存View中的列,列中元素为符合条件的字符串
    map<string, int> colIndex;
    //列与其别名的匹配
    void createGroup(string name) {
        groups.push_back( vector<Token>() );
        colIndex[name] = groups.size() - 1;
    }
    //创建列,参数为列名
    bool insert(string name, Token token) {
        map<string, int>::iterator index = colIndex.find(name);
        if (index == colIndex.end()) return false;
        groups[index->second].push_back(token);
    }
    //往列中插入元素
    void output() {
    }
    //输出View
    vector<Token>& findByColName(string name) {
        map<string, int>::iterator index = colIndex.find(name);
        if (index == colIndex.end()) {
        	cout << "Error!" << endl;
			exit(0); 
        }
        else return groups[index->second];
    }
    //根据列名查询列,返回列
};

