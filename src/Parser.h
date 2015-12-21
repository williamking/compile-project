#include "Lexer.h"
#include <iostream.h>
#include <View.h>
#include <Tokenizer.h>
#include <Executer.h>
#include <utility>
#include <regex.h>
#include <sstring>
#include <PatternMatcher>

using namespace std;

class Parser {
    Parser(String fName, String artName): textTokenizer(artname), programTokenizer(fName){  
        textString = "";
        while (getline(fileStream, str)) {
            textString += str + "\n";
        }
        createView("Document");
        views[0].createGroup("text");
        while (!textTokenizer.isEnd()) {
            textTokenizer.findByColName("text").push_back(textTokenizer.getToken());
        }
    }
    //构造函数,参数为AQL文件名和处理文本文件名
    private Tokenizer textTokenizer;
    //源文本分析器
    private string textString;
    private vector<View> views;
    //保存所有创建的View
    private map<String, int> viewIndex;
    //View与其名称的绑定
    private Lexer lexer;
    
    public createView(String name) {
        views.push_back(new View());
        viewIndex[name] = view.size() - 1;
    }
    //创建一个新的View

    private error() {
        cout << "Error!" << endl;
        exit(0);
    }
    //语法分析错误时的处理

    public void exec() {
        while (lexer.ahead.type != END) aql_stmt();
        for (int i = 0; i < views.size(); ++i) {
            views[i].output();
        }
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
    public void create_stmt() {
        move(); 
        if (lexer.ahead.type != VIEW) error();
        move();
        if (lexer.ahead.type != ID) error();
        string name = lexer.ahead.content;
        createView(lexer.ahead.content); 
        move();
        view_stmt(name);
    }
    //创建View的语句,创建一条新View
    public void view_stmt(String name) {
        if (lexer.ahead.type == SELECT) {
            move();
            select_stmt(name);
        }
        if (lexer.ahead.type == EXTRACT) {
            move();
            extract_stmt(name)
        };
    }   
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
            }
        }
        if (lexer.ahead.type != FROM) {
            error();
        }
        lexer.move();
        map<string, string> fromList = from_list();
        if (!flag) {
            View &view = views[viewIndex[name]];
            View &fromView = views[viewIndex[fromList[viewName]]];
            vector<string> col = fromView.groups[fromView.colIndex[colName]];
            vector<vector<int>> spans = findall(regexp.c_str(), textString.c_str());
            for (int j = 0; j < namespec.size(); ++j)
                view.createGroup(namespec[j]);
            for (int i = 0; i < spans.size(); ++i) {
                for (int j = 0; j < spans[i].size(); j += 2) {
                    Token token = new Token(textString.substr(spans[i][j], spans[i][j + 1] - spans[i][j]), 0, 0, spans[i][j]);  
                    view.insert(namespec[j / 2], token);
                }   
            }
        } else {
            PatternMatcher matcher(textString, view[0].groups[0]);
            pattern_spec(matcher);
            map<string, View> fromViewsList;
            for (int i = 0; i < fromList.size(); ++i) {
                fromViewsList[fromList[i].first] = views[viewIndex[fromList[i].second]]; 
            }
            matcher.match(views[0], fromViewsList);
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
            lexer.move();
            if (lexer.ahead.type != ID) {
                error();
            }
            map<int, string> nameSpec;
            nameSpec[0] = lexer.ahead.content;
            lexer.move(); 
            return nameSpec;
        }
        if (lexer.ahead.type == RETURN) {
            lexer.move();
            if (lexer.ahead.type != GROUP) error();
            return group_spec();
        } 
    }
    //若是为as ID,则返回只有一个pair<0, ID>的map,否则返回 group_spec
    public map<int, string> group_spec() {
        map<int, string> list;
        while (lexer.ahead.type == GROUP) {
            lexer.move();
            if (lexer.ahead.type != NUM) error();
            stringstream ss;
            ss << lexer.ahead.content;
            int num;
            ss >> num; //提取num token
            lexer.move();
            if (lexer.ahead.type != AS) error();
            lexer.move();
            if (lexer.ahead.type != ID) error();
            string viewname = lexer.ahead.content;
            list[num] = viewname;
            move();
            if (lexer.ahead.type != AND) break;
            move();
        }
        return list;
    }
    //返回列号与列名的map
    //public pair<int, String> single_group();
    //返回新View列号与操作的新View的列名的pair
    public void pattern_spec(PatternMatcher &matcher) {
        pattern_expr(matcher); 
        map<int, string> colList = namespec();
    }
    public void pattern_expr(PaternMatcher &matcher) {
        if (lexer.ahead.type != BRACKETLEFT && lexer.ahead.type
           != REGEXP && lexer.ahead.type != PARENLEFT) error();
        int cont = 1;
        while (cont) {
            atom(matcher);
            if (lexer.ahead.type != BRACKETLEFT && lexer.ahead.type
            != REGEXP) cont = 0;
        }
    }
    public void atom(PatternMatcher &matcher) {
        if (lexer.ahead.type == PARENLEFT) {
            move();
            matcher.insertSubMatcher();
            pattern_expr();
            if (lexer.ahead.type != PARENLEFT) error();
            move();
            matcher.popSubMatcher();
        }
        if (lexer.ahead.type == BRACKETLEFT) {
            move();
            boolean isToken = 0;
            pair<string, string> col;
            if (lexer.ahead.type == ID) {
                col = column();
            } else {
                if (lexer.ahead.type != TOKEN) error();
                isToken = 1;
                lexer.move();
            }     
            int bot = top = 1;
            if (lexer.ahead.type == BRACESLEFT) {
                move();
                if (lexer.ahead.type != NUM) error();
                stringstream ss;
                ss << lexer.ahead.content;
                ss >> bot;
                move();
                if (lexer.ahead.type != NUM) error();
                ss << lexer.ahead.content;
                ss >> top;
                move();
                if (lexer.ahead.type != BRACESRIGHT) error();
                move();
            }
            if (isToken) matcher.insertToken(bot, up);
            else matcher.insert(col.first, col.second, bot, up);
        }
        if (lexer.ahead.type == REGEXP) {
            int bot = top = 1;
            string regexp = lexer.ahead.content;
            lexer.move();
            if (lexer.ahead.type == BRACESLEFT) {
                move();
                if (lexer.ahead.type != NUM) error();
                stringstream ss;
                ss << lexer.ahead.content;
                ss >> bot;
                move();
                if (lexer.ahead.type != NUM) error();
                ss << lexer.ahead.content;
                ss >> top;
                move();
                if (lexer.ahead.type != BRACESRIGHT) error();
                move();
            } 
            matcher.insertReg(regexp, bot, up);
        } 
    }
    public void pattern_group();
    //以上的函数均为非终结符的匹配函数

};
