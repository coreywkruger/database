/*
 * recordList.h
 *
 *  Created on: Mar 9, 2013
 *      Author: coreykruger
 */
#include <string>
using namespace std;

class record;
struct node;

class recordList {
public:
	recordList();
	void printTree();
	void indexTree();
	void toggleDebug(bool toggle);
	int calcTreeLength(node*);
	void returnRecords(int, record&);
	int getlength();
	void treeDeleteNode(record);
	void treeInsert(record&);
	record retrieveRecord(record);
private:
	bool isEmpty();
	bool debug;
	node* start;
	int length;
};
