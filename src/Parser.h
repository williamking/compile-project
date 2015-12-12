#include "Lexer.h"
#include <iostream.h>
#include <View.h>
#include <Tokenizer.h>
#include <Executer.h>
#include <utility>

using namespace std;

class Parser {
    Parser(String filename, String articleName);
    //构造函数,参数为AQL文件名和处理文本文件名
    private Tokenizer textTokenizer;
    //源文本分析器
    private vector<View> views;
    //保存所有创建的View
    private map<String, int> viewIndex;
    //View与其名称的绑定
    private Lexer lexer;
    //词法分析器
    
    public createView(String name);
    //创建一个新的View

    private error();
    //语法分析错误时的处理

    public void exec() {
    }
    //执行文本中一条AQL语句
    public void aql_stmt() {
        if (lexer.ahead.type == CREATE) {
            create_stmt();
        } else {
            if (lexer.next.type == OUTPUT) {
                output_stmt();
            } else {
                error();
            }
        }
    }
    //这是一个实现的例子
    public void create_stmt();
    //创建View的语句,创建一条新View
    public void view_stmt(String name);    
    //传入参数为创建的View名称
    public void output_stmt();
    public String alias();
    //返回别名
    public map<String, String> alias();
    //返回View和其别名的绑定
    public void select_stmt(String name);
    //从已有的View中选择列插入到创建的新View中(参数为操作的View的名称)
    public map<String, pair<String,String>> select_list();
    //返回要组成新View的内容的多个列和其别名组成的map
    //map的key为选择的列并入到新View中的列名,value为选择的View别名和其列名组成的pair
    public pair<String, pair<String, String>> select_item();
    //返回select_list中map的一个value
    public map<String, String> from_list();
    //返回View名和其别名组成的map
    public pair<String, String> from_item();
    //返回View名和其别名组成的pair
    public void extract_stmt(String name);
    //执行正则匹配操作,并将结果赋予新View,参数为View的名称
    public void extract_spec(vector<String> &regexps, );
    public void regex_spec(String &regexp, String &viewName, String &colName, map<int, String> nameSpec);
    public pair<String, String> column();
    //返回来源View的别名以及其列名
    public map<int, String> name_spec();
    //若是为as ID,则返回只有一个pair<0, ID>的map,否则返回 group_spec
    public map<int, String> group_spec();
    //返回列号与列名的map
    public pair<int, String> single_group();
    //返回新View列号与操作的新View的列名的pair
    public PatternMatcher pattern_spec();
    public void pattern_expr();
    public void pattern_pkg();
    public Atom atom();
    public void pattern_group();
    //以上的函数均为非终结符的匹配函数

}