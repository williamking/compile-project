#include "Lexer.h"
#include <iostream.h>
#include <View.h>
#include <Tokenizer.h>
#include <Executer.h>
#include <utility>
#include <regex.h>

using namespace std;

class Parser {
    Parser(String fName, String artName): textTokenizer(artname), programTokenizer(fName){  
    }
    //构造函数,参数为AQL文件名和处理文本文件名
    private Tokenizer textTokenizer;
    //源文本分析器
    private string textString;
    private vector<View> views;
    //保存所有创建的View
    private map<String, int> viewIndex;
    //View与其名称的绑定
    //private Lexer lexer;
    private Tokenizer programTokenizer;
    //词法分析器
    
    public createView(String name) {
        View newView();
    }
    //创建一个新的View

    private error();
    //语法分析错误时的处理

    public void exec() {
        while ()
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
    public void extract_stmt(String name) {
        boolean flag = 0;
        string viewName;
        string regexp;
        string colName;
        map<int, string> nameSpec;

        if (lexer.ahead.type == REGEX) {
            regex_spec(regexp, viewName, colName, nameSpec);
            flag = 0;
        } else {
            if (lexer.ahead.type == PATTERN) {
                pattern_spec();
                flag = 1;
            }
            else {
                error();
                return;
            }
        }
        if (lexer.ahead.type != FROM) {
            error();
            return;
        }
        lexer.move();
        map<string, string> fromList = from_list();
        if (!flag) {
            View &view = views[viewIndex[name]];
            View &fromView = views[viewIndex[fromList[viewName]]];
            vector<string> col = fromView.groups[fromView.colIndex[colName]];
            vector<vector<int>> spans = findall(regexp.c_str(), textString.c_str());
            for (j = 0; j < namespec.size(); ++j)
                view.createGroup(namespec[j]);
            for (int i = 0; i < spans.size(); ++i) {
                for (int j = 0; j < spans[i].size(); j += 2) {
                    Token token = new Token(textString.substr(spans[i][j], spans[i][j + 1] - spans[i][j]), 0, 0, spans[i][j]);  
                    view.insert(namespec[j / 2], token);
                }   
            }
        }
    }
    //执行正则匹配操作,并将结果赋予新View,参数为View的名称
    //public void extract_spec(vector<String> &regexps, );
    public void regex_spec(String &regexp, String &viewName, String &colName, map<int, string> &nameSpec) {
        move(); 
        if (lexer.ahead.type != REGEXP) {
            error();
            return;
        }   
        regexp = lexer.ahead.content;
        move();
        if (lexer.ahead.type != ON) {
            error();
            return;
        } 
        move();
        pair<string, string> source = column();
        viewName = source.first;
        colName = source.second; 
        nameSpec = name_spec();
    }
    public pair<string, string> column() {
        if (lexer.ahead.type != ID) {
            error();
            exit(0);
        }
        string viewName = lexer.ahead.content; 
        move();
        if (lexer.ahead.type != DOT) {
            error();
            exit(0);
        }
        if (lexer.ahead.type != ID) {
            error();
             return;
        }
        string colName = lexer.ahead.
        move();
        return make_pair(viewName, colName);
    }
    //返回来源View的别名以及其列名
    public map<int, string> name_spec() {
        if (lexer.ahead.type == AS) {
            move();
            if (lexer.ahead.type != ID) {
                error();
            }
            map<int, string> nameSpec;
            nameSpec[0] = lexer.ahead.content;
            move(); 
            return nameSpec;
        }
         
    }
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

};
