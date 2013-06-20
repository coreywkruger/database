/*
 * databaseImp.cpp
 *
 *  Created on: Apr 15, 2013
 *      Author: coreykruger
 */
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "databaseHeader.h"
using namespace std;


database::database(string name) {
	ID = name;
	length = 10;
	tags.tag_1 = "entry";
	tags.tag_2 = "head";
	tags.tag_3 = "body";
	tags.tag_4 = "color";
	tags.tag_5 = "size";
}

database::~database() {
	data = NULL;
}

int database::getLength() {
	return length;
}

void database::openFile(string fileName) {
	fstream file;
	file.open(fileName.c_str(), ios::in);
	data = makeTree(file, length);
	file.close();
}

int find(fstream& file, int start, int end, string tag) {
	char* line = new char [tag.length()];
	int i;

	for (i = start; i < end; i++) {
		file.seekg(i, ios::beg);
		file.read(line, tag.length());
		if (line == tag) {
			break;
		} else {
			continue;
		}
	}
	return i;
}

string database::grab(fstream& file, string tag1, string tag2, int count, int bound) {

	int EndFile = bound;
	int temp;
	int BeginTgtLevel = 0;

	int k = 0;
	while (k != count) {
		temp = find(file, BeginTgtLevel, EndFile, "<" + tag1 + ">") + tag1.length() + 2;
		BeginTgtLevel = temp;
		k++;
	}

	int EndTgtLevel = find(file, BeginTgtLevel, EndFile, "</" + tag1 + ">");
	int BeginSubLevel = find(file, BeginTgtLevel, EndTgtLevel, "<" + tag2 + ">") + tag2.length() + 2;
	int EndSubLevel = find(file, BeginSubLevel, EndTgtLevel, "</" + tag2 + ">");

	if (BeginSubLevel >= EndFile) {
		return "NULL";
	} else {
		file.seekg(BeginSubLevel, file.beg);
		char* output = new char [EndSubLevel - BeginSubLevel];
		file.read(output, EndSubLevel - BeginSubLevel);
		return string(output);
	}

}

recordList* database::makeTree(fstream& file, int numrecs) {

	file.seekg(0, file.end);
	int EndFile = file.tellg();
	recordList* newtree = new recordList;

	int i = 1;
	while (i < numrecs) {
		record* temp = new record;

		string hea = grab(file, tags.tag_1, tags.tag_2, i, EndFile);
		string bod = grab(file, tags.tag_1, tags.tag_3, i, EndFile);
		string col = grab(file, tags.tag_1, tags.tag_4, i, EndFile);
		string siz = grab(file, tags.tag_1, tags.tag_5, i, EndFile);

		if (hea != "NULL") { temp->setHeader(hea); }
		if (bod != "NULL") { temp->setBody(bod); }
		if (col != "NULL") { temp->setColor(atof(col.c_str())); }
		if (siz != "NULL") { temp->setSize(atof(siz.c_str())); }


		if (hea == "NULL" && bod == "NULL" && col == "NULL" && siz == "NULL") {
			break;
		}
		i++;
		newtree->treeInsert(*temp);

	}
	return newtree;
	}

void database::write(string fileName) {

	ofstream file;
	file.open(fileName.c_str(), ios::out);

	int numEntries = data->getlength();
	int i;
	for (i = 0; i < numEntries; i++) {
		record temp;
		data->returnRecords(i, temp);
		file << "<" << tags.tag_1 << ">";
		file << "<" << tags.tag_2 << ">" << temp.getHeader() << "</" << tags.tag_2 << ">";
		file << "<" << tags.tag_3 << ">" << temp.getBody() << "</" << tags.tag_3 << ">";
		file << "<" << tags.tag_4 << ">" << temp.getColor() << "</" << tags.tag_4 << ">";
		file << "<" << tags.tag_5 << ">" << temp.getSize() << "</" << tags.tag_5 << ">";
		file << "</" << tags.tag_1 << ">";
	}
	file.close();
}

void database::addRecord(record rec) {
	data->treeInsert(rec);
}

void database::deleteRecord(record rec) {
	data->treeDeleteNode(rec);
}

void database::print() {
	cout << "---------------------------start - " << ID << endl;
	data->printTree();
	cout << "---------------------------end - " << ID << endl;
}
