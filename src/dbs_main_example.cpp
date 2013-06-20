/*
 * database1.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: coreykruger
 */
#include <iostream>
#include <string>
#include "databaseHeader.h"
using namespace std;

int main() {

	/////////////////////////////////////////
	//Some new database objects are created for testing below.
	//The order of operations is not carried out in any sequence.
	//During construction, this non-order demonstrated that new database objects were distinct,
	//and memory or file sources were not accidentally shared among them (this had been a problem at one point).

	database db1("first");
	db1.openFile("data/one.dat");
	db1.print();

	cout << endl;


	database db4("fourth");
	db4.openFile("data/four.dat");
	db4.print();

	cout << endl;


	database db3("third");
	db3.openFile("data/three.dat");
	db3.print();

	cout << endl;


	/////////////////////////////////////////
	//Here, db4 writes to two.dat. db2 opens two.dat and prints the now sorted data from four.dat.
	db4.write("data/two.dat");

	database db2("second");
	db2.openFile("data/two.dat");
	db2.print();

	cout << endl;


	/////////////////////////////////////////
	//db5 opens five.dat, which consists of sorted data from one.dat. The content is printed.
	db1.write("data/five.dat");

	database db5("fifth");
	db5.openFile("data/five.dat");
	db5.print();

	cout << endl;


	/////////////////////////////////////////
	//db6 opens six.dat for testing below.
	database db6("sixth");
	db6.openFile("data/six.dat");
	db6.print();

	cout << endl;


	/////////////////////////////////////////
	//A new record is inserted into db6.
	record rec1;
	rec1.setHeader("New Record");
	rec1.setBody("I am new.");
	db6.addRecord(rec1);
	cout << "New record has been added:" << endl;
	db6.print();

	cout << endl;


	/////////////////////////////////////////
	//The updated version of six.dat is saved to seven.dat.
	db6.write("data/seven.dat");

	/////////////////////////////////////////
	//seven .dat is opened for testing below.
	database db7("seventh");
	db7.openFile("data/seven.dat");
	db7.print();

	cout << endl;


	/////////////////////////////////////////
	//seven.dat consists of sorted and updated content from six.dat. Now, an existing record is deleted.
	record rec2;
	rec2.setHeader("Delete Me");
	db7.deleteRecord(rec2);
	cout << "record has been deleted:" << endl;
	db7.print();

	cout << endl << endl;


	/////////////////////////////////////////
	//seven.dat is updated after the deletion.
	db7.write("data/seven.dat");


	/////////////////////////////////////////
	//db8 opens seven.dat and prints the updated information (now that a record has been deleted).
	database db8 = database("eighth");
	db8.openFile("data/seven.dat");
	db8.print();

	cout << endl << "___END___" << endl;

	return 0;
}
