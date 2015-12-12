#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <fstream>

using namespace std;

enum {
    REG,
    TOKEN,
    COLUMN
}

struct Val {
    String regexp;
    pair<String, String> column;
}

struct Atom {
    int type;
    Val value;
    int num;
}
//patern模式中的atom,可能为任意token,可能为某View中的一列,可能为正则表达式,用type表示其类型,若为正则,则表达式在value成员的regexp成员中,若为token,则其别名及列名在value成员的column成员中

class PatternMatcher {
    private vector<atom> atoms;
    public match(map<String, String> index, String filename);
    //匹配方法,参数为View的别名和其名字的map和匹配的文件名
}
