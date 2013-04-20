/*
 * recordListImp.cpp
 *
 *      Author: coreykruger
 */
#include <iostream>
#include <string>
#include "recordList.h"
#include "record.h"
#include "miscData.h"
using namespace std;

//The node struct is the container for data objects in the tree structure.
struct node {
  int index; //The index indicates the nodes place in the list.
	record* info; //info points to the record object contained by this node.
	node* right; //right points to this node's right child node in the tree.
	node* left; //left points to this node's left child node in the tree.
};

//recordList constructor
recordList::recordList() {
	//'start' is a pointer to the first item in the list (the root of the tree structure).
	start = NULL;
	length = 0;
	order = HEAD;
}

//Overloaded recordList constructor which sets the list's name/ID
recordList::recordList(string name) {
	//Here the identifying name of the list is set.
	setName(name);
	start = NULL;
	length = 0;
	order = HEAD;
}

//'deleteList' function prototype
void deleteListRec(node* tree);

void recordList::deleteListTree() {
	deleteListRec(start);
}

//Destructor calls 'deleteList' to recursively delete every node in the tree.
recordList::~recordList() {
	deleteListTree();
}

//'deleteList' is a recursive function to delete every node in a tree structure.
void deleteListRec(node* tree) {
	if (tree != NULL) {
		//Delete the left subtree.
		deleteListRec(tree->left);
		//Delete the right subtree.
		deleteListRec(tree->right);
		//Delete the current node.
		delete tree;
	}
}

//sets the name of the list.
void recordList::setName(string name) {
	ID = name;
}


/*toggleDebug() toggles the debug option. When the data member 'debug' is true,
selected class functions are overloaded by debugging versions- which are laced with
print statements and helpful info like pointer addresses.*/
void recordList::toggleDebug() {
	if (debug == 0)
		debug = 1;
	else debug = 0;
}

bool recordList::getDebug() {
	return debug;
}



//BEGIN TREE INDEX/////////////////////////////////////////////
//These functions reset the index-member of each node-struct in a newly created list.

//index function prototype
void index(node* tree, int& i);

void recordList::indexTree() {
	int i = 0;
	//The start node is inserted to begin the recursive process.
	index(start, i);
}

//This function uses recursion to iterate through all the nodes in the tree.
void index(node* tree, int& i) {
	if (tree != NULL) {
		//Index the left sub-tree
		index(tree->left, i);
		tree->index = i;
		i++;
		//Index the right sub-tree
		index(tree->right, i);
	}
}
//
//END TREE INDEX/////////////////////////////////////////////



//BEGIN TREE LENGTH/////////////////////////////////////////////
//These functions calculate and set the length of the tree structure.

//TreeLength function prototype
void TreeLength(node* tree, int& len);

int recordList::calcTreeLength(node* tree) {
	int i = 0;
	TreeLength(tree, i);
	return i;
}

//Recursive calculation of length.
void TreeLength(node* tree, int& len) {
	if (tree != NULL) {
		len++;
		//Process the left sub-tree
		TreeLength(tree->left, len);
		//Process the right sub-tree
		TreeLength(tree->right, len);
	}
}

//
//END TREE LENGTH/////////////////////////////////////////////



//BEGIN TREE INSERTION/////////////////////////////////////////////
//
//'insertStandard' function prototype
void insertStandard(node*& root, record& rec, itemType order);

//'InsertDebug' function prototype
void InsertDebug(node*& root, record& rec, itemType order);

void recordList::treeInsert(record& rec) {
	//The debug option is checked first.
	if (!debug)
		insertStandard(start, rec, getOrder());
	else InsertDebug(start, rec, getOrder());

	//After the new node is inserted, the tree is indexed/re-indexed.
	indexTree();
	//The 'length' member is reset after each insertion.
	length = calcTreeLength(start);
}

void insertStandard(node*& root, record& rec, itemType order) {
	/*This is a recursive insertion. When the given 'root' node is 'NULL',
	 the desired position in the tree has been reached. Otherwise, the
	 left and right subtrees are searched for the desired location.*/

	if (root == NULL) {

		//A new node is created to store the information in the given record.
		node* newNode = new node;
		//A new record is created to contain the information in the referenced record 'rec'.
		record* newRecord = new record;
		//The contents of 'newRecord' are set.
		*newRecord = rec;
		//The data member 'info' for 'newNode' is set.
		newNode->info = newRecord;
		//Right and left pointers are initialized to 'NULL'.
		newNode->right = NULL;
		newNode->left = NULL;
		//The new node is placed at the current position in the tree.
		root = newNode;

	} else {
		switch (rec.Comparison(*(root->info), order)) {
		//Search the left sub-tree
		case LESSER: insertStandard(root->left, rec, order);
			break;
		//Search the right sub-tree
		case GREATER: insertStandard(root->right, rec, order);
			break;
		/*If the given node is classified 'EQUAL' to the current position,
		 * it is inserted just to the left of the current position in the tree.*/
		case EQUAL: insertStandard(root->left, rec, order);
			break;

		}
	}
}

