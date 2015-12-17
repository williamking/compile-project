#include <iostream>
#include <vector>
#include <map>

class View {
    private vector<vector<String>> groups;
    //保存View中的列,列中元素为符合条件的字符串
    private map<String, int> colIndex;
    //列与其别名的匹配
    public createGroup(String name);
    //创建列
    public insert(int index, String str);
    //往列中插入元素
    public output();
    //输出View
};
