#include "Lexer.h"
#include <iostream>
#include "View.h"
#include "Tokenizer.h"
//#include <>Executer.h>
#include <utility>
#include "regex.cpp"
#include <sstream>
#include "PatternMatcher.h"

using namespace std;

class Parser {
    private:
    //构造函数,参数为AQL文件名和处理文本文件名
    Tokenizer textTokenizer;
    //源文本分析器
    string textString;
    vector<View> views;
    //保存所有创建的View
    map<string, int> viewIndex;
    //View与其名称的绑定
    Lexer lexer;

    void error() {
        cout << "Error!" << endl;
        exit(0);
    }
    void move() {
    	if (lexer.move() == false) error();
    }
    //语法分析错误时的处理

    public:
    Parser(string fName, string artName): textTokenizer(artName), lexer(fName) {
		//textTokenizer(artName);
		//lexer(fName); 
        textString = "";
        string str;
        ifstream fileStream;
        fileStream.open(artName.c_str(), ifstream::in);
        while (getline(fileStream, str)) {
            textString += str + "\n";
        }
        createView("Document");
        views[0].createColumn("text");
        while (!textTokenizer.isEnd()) {
            views[0].insert("text", textTokenizer.getToken());
        }
    }

    void createView(string name) {
        views.push_back(View());
        viewIndex[name] = views.size() - 1;
    }
    //创建一个新的View

