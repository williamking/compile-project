#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
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
		for (int i = 0; i < col_numbers; i++) {
			int max = 0;
			if (groups[i].size() != 0) {
				int max = groups[i][0].content.size();
			}
			for (int j = 0; j < groups[i].size(); j++) {
				if (row_numbers < groups[i].size()) {
					row_numbers = groups[i].size();
				}
				int temp = groups[i][j].content.size();
				if (temp > max) {
					max = temp;
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
				if (row_id < groups[i].size()) {
					content = groups[i][row_id].content;
				} else {
					int s = groups[i].back().content.size();
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
		cout << row_numbers << " rows in set" << endl;
    }

    // 根据列名返回列
    vector<Token>& getColByName(string name) {
    	if (col_id_map.count(name) <= 0) {
    		cout << endl;
    		cerr << "Query Error: Column Name: \"" << name << "\" Doesn't Exist!" << endl;
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

};

#endif