void InsertDebug(node*& root, record& rec, itemType order) {
	/*'InsertDebug' is identical to 'insertStandard' except that it includes debugging print statements.
	 * See the function above for specific info on how the function works.*/

	if (root == NULL) {
		node* newNode = new node;
		record* newRecord = new record;
		*newRecord = rec;
		newNode->info = newRecord;
		newNode->right = NULL;
		newNode->left = NULL;
		root = newNode;

		//This is currently the only extra print statement.
		cout << newNode->info->getHeader() << " inserted at " << newNode << endl;
	} else {
		switch (rec.Comparison(*(root->info), order)) {
		case LESSER: InsertDebug(root->left, rec, order);
			break;
		case GREATER: InsertDebug(root->right, rec, order);
			break;
		case EQUAL: InsertDebug(root->left, rec, order);
			break;
		}
	}
}
//
//END TREE INSERTION/////////////////////////////////////////////



//BEGIN TREE DELETE/////////////////////////////////////////////
//
//'getPrevNode' function prototype
void getPrevNode(node*, record&);

//'deleteLoc' function prototype
void deleteLoc(node*&, record, itemType order);

void recordList::treeDeleteNode(record rec) {
	//'deleteLoc' locates the given record in the tree for deletion.
	deleteLoc(start, rec, order);
	//The tree is re-indexed after a deletion.
	indexTree();
	//The length of the tree is re-calculated after a deletion.
	length = calcTreeLength(start);
}

//'deleteLocNode' deletes the node which contains the desired record.
void deleteLocNode(node*& root, itemType order) {
	record pred;
	node* tempRec;
	tempRec = root;

	if (root->left == NULL) {
		//Current position is simply replaced by right child node.
		root = root->right;
		delete tempRec;

	} else if (root->right == NULL) {
		//Current position is replaced by left child node.
		root = root->left;
		delete tempRec;

	} else {
		/*If the left and right child positions are both occupied, the current node is replaced by its
		  logical predecessor in the tree by calling 'getPrevNode'.*/
	    getPrevNode(root->left, pred);
	    root->info = &pred;
	    /*After the replacement, the logical predecessor is deleted, since its contents have been copied
	      to the current position in the tree.*/
	    deleteLoc(root->left, pred, order);
	}
}

//'deleteLoc' recursively locates the desired record for deletion.
void deleteLoc(node*& root, record rec, itemType order) {
	if (root == NULL)
		//If the node could not be located in the tree.
		cerr << "Node could not be LOCATED." << endl << "Node : '" << rec.getHeader() << "' was not DELETED." << endl;
	else if (rec.Comparison(*(root->info), order) == LESSER)
		// Look in left subtree.
		deleteLoc(root->left, rec, order);
	else if (rec.Comparison(*(root->info), order) == GREATER)
		// Look in right subtree.
		deleteLoc(root->right, rec, order);
	else if (rec.Comparison(*(root->info), order) == EQUAL)
		//If the record is found, the node in which it resides is passed to 'deleteLocNode' for deletion.
		deleteLocNode(root, order);
}

/*'getPrevNode' follows the simple recursive path to the logical predecessor of the given node (root).
 * The path goes like this: Find the right-most/lower-most node in the root's left subtree.*/
void getPrevNode(node* root, record& rec) {
	//If the current node is 'NULL', then the end of the branch has been reached.
	if (root->right != NULL) {
		//Check the right child until the end of the branch is reached.
		getPrevNode(root->right, rec);
	} else {
		//The Predecessor has been found.
		rec = *(root->info);
	}
}
//
//END TREE DELETE/////////////////////////////////////////////



//BEGIN TREE PRINT/////////////////////////////////////////////
//
//'printStandard' function prototype
void printStandard(node* tree);

//'printInfo' function prototype
void printInfo(node* tree);

//This is a basic printing function for convenience.
void recordList::printTree() {
	//cout << endl << "VVVVVV" << ID << endl;
	//The debug option is checked first.
	if (!debug)
		printStandard(start);//Print without extra info.
	else
		printInfo(start);//Print WITH extra info.
	//cout << "^^^^^^" << ID << endl;
}

