/*
 * databaseHeader.h
 *
 *  Created on: Apr 15, 2013
 *      Author: coreykruger
 */

#include <string>
#include "recordList.h"
#include "record.h"
using namespace std;

//Arbitrary tags are defined here.
struct tagList {
	string tag_1;
	string tag_2;
	string tag_3;
	string tag_4;
	string tag_5;
};

class database {
public:
	database(string name);
	~database();
	int getLength();
	void openFile(string file);
	recordList* makeTree(fstream& file, int numrecs);
	void get(string head);
	string grab(fstream& file, string tag1, string tag2, int count, int bound);
	void write(string fileName);
	void addRecord(record rec);
	void deleteRecord(record rec);
	void print();
private:
	recordList* data;
	string ID;
	int length;
	tagList tags;
};
