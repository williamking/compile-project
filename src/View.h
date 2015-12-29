#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "Tokenizer.h"

using namespace std;

class View {

public:
	// Default Constructor
	View() {
		col_id = 0;
		view_name = "";
	}

	View(string view_name) {
		col_id = 0;
		this->view_name = view_name;
	}

    //创建列
    void createColumn(string name) {
    	if (col_id_map.count(name) > 0) {
    		// Column already exists.
    		cerr << "Error: Column name already exists." << endl;
    		exit(0);
    	} else {
    		// Column doesn't exists.
    		col_id_map.insert(pair<string, int>(name, col_id));
    		col_id++;
    		// Define temp vector of the column
    		std::vector<Token> v;

    		groups.push_back(v);
    	}
    }

    //往列中插入元素
    void insert(string name, Token token) {
    	if (col_id_map.count(name) <= 0) {
    		cout << endl;
    		cerr << "Insert Error: Column Name: \"" << name << "\" Doesn't Exist!" << endl;
    		cerr << "Program Exits." << endl;
    		cout << endl;
    		exit(0);
    	} else {
    		int id = col_id_map.find(name)->second;
    		groups.at(id).push_back(token);
    	}
    }
    
    int getColNumber() {
        return groups.size();
    }

    //输出View
    void output() {
    	int col_numbers = groups.size();
    	int row_numbers = 0;

		cout << "View: " + view_name << endl;
		cout.setf(ios::left);
		vector<int> width;

		// Contruct Vector width.
		for (int i = 0; i < col_numbers; i++) {
			int max = 0;
			if (groups[i].size() != 0) {
				Token temp = groups[i][0];
				string content = getFullContent(temp);
				int max = content.size();
			}
			for (int j = 0; j < groups[i].size(); j++) {
				if (row_numbers < groups[i].size()) {
					row_numbers = groups[i].size();
				}

				Token temp = groups[i][j];
				string content = getFullContent(temp);
				int temp_size = content.size();
				if (temp_size > max) {
					max = temp_size;
				}
			}
			max = (getColNameById(i).size() > max) ? getColNameById(i).size() : max;
			width.push_back(max);
		}

		if (col_numbers == 0) {
			cout << "[No Column Found. Please Create A Column]" << endl;
			return;
		}

		// Cout View Header.
		for (int i = 0; i < col_numbers; i++) {
			cout << '+';
			for (int j = 0; j < width[i] + 2; j++) {
				cout << '-';
			}
		}
		cout << '+' << endl;
		
		for (int i = 0; i < col_numbers; i++) {
			cout << "| ";
			cout << getColNameById(i);
			int padding = width[i] - getColNameById(i).size();
			int j = 0;
			while (j < padding) {
				cout << ' ';
				j++;
			}

			cout << " ";
		}
		cout << "|" << endl;

		for (int i = 0; i < col_numbers; i++) {
			cout << '+';
			for (int j = 0; j < width[i] + 2; j++) {
				cout << '-';
			}
		}
		cout << '+' << endl;

		if (row_numbers == 0) {
			cout << "[Empty Set]" << endl;
			return;
		}
		// 按行列遍历输出
		for (int row_id = 0; row_id < row_numbers; row_id++) {
			for (int i = 0; i < col_numbers; i++) {
				string content = "";
				//cout << row_id << endl;
				if (row_id < groups[i].size()) {
					content = getFullContent(groups[i][row_id]);
				} else {
                    cout << groups[i].size() << endl;
                    cout << i << endl;
                    Token pp = groups[i].back();
					int s = getFullContent( groups[i].back() ).size();
					int j = 0;
					while (j < s) {
						content += " ";
						j++;
					}
				}
				cout << "| ";
				cout << content;
				int padding = width[i] - content.size();
				int j = 0;
				while (j < padding) {
					cout << ' ';
					j++;
				}
				cout << " ";
			}
			cout << "|" << endl;
		}

		for (int i = 0; i < col_numbers; i++) {
			cout << '+';
			for (int j = 0; j < width[i] + 2; j++) {
				cout << '-';
			}
		}
		cout << '+' << endl;
		cout << row_numbers << " rows in set" << endl << endl;
    }

    // 根据列名返回列
    vector<Token>& getColByName(string name) {
    	if (col_id_map.count(name) <= 0) {
    		cout << endl;
    		cerr << "Query Error: Column Name: \"" << name << "\" Doesn't Exist in View: " << view_name << '!' << endl;
    		cerr << "Program Exits." << endl;
    		cout << endl;
    		exit(0);
    	} else {
    		int id = col_id_map.find(name)->second;
    		return groups[id];
    	}
    }

    string getColNameById(int id) {
    	map<string, int>::iterator iter;
    	for (iter = col_id_map.begin(); iter != col_id_map.end(); iter++) {
    		if (iter->second == id) {
    			return iter->first;
    		}
    	}
    }

private:
	//保存View中的列,列中元素为符合条件的字符串
    vector< vector <Token> > groups;

    //列与其别名的匹配
    map<string, int> col_id_map;

    // 列号
    int col_id;

    // View name
    string view_name;

    string getFullContent(Token T) {
    	if (T.content.length() == 0) return "Empty";
    	for (int i = 0; i < T.content.length(); ++i)
    	   if (T.content[i] == '\n') T.content[i] = ' ';
    	int startP = T.position;
		int endP = T.position + T.content.size();
		stringstream ss;
		string s_startP = int2string(startP);
		string s_endP = int2string(endP);

		string postfix = ":(" + s_startP + ',' + s_endP + ')';
		string content = T.content + postfix;
		return content;
    }

    string int2string(int i) {
    	stringstream ss;
    	string s;
    	ss << i;
    	ss >> s;
    	return s;
    }

};

#endif
