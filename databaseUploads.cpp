/*
 * databaseUploads.cpp
 *
 *  Created on: April 20, 2013
 *      Author: coreykruger
 */
#include <iostream>
#include <string>
#include "databaseHeader.h"
using namespace std;

int main() {


	database DBA("my-DBA");

	DBA.openFile("data/input.dat");
	DBA.print();

	cout << endl;
	DBA.write("data/output.dat");

	cout << endl << "___END___" << endl;

	return 0;
}