    void exec() {
        while (lexer.move())
		 	aql_stmt();
        //for (int i = 0; i < views.size(); ++i) {
        //    views[i].output();
       // }
    }
    //执行文本中一条AQL语句
    void aql_stmt() {
        if (lexer.getAheadToken().type == CREATE) {
            create_stmt();
        } else {
            if (lexer.getAheadToken().type == OUTPUT) {
                //output_stmt();
            } else {
                error();
            }
        }
    }
    //这是一个实现的例子
    void create_stmt() {
        move(); 
        if (lexer.getAheadToken().type != VIEW) error();
        move();
        if (lexer.getAheadToken().type != ID) error();
        string name = lexer.getAheadToken().content;
        createView(lexer.getAheadToken().content); 
        move();
        if (lexer.getAheadToken().type != AS) error();
        move();
        view_stmt(name);
        cout << "View: " << name << endl;
        View result = views[viewIndex[name]];
        result.output();
        //for (map<string, int>::iterator it = result.colIndex.begin(); it != result.colIndex.end(); ++it) {
            //cout << it->first << ':' << endl;
            //vector<Token> col = result.groups[it->second];
            //for (int j = 0; j < col.size(); ++j) {
                //Token tok = col[j];
                //cout << tok.content << ": (" << tok.position << ',' << tok.position + tok.content.length() << ')' << endl;
            //}
        //}
    }
    //创建View的语句,创建一条新View
    void view_stmt(string name) {
        //if (lexer.getAheadToken().type == SELECT) {
        //    move();
        //    select_stmt(name);
        //}
        if (lexer.getAheadToken().type == EXTRACT) {
            move();
            extract_stmt(name);
        }
    }   
    //传入参数为创建的View名称
    //void output_stmt();
    //map<string, string> alias();
    //返回View和其别名的绑定
    //void select_stmt(string name);
    //从已有的View中选择列插入到创建的新View中(参数为操作的View的名称)
    //map< string, pair<string,string> > select_list();
    //返回要组成新View的内容的多个列和其别名组成的map
    //map的key为选择的列并入到新View中的列名,value为选择的View别名和其列名组成的pair
    //pair< string, pair<string, string> > select_item();
    //返回select_list中map的一个value
    map<string, string> from_list() {
        map <string, string> results;
        bool cont = true;
        do {
            results.insert(from_item());
            if (lexer.getAheadToken().type != COMMA) cont = false;
            else move();
        } while (cont);
        if (lexer.getAheadToken().type != SEMICOLON) error();
        return results;
    }
    //返回View名和其别名组成的map
    pair<string, string> from_item() {
        if (lexer.getAheadToken().type != ID) error();
        string realName = lexer.getAheadToken().content;
        move();
        if (lexer.getAheadToken().type != ID) error();
        string name = lexer.getAheadToken().content;
        move();
        return make_pair(name, realName);
    }
    //返回View名和其别名组成的pair
    void extract_stmt(string name) {
        bool flag = false;
        string viewName;
        string regexp;
        string colName;
        map<int, string> nameSpec;
        PatternMatcher matcher(textString, views[0].getColByName("text"));

        if (lexer.getAheadToken().type == REGEX) {
            regex_spec(regexp, viewName, colName, nameSpec);
            flag = false;
        } else {
            if (lexer.getAheadToken().type == PATTERN) {
                flag = true;
                move();
                pattern_spec(matcher, nameSpec); 
            }
            else {
                error();
            }
        }
        if (lexer.getAheadToken().type != FROM) {
            error();
        }
        move();
        map<string, string> fromList = from_list();
        if (!flag) {
            View &view = views[viewIndex[name]];
            View &fromView = views[viewIndex[fromList[viewName]]];
            vector<Token> col = fromView.getColByName(colName);
            vector< vector<int> > spans = findall(regexp.c_str(), textString.c_str());
            for (int j = 0; j < nameSpec.size(); ++j)
                view.createColumn(nameSpec[j]);
            for (int i = 0; i < spans.size(); ++i) {
                for (int j = 0; j < spans[i].size(); j += 2) {
                    Token token = Token(textString.substr(spans[i][j], spans[i][j + 1] - spans[i][j]), ID, 0, 0, spans[i][j]);  
                    view.insert(nameSpec[j / 2], token);
                }   
            }
        } else {
            View &view = views[viewIndex[name]];
            map<string, View> fromViewsList;
            map<string, string>::iterator it;
            for (it = fromList.begin(); it != fromList.end(); ++it) {
                fromViewsList[it->first] = views[viewIndex[it->second]]; 
            }
            View &a = views[0];
            vector< vector< pair<int, int> > > results = matcher.match(a, fromViewsList);
            for (int j = 0; j < nameSpec.size(); ++j) {
                view.createColumn(nameSpec[j]);
            }
            for (int i = 0; i < results.size(); ++i) {
                for (int j = 0; j < results[i].size(); ++j) {
                    if (i != 0) {
                        bool jud = false;
                        for (int k = 0; k < results[0].size(); ++k) {
                            if ((results[0][k].first <= results[i][j].first) && (results[i][j].second <= results[0][k].second)) {
                                jud = true;
                                break;
                            }
                        }
                        if (!jud) continue;
                    }
                    view.insert(nameSpec[i], Token(textString.substr(results[i][j].first, results[i][j].second - results[i][j].first), ID, 0, 0, results[i][j].first));
                }   
            }     
        }
    }
    //执行正则匹配操作,并将结果赋予新View,参数为View的名称
    //public void extract_spec(vector<String> &regexps, );
    void regex_spec(string &regexp, string &viewName, string &colName, map<int, string> &nameSpec) {
        move(); 
        if (lexer.getAheadToken().type != REG) {
            error();
            return;
        }   
        regexp = lexer.getAheadToken().content;
        move();
        if (lexer.getAheadToken().type != ON) {
            error();
            return;
        } 
        move();
        pair<string, string> source = column();
        viewName = source.first;
        colName = source.second; 
        nameSpec = name_spec();
    }
    pair<string, string> column() {
        if (lexer.getAheadToken().type != ID) {
            error();
            exit(0);
        }
        string viewName = lexer.getAheadToken().content; 
        move();
        if (lexer.getAheadToken().type != DOT) {
            error();
        }
        move();
        if (lexer.getAheadToken().type != ID) {
            error();
        }
        string colName = lexer.getAheadToken().content;
        move();
        return make_pair(viewName, colName);
    }
    //返回来源View的别名以及其列名
    map<int, string> name_spec() {
        if (lexer.getAheadToken().type == AS) {
            lexer.move();
            if (lexer.getAheadToken().type != ID) {
                error();
            }
            map<int, string> nameSpec;
            nameSpec[0] = lexer.getAheadToken().content;
            lexer.move(); 
            return nameSpec;
        }
        if (lexer.getAheadToken().type == RETURN) {
            lexer.move();
            if (lexer.getAheadToken().type != GROUP) error();
            return group_spec();
        } 
    }
    //若是为as ID,则返回只有一个pair<0, ID>的map,否则返回 group_spec
    map<int, string> group_spec() {
        map<int, string> list;
        while (lexer.getAheadToken().type == GROUP) {
            lexer.move();
            if (lexer.getAheadToken().type != NUM) error();
            stringstream ss;
            ss << lexer.getAheadToken().content;
            int num;
            ss >> num; //提取num token
            lexer.move();
            if (lexer.getAheadToken().type != AS) error();
            lexer.move();
            if (lexer.getAheadToken().type != ID) error();
            string viewname = lexer.getAheadToken().content;
            list[num] = viewname;
            move();
            if (lexer.getAheadToken().type != AND) break;
            move();
        }
        return list;
    }
    //返回列号与列名的map
    //public pair<int, String> single_group();
    //返回新View列号与操作的新View的列名的pair
    void pattern_spec(PatternMatcher &matcher, map<int, string> &nameSpec) {
        pattern_expr(matcher); 
        nameSpec = name_spec();
    }
    void pattern_expr(PatternMatcher &matcher) {
        //cout << lexer.getAheadToken().content << endl;
        if (lexer.getAheadToken().type != BRACKETLEFT && lexer.getAheadToken().type
           != REG && lexer.getAheadToken().type != PARENLEFT) error();
        int cont = 1;
        while (cont) {
            atom(matcher);
            if (lexer.getAheadToken().type != BRACKETLEFT && lexer.getAheadToken().type
            != REG && lexer.getAheadToken().type != PARENLEFT) cont = 0;
        }
    }
    void atom(PatternMatcher &matcher) {
        if (lexer.getAheadToken().type == PARENLEFT) {
            move();
            matcher.insertSubMatcher();
            pattern_expr(matcher);
            if (lexer.getAheadToken().type != PARENRIGHT) error();
            move();
            matcher.popSubMatcher();
            return;
        }
        if (lexer.getAheadToken().type == BRACKETLEFT) {
            move();
            bool isToken = 0;
            pair<string, string> col;
            if (lexer.getAheadToken().type == ID) {
                col = column();
            } else {
                if (lexer.getAheadToken().type != TOKEN) error();
                isToken = 1;
                move();
            }
            if (lexer.getAheadToken().type != BRACKETRIGHT) error();
            move();
            int bot = 1, top = 1;
            if (lexer.getAheadToken().type == BRACESLEFT) {
                move();
                if (lexer.getAheadToken().type != NUM) error();
                stringstream ss;
                ss << lexer.getAheadToken().content;
                ss >> bot;
                move();
                if (lexer.getAheadToken().type != COMMA) error();
                move();
                if (lexer.getAheadToken().type != NUM) error();
                stringstream st;
                st << lexer.getAheadToken().content;
                st >> top;
                move();
                if (lexer.getAheadToken().type != BRACESRIGHT) error();
                move();
            }
            if (isToken) matcher.insertToken(bot, top);
            else matcher.insertColumn(col.first, col.second, bot, top);
            return;
        }
        if (lexer.getAheadToken().type == REG) {
            int bot = 1;
			int top = 1;
            string regexp = lexer.getAheadToken().content;
            lexer.move();
            if (lexer.getAheadToken().type == BRACESLEFT) {
                move();
                if (lexer.getAheadToken().type != NUM) error();
                stringstream ss;
                ss << lexer.getAheadToken().content;
                ss >> bot;
                move();
                if (lexer.getAheadToken().type != NUM) error();
                ss << lexer.getAheadToken().content;
                ss >> top;
                move();
                if (lexer.getAheadToken().type != BRACESRIGHT) error();
                move();
            } 
            matcher.insertReg(regexp, bot, top);
        } 
    }
    //void pattern_group();
    //以上的函数均为非终结符的匹配函数

};
