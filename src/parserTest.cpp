#include "Parser.h"
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;a

int main(int argc, char *argv[]) {
	//cout << "Input the AQL file name and the Article filename" << endl;
	string aql, art;
    aql = argv[1];
	art = argv[2];
	//aql = "test.aql";
	//art = "test.txt";
	Parser parser(aql, art);
	parser.exec();
	return 0;
}
