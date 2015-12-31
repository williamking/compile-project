#include "Parser.h"
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

int main() {
	//cout << "Input the AQL file name and the Article filename" << endl;
	string aql, art;
    aql = "PerLoc.aql";
	art = "data/PerLoc.input";
	//aql = "test.aql";
	//art = "test.txt";
	Parser parser(aql, art);
	parser.exec();
	return 0;
}