//'printStandard' prints the header of each item in the list recursively.
void printStandard(node* tree) {
	//If 'tree' is 'NULL', then the end of the current branch has been reached.
	if (tree != NULL) {
		//Print left subtree.
		printStandard(tree->left);
		cout << tree->info->getHeader() << "-   -";
		cout << tree->info->getBody() << "-   -";
		cout << tree->info->getColor() << "-   -";
		cout << tree->info->getSize() << endl;
		//Print right subtree.
		printStandard(tree->right);
	}
}

//'printInfo' prints the header of each item in the list recursively- as well as addressing info to help chase down bugs.
void printInfo(node* tree) {
	//If 'tree' is 'NULL', then the end of the current branch has been reached.
	if (tree != NULL) {
		//Print left subtree.
		printInfo(tree->left);
		cout << "CURRENT_ADDRESS: " << tree << "   INDEX: " << tree->index << "   HEADER:   -";
		cout << tree->info->getHeader() << "-   LEFT_ADR: " << tree->left << "   RIGHT_ADR: " << tree->right << endl;
		//Print right subtree.
		printInfo(tree->right);
	}
}
//
//END TREE PRINT/////////////////////////////////////////////



//Returns the number of items in the list.
int recordList::getlength() {
	return length;
}

//Checks if the list is empty
bool recordList::isEmpty() {
	if (start == NULL)
		//If the start node is 'NULL', the list must be empty.
		return true;
	else return false;
}



//BEGIN TREE RETRIEVE BY INDEX /////////////////////////////////////////////
/*These functions are a basic utility for retrieving a node and its record according to the node's index data member.
It's intended use is inside a loop, which iterates through the whole list returning each record in order. These
functions are not really intended to be used for retrieving a singular item from the list. Although the retrieval
of a single item is certainly allowed, it's not very useful*/

//'iterateNodes' function prototype
void iterateNodes(node* tree, int index, record& rec);

void recordList::returnRecords(int index, record& rec) {
	if (index > getlength())
		//If the requested index is out of range, an error is reported.
		cerr << "\n---\nIndex out of range." << endl << "In tree structure : '" << ID << "'" << endl;
	else iterateNodes(start, index, rec);
}

/*This function recursively searches the tree until the node with the matching index (named 'index') is found.
When the desired node is found, the contents of its 'info' data member are copied to the referenced record named 'rec'.*/
void iterateNodes(node* tree, int index, record& rec) {
	if (tree != NULL) {
		//Search left subtree.
		iterateNodes(tree->left, index, rec);
		if (tree->index == index) {
			//If the index matches the current node (named 'root'), the data member 'info' is copied to 'rec'.
			rec = *(tree->info);
		}
		//Search right subtree.
		iterateNodes(tree->right, index, rec);
	}
}
//
//END TREE RETRIEVE BY INDEX/////////////////////////////////////////////



//BEGIN TREE RETRIEVE/////////////////////////////////////////////
//The following functions simply search the tree for a given record, copy that record, and return a pointer to the copy.

//'retrieve' function prototype
void retrieve(node*&, record, record*, itemType);


//This function creates the new record, returns the pointer (with copied contents) if the record is located.
record* recordList::retrieveRecord(record rec) {
	//The record named 'rec' is the record being searched. 'newRec' is the new record pointer to be returned upon success.
	record* newRec = new record;

	try {
		//The recursive function 'retrieve' is first called from the 'start' data-member.
		retrieve(start, rec, newRec, order);

		//If successful, 'newRec' is returned.
		return newRec;
	} catch (const char* NonexistentNode) {
		//If 'retrieve' is unsuccessful, see error messages below.
		cerr << "\n---\n" << NonexistentNode << endl;
		cerr << "In tree structure : '" << ID << "' \nretrieving record named: '" << rec.getHeader() << "'" << endl;

		//'newRec' is returned 'NULL'.
		return newRec;
	}

}

/*'retrieve' recursively searches the tree for a record named 'rec'. If a node containing such a record is
found, the contents of the node's 'info' data member are copied to the record pointer named 'newRec' and the recursion
is stopped.*/
void retrieve(node*& root, record rec, record* newRec, itemType order) {
	if (root == NULL) {
		//Error message to be thrown if a record named 'rec' is not present in the tree.
		throw "WARNING : Record could not be LOCATED and was not RETRIEVED";

	} else if (rec.Comparison(*(root->info), order) == LESSER)
		//Search the left subtree.
		retrieve(root->left, rec, newRec, order);

	else if (rec.Comparison(*(root->info), order) == GREATER)
		//Search the right subtree.
		retrieve(root->right, rec, newRec, order);

	else if (rec.Comparison(*(root->info), order) == EQUAL)
		//Found!
		*(newRec) = *(root->info);
}
//
//END TREE RETRIEVE/////////////////////////////////////////////

void recordList::setOrder(itemType item) {
	order = item;
}

itemType recordList::getOrder() {
	return order;
}
