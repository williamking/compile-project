#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <fstream>
#include <stack>

using namespace std;

//enum {
//    REG,
//    TOKEN,
//    COLUMN
//}


struct Atom {
    Atom(int t, string reg, string name, int bot, int up, int paren) {
        type = t;
        regexp = reg;
        viewName = name;
        nums = make_pair(bot, up);
        parenNum = paren;
    }
    int type;
    //Atom类型,0为正则,1为列,2为token,3为子表达式
    string regexp;
    string viewName;
    pair<int, int> nums;
    int parenNum;
};
//patern模式中的atom,可能为任意token,可能为某View中的一列,可能为正则表达式,用type表示其类型,若为正则,则表达式在value成员的regexp成员中,若为token,则其别名及列名在value成员的column成员中

class PatternMatcher {
	private:
    vector< vector<Atom> > atoms;
    vector< vector< pair<int, int> > > results;
    string text;
    vector<Token> document;
    map<string, string> columns;
    map<int, string> returnList;
    int current;
    stack<int> catchStack;
    public:
    PatternMatcher(string &t, vector<Token> &doc) {
        current = 0;
        text = t;
        document = doc; 
        catchStack.push(0);
        atoms.push_back(vector<Atom>());
    }
    map<string, View> sourceViews;
    void insertReg(string regexp, int bot, int up) {
        atoms[catchStack.top()].push_back(Atom(0, regexp, "", bot, up, 0));
    }
    void insertColumn(string viewName, string colName, int bot, int up) {
        atoms[catchStack.top()].push_back(Atom(1, "", viewName, bot, up, 0));
        columns[viewName] = colName;
    }
    void insertToken(int bot, int up) {
        atoms[catchStack.top()].push_back(Atom(2, "", "", bot, up, 0));
    }
    void insertSubMatcher() {
        catchStack.push(current + 1);
        vector<Atom> newAtom;
        atoms.push_back(newAtom);
        atoms[catchStack.top()].push_back(Atom(3, "", "", 0, 0, ++current));
    }
    void insertReturnColumn(int index, string colName) {
        returnList[index] = colName;
    }
    void popSubMatcher() {
        catchStack.pop();
    }
    vector<Token> match(View &textView, map<string, View> &views) {
        sourceViews = views;
        for (int i = 0; i < atoms.size(); ++i) {
            results.push_back(vector< pair<int, int> >());
        }
        for (int i = atoms.size() - 1; i >= 0; --i) {
            check(i, 0, -1, -1);
        } 
    }
    void check(int stackIndex, int j, int start, int pos) {
        if (j >= atoms[stackIndex].size()) {
            results[stackIndex].push_back(make_pair(start, pos - start + 1));
        }
        if (atoms[stackIndex][j].type == 0) {
            string regexp = atoms[stackIndex][j].regexp;
            pair<int, int> nums = atoms[stackIndex][j].nums;
            string newReg = "";
            for (int i = 0; i < nums.first; ++i) newReg += regexp;
            for (int k = nums.first; k <= nums.second; ++k) {
                vector< vector<int> > spans = findall(newReg.c_str(), text.c_str());
                for (int i = 0; i < spans.size(); ++i) {
                    if (spans[i][0] == (pos + 1) && pos != -1)
                        check(stackIndex, j + 1, start, spans[i][1]);
                    if (pos == -1) {
                        check(stackIndex, j + 1, spans[i][0], spans[i][1]); 
                    }
                }
                newReg += regexp;
            } 
        }
        if (atoms[stackIndex][j].type == 1) {
            string viewName = atoms[stackIndex][j].viewName;
            string colName = columns[viewName]; 
            View view = sourceViews[viewName];
            vector<Token> val = view.groups[view.colIndex[colName]];
            pair<int, int> nums = atoms[stackIndex][j].nums;
            vector< vector< pair<int, int> > > availables; 
            for (int i = nums.first; i < nums.second; ++i)
                availables.push_back(vector< pair<int, int> >());
            for (int i = 0; i < val.size(); ++i) {
                int start = val[i].position;
                int end = val[i].position + val[i].content.length();
                int j;
                for (j = 2; j <= nums.first; ++i) {
                    if (val[i + j - 1].position == end)
                        end = val[i + j - 1].position + val[i + j -1].position + val[i + j - 1].content.length();
                    else
                        break;
                } 
                if (j == nums.first + 1) {
                    for (j = nums.first; j <= nums.second; ++j) {
                        availables[j - nums.first].push_back(make_pair(start, end));
                        if (val[i + j].position == end) {
                            end = val[i + j - 1].position + val[i + j - 1].content.length();
                        }
                    }
                }
            }
            for (int i = 0; i < nums.second - nums.first + 1; ++i) {
                for (int j = 0 ; j = availables[i].size(); ++j) {
                    if (availables[i][j].first == pos && pos != -1) {
                        check(stackIndex, j + 1, start, availables[i][j].second);
                    }
                    if (val[i].position == -1) {
                        check(stackIndex, j + 1, availables[i][j].first, availables[i][j].second);
                    }
                } 
            }
        }
        if (atoms[stackIndex][j].type == 2) {
            int from = -1; 
            int bot = atoms[stackIndex][j].nums.first;
            int top = atoms[stackIndex][j].nums.second;
            int i;
            for (i = 0; i < document.size() - bot; ++i) {
                if (document[i].position > pos && pos != -1) {
                    from = document[i].position;
                    break;
                }
                if (pos == -1) {
                    from = document[i].position;
                    break;
                }
            }
            --i;
            if (from != -1) {
                if (start == -1) {
                    for (int k = bot; k < top; ++k) {
                        if (i + k < document.size()) {
                            check(stackIndex, j + 1, from, document[i + k].position
                            + document[i + k].content.length());
                        }
                    }
                } else {
                    for (int k = bot; k < top; ++k) {
                        if (i + k < document.size()) {
                            check(stackIndex, j + 1, start, document[i + k].position
                            + document[i + k].content.length());
                        }
                    }
                }
            }
        }   
        if (atoms[stackIndex][j].type == 3) {
            int paren = atoms[stackIndex][j].parenNum;
            vector< pair<int, int > > result = results[paren];
            for (int i = 0; i < result.size(); ++i) {
                if (result[i].first == pos + 1 && pos != -1) {
                    check(stackIndex, j+ 1, start, result[i].second);
                }
                if (pos == -1) {
                    check(stackIndex, j + 1, result[i].first, result[i].second);
                }
            }
        }
    }

    //匹配方法,参数为View的别名和其名字的map和匹配的文件名
};
