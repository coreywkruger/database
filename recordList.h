/*
 * recordList.h
 *
 *      Author: coreykruger
 */
#include <string>
#include "miscData.h"
using namespace std;

//'record' class prototype
class record;
//'node' struct prototype
struct node;

class recordList {
public:
	//Constructors/Destructors:
	recordList();
	recordList(string name);
	~recordList();

	//Public functions:
	void deleteListTree();
	void setName(string name);
	void printTree();
	void toggleDebug();
	bool getDebug();
	void returnRecords(int, record&);
	int getlength();
	void treeDeleteNode(record);
	void treeInsert(record&);
	record* retrieveRecord(record);
	void setOrder(itemType item);
	itemType getOrder();

private:
	//Private utilities:
	int calcTreeLength(node*);
	void indexTree();
	bool isEmpty();

	//Private data members:
	bool debug; //A debug flag for print-statement helps.
	node* start; //A pointer to the first item in the list (the root of the tree structure).
	string ID; //ID is the identifying name of the class instance.
	int length; //The number of items in the list.
	itemType order;
};
