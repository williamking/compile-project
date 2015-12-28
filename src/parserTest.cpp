#include "Parser.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <dirent.h>

using namespace std;

int main(int argc, char *argv[]) {
	//cout << "Input the AQL file name and the Article filename" << endl;
	string aql, art;
    struct dirent *ptr;
    DIR *dir;
    aql = argv[1];
	art = argv[2];
	//aql = "test.aql";
	//art = "test.txt";

    dir = opendir(art.c_str());
    if (dir != NULL) {
        while ((ptr=readdir(dir)) != NULL) {
            if (ptr->d_name[0] == '.') continue;
            string dataName(ptr->d_name);
            cout << "Processing " << dataName << endl;
            dataName = art + '/' + dataName;
            Parser parser(aql, dataName);
            parser.exec();
        }
    } else {
	    Parser parser(aql, art);
	    parser.exec();
    }
	return 0;
}
