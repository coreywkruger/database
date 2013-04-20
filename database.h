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

//static const string tagArray[5] = {"entry", "head", "body", "color", "size"};

class tagList {
public:
  tagList(string t0, string t1, string t2, string t3, string t4) {
		tagArray[0] = t0;
		tagArray[1] = t1;
		tagArray[2] = t2;
		tagArray[3] = t3;
		tagArray[4] = t4;
		length = 5;
	}
	void setTags(string t0, string t1, string t2, string t3, string t4) {
		tagArray[0] = t0;
		tagArray[1] = t1;
		tagArray[2] = t2;
		tagArray[3] = t3;
		tagArray[4] = t4;
	}
	string getTag(int index) {
		return tagArray[index];
	}
	int getLength() {
		return length;
	}
private:
	string tagArray[5];
	int length;
};



class database {
public:
	//Constructor/Destructor
	database();
	database(string name);
	~database();
	int getLength();
	void openFile(string file);
	void openFile(string file, int MaxNumRecs);
	void makeTree(fstream& fileName, int numrecs);
	//void get(string head);
	string exractText(fstream& file, string tag1, string tag2, int count);
	void write(string fileName);
	void addRecord(record rec);
	void deleteRecord(record rec);
	void print();
	void toggleDebug();
	bool getDebug();
private:
	recordList* data;
	string ID;
	int length;
	tagList* tags;
};
