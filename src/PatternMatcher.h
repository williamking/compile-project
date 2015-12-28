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
    int currentAtom;
};
//patern模式中的atom,可能为任意token,可能为某View中的一列,可能为正则表达式,用type表示其类型,若为正则,则表达式在value成员的regexp成员中,若为token,则其别名及列名在value成员的column成员中

class PatternMatcher {
	private:
    vector< vector<Atom> > atoms;
    vector< vector< pair<int, int> > > results;
    vector< vector< vector< pair<int, int> > > > resultParens;
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
        int p = catchStack.top();
        catchStack.push(current + 1);
        vector<Atom> newAtom;
        atoms.push_back(newAtom);
        atoms[p].push_back(Atom(3, "", "", 0, 0, ++current));
    }
    void insertReturnColumn(int index, string colName) {
        returnList[index] = colName;
    }
    void popSubMatcher() {
        catchStack.pop();
    }
    vector< vector< pair<int, int> > > match(View &textView, map<string, View> &views) {
        sourceViews = views;
        for (int i = 0; i < atoms.size(); ++i) {
            results.push_back(vector< pair<int, int> >());
            resultParens.push_back(vector< vector< pair<int, int> > >());
        }
        for (int i = atoms.size() - 1; i >= 0; --i) {
            check(i, 0, -1, -1);
        }
        vector< vector< pair<int, int> > > filtResults;
        for (int i = 0; i < results.size(); ++i) {
            filtResults.push_back(vector< pair<int, int> >());
        }
        for (int i = 0; i < results[0].size(); ++i) {
            filtResults[0].push_back(results[0][i]);
        }
        for (int j = 0; j < resultParens[0].size(); ++j) {
            for (int k = 0; k < resultParens[0][j].size(); ++k) {
                filt(resultParens[0][j][k].first, resultParens[0][j][k].second, filtResults);
            }
        }
        return filtResults;
    }
    //��ջ�ķ�ʽ�������Ų�ι�ϵ,j��ʾ�ڼ���Atom,start��ʾ������ƥ�����ʼλ��,posΪ��ǰƥ���λ�� 
    void check(int stackIndex, int j, int start, int pos) {
        if (j >= atoms[stackIndex].size()) {
            vector< pair<int, int> > pars;
            results[stackIndex].push_back(make_pair(start, pos));
            for (int i = 0; i < atoms[stackIndex].size(); ++i) {
                if (atoms[stackIndex][i].type == 3) {
                    pars.push_back(make_pair(atoms[stackIndex][i].parenNum, atoms[stackIndex][i].currentAtom));
                }
            }
            resultParens[stackIndex].push_back(pars);
            return;
        }
        // if (atoms[stackIndex][j].type == 0) {
        //     string regexp = atoms[stackIndex][j].regexp;
        //     pair<int, int> nums = atoms[stackIndex][j].nums;
        //     string newReg = "";
        //     for (int i = 0; i < nums.first; ++i) newReg += regexp;
        //     vector<int> availablePos;
        //     availablePos.push_back(pos); 
        //     for (int k = nums.first; k <= nums.second; ++k) {
        //         vector< vector<int> > spans = findall(newReg.c_str(), text.c_str());
        //         for (int i = 0; i < spans.size(); ++i) {
        //             int ep = spans[i][1];
        //             if (spans[i][0] >= pos && pos != -1) {
        //                 bool valid = true;
        //                 for (int p = pos; p < spans[i][0]; ++p) 
        //                     if (text[p] != ' ' && text[p] != '\n') {
        //                         valid = false;
        //                         break;
        //                     }
        //                 if (valid)
        //                     check(stackIndex, j + 1, start, ep);
        //             }
        //             if (pos == -1) {
        //                 check(stackIndex, j + 1, spans[i][0], ep); 
        //             }
        //         }
        //         newReg += regexp;
        //     }
        //     return;
        // }
        if (atoms[stackIndex][j].type == 0) {
        	string regexp = atoms[stackIndex][j].regexp;
        	pair<int, int> nums = atoms[stackIndex][j].nums;
        	vector< vector<int> > spans = findall(regexp.c_str(), text.c_str());
        	for (int i = 0; i < spans.size() - nums.first + 1; ++i) {
        		int ep = pos;
        		for (int k = 0; k < nums.second; ++k) {
        			if (spans[i + k][0] >= ep || ep != -1) {
        				bool valid = true;
        				for (int p = ep; p < spans[i + k][0]; ++p) {
                            if (text[p] != ' ' && text[p] != '\n') {
                                valid = false;
                                break;
                            }        					
        				}
        				if (valid) {
        					ep = spans[i + k][1];
        					if (k >= nums.first - 1)
        						check(stackIndex, j + 1, start, ep);
        				} else {
        					break;
        				}
        			}
        			if (ep == -1) {
        				ep = spans[i + k][1];
        				if (start == -1) start = spans[i + k][0];
        				if (k >= nums.first - 1)
        					check(stackIndex, j + 1, start, ep);
        			}
        		}
        	}
        }
        if (atoms[stackIndex][j].type == 1) {
            string viewName = atoms[stackIndex][j].viewName;
            string colName = columns[viewName]; 
            View view = sourceViews[viewName];
            vector<Token> val = view.getColByName(colName);
            pair<int, int> nums = atoms[stackIndex][j].nums;
            vector< vector< pair<int, int> > > availables; 
            for (int i = nums.first; i <= nums.second; ++i)
                availables.push_back(vector< pair<int, int> >());
            for (int i = 0; i < val.size(); ++i) {
                int start = val[i].position;
                int end = val[i].position + val[i].content.length();
                int k;
                for (k = 2; k <= nums.first; ++k) {
                    if (val[i + k - 1].position == end)
                        end = val[i + k - 1].position + val[i + k -1].position + val[i + k - 1].content.length();
                    else
                        break;
                } 
                if (k == nums.first + 1) {
                    for (k = nums.first; k <= nums.second; ++k) {
                        availables[k - nums.first].push_back(make_pair(start, end));
                        if (val[i + k].position == end) {
                            end = val[i + k - 1].position + val[i + k - 1].content.length();
                        }
                    }
                }
            }
            for (int i = 0; i < nums.second - nums.first + 1; ++i) {
                for (int k = 0 ; k < availables[i].size(); ++k) {
                    int ep = availables[i][k].second;
                    if (availables[i][k].first >= pos && pos != -1) {
                        bool valid = true;
                        for (int p = pos; p < availables[i][k].first; ++p) 
                            if (text[p] != ' ' && text[p] != '\n') {
                                valid = false;
                                break;
                            }
                        if (valid)
                            check(stackIndex, j + 1, start, ep);
                    }
                    if (pos == -1) {
                        check(stackIndex, j + 1, availables[i][k].first, ep);
                    }
                } 
            }
            return;
        }
        if (atoms[stackIndex][j].type == 2) {
            int from = -1; 
            int bot = atoms[stackIndex][j].nums.first;
            int top = atoms[stackIndex][j].nums.second;
            int i;
            for (i = 0; i < document.size() - bot; ++i) {
                if (document[i + 1].position >= pos && pos != -1) {
                    from = document[i + 1].position;
                    break;
                }
                if (pos == -1) {
                    from = document[i + 1].position;
                    break;
                }
            }
            if (from != -1) {
                if (start == -1) {
                    for (int k = bot; k < top; ++k) {
                        if (i + k >= document.size()) break;
                        int ep = document[i + k].position + document[i + k].content.length();
                        //while (text[ep] == ' ') ++ep;
                        if (i + k < document.size()) {
                            check(stackIndex, j + 1, from, ep);
                        }
                    }
                } else {
                    for (int k = bot; k < top; ++k) {
                        int ep = document[i + k].position + document[i + k].content.length();
                        //while (text[ep] == ' ') ++ep;
                        if (i + k < document.size()) {
                            check(stackIndex, j + 1, start, ep);
                        }
                    }
                }
            }
            return;
        }   
        if (atoms[stackIndex][j].type == 3) {
            int paren = atoms[stackIndex][j].parenNum;
            vector< pair<int, int > > result = results[paren];
            for (int i = 0; i < result.size(); ++i) {
                int ep = result[i].second;
                if (result[i].first >= pos && pos != -1) {
                    bool valid = true;
                    for (int p = pos; p < result[i].first; ++p) 
                        if (text[p] != ' ' && text[p] != '\n') {
                            valid = false;
                            break;
                        }
                    if (valid) {
                        atoms[stackIndex][j].currentAtom = i;
                        check(stackIndex, j + 1, start, ep);
                    }       
                }
                if (pos == -1) {
                    atoms[stackIndex][j].currentAtom = i;
                    check(stackIndex, j + 1, result[i].first, ep);
                }
            }
        }
    }
    
    void filt(int parenNum, int index, vector< vector< pair<int, int> > > &filtResults) {
        filtResults[parenNum].push_back(results[parenNum][index]);
        for (int i = 0; i < resultParens[parenNum][index].size(); ++i) {
            filt(resultParens[parenNum][index][i].first, resultParens[parenNum][index][i].second, filtResults);
        }
    }

    //匹配方法,参数为View的别名和其名字的map和匹配的文件名
};
