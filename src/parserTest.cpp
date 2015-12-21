#include "Parser.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	cout << "Input the AQL file name and the Article filename";
	string aql, art;
	cin >> aql, art;
	Parser parser(aql, art);
	parser.aql_stmt();
}