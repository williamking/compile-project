#include "Parser.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	cout << "Input the AQL file name and the Article filename" << endl;
	string aql, art;
	//cin >> aql;
	//cin >> art;
	aql = "PerLoc.aql";
	art = "PerLoc.input";
	Parser parser(aql, art);
	parser.exec();
	return 0;
}
