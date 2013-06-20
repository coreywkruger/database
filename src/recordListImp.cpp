/*
 * recordListImp.cpp
 *
 *  Created on: Mar 9, 2013
 *      Author: coreykruger
 */
#include <iostream>
#include <string>
#include "recordList.h"
#include "record.h"
using namespace std;

struct node {
	int index;
	record* info;
	node* right;
	node* left;
};

recordList::recordList() {
	start = NULL;
	length = 0;
}

void index(node* tree, int& i);

void recordList::indexTree() {
	int i = 0;
	index(start, i);
}

void index(node* tree, int& i) {
	if (tree != NULL) {
		index(tree->left, i);
		tree->index = i;
		i++;
		index(tree->right, i);
	}
}

void recordList::toggleDebug(bool toggle) {
	debug = toggle;
}

//BEGIN TREE LENGTH/////////////////////////////////////////////
//
void TreeLength(node* tree, int& len);

int recordList::calcTreeLength(node* tree) {
	int i = 0;
	TreeLength(tree, i);
	return i;
}

void TreeLength(node* tree, int& len) {
	if (tree != NULL) {
		len++;
		TreeLength(tree->left, len);
		TreeLength(tree->right, len);
	}
}
//
//END TREE LENGTH/////////////////////////////////////////////


//BEGIN TREE INSERTION/////////////////////////////////////////////
//
void insertStandard(node*& root, record& rec);

void insertInfo(node*& root, record& rec);

void recordList::treeInsert(record& rec) {
	if (!debug)
		insertStandard(start, rec);
	else insertInfo(start, rec);
	indexTree();
	length = calcTreeLength(start);
}

void insertStandard(node*& root, record& rec) {
	if (root == NULL) {
		node* newNode = new node;
		record* newRecord = new record;
		*newRecord = rec;
		newNode->info = newRecord;
		newNode->right = NULL;
		newNode->left = NULL;
		root = newNode;
	} else {
		switch (rec.Comparison(*(root->info))) {
		case LESSER: insertStandard(root->left, rec);
			break;
		case GREATER: insertStandard(root->right, rec);
			break;
		case EQUAL: insertStandard(root->left, rec);
			break;
		}
	}
}

void insertInfo(node*& root, record& rec) {
	if (root == NULL) {
		node* newNode = new node;
		record* newRecord = new record;
		*newRecord = rec;
		newNode->info = newRecord;
		newNode->right = NULL;
		newNode->left = NULL;
		root = newNode;
		cout << newNode->info->getHeader() << " inserted at " << newNode << endl;
	} else {
		switch (rec.Comparison(*(root->info))) {
		case LESSER: insertInfo(root->left, rec);
			break;
		case GREATER: insertInfo(root->right, rec);
			break;
		case EQUAL: insertInfo(root->left, rec);
			break;
		}
	}
}
//
//END TREE INSERTION/////////////////////////////////////////////


//BEGIN TREE DELETE/////////////////////////////////////////////
//
void getPredecessor(node*, record&);

void findNode(node*&, node*&, record);

void Delete(node*&, record);

void recordList::treeDeleteNode(record rec) {
	Delete(start, rec);
	indexTree();
	length = calcTreeLength(start);
}

void DeleteNode(node*& root) {
	record pred;
	node* tempRec;
	tempRec = root;

	if (root->left == NULL) {
		root = root->right;
		delete tempRec;
	} else if (root->right == NULL) {
		root = root->left;
		delete tempRec;
	} else {
	    getPredecessor(root->left, pred);
	    root->info = &pred;
	    Delete(root->left, pred);
	}
}

void findNode(node*& root, node*& target, record rec) {
	if (root == NULL) {
		cout << "Node could not be located." << endl;
	} else {
		switch (rec.Comparison(*(root->info))) {
		case LESSER: findNode(root->left, target, rec);
			break;
		case GREATER: findNode(root->right, target, rec);
			break;
		case EQUAL: target = root;
			break;
		}
	}
}

void Delete(node*& root, record rec) {
	if (root == NULL)
		cout << "Node could not be DELETED." << endl;
	else if (rec.Comparison(*(root->info)) == LESSER)
		Delete(root->left, rec);   // Look in left subtree.
	else if (rec.Comparison(*(root->info)) == GREATER)
		Delete(root->right, rec);  // Look in right subtree.
	else if (rec.Comparison(*(root->info)) == EQUAL)
		DeleteNode(root);
}

void getPredecessor(node* root, record& rec) {
	if (root->right != NULL) {
		getPredecessor(root->right, rec);
	} else {
		rec = *(root->info);
	}
}
//
//END TREE DELETE/////////////////////////////////////////////

//BEGIN TREE PRINT/////////////////////////////////////////////
//
void printStandard(node* tree);

void printInfo(node* tree);

void recordList::printTree() {
	if (!debug)
		printStandard(start);
	else printInfo(start);
}

void printStandard(node* tree) {
	if (tree != NULL) {
		printStandard(tree->left);
		cout << tree->info->getHeader() << " | ";
		cout << tree->info->getBody() << " | ";
		cout << tree->info->getColor() << " | ";
		cout << tree->info->getSize() << " | ";
		cout << endl;
		printStandard(tree->right);
	}
}

void printInfo(node* tree) {
	if (tree != NULL) {
		printInfo(tree->left);
		cout << tree << "   " << tree->index << "    -";//" - " << tree->right << endl;
		cout << tree->info->getHeader() << "-         right-> " << tree->right << "   left->" << tree->left << endl;
		printInfo(tree->right);
	}
}

void iterateNodes(node* tree, int index, record& rec);

void recordList::returnRecords(int index, record& rec) {
	iterateNodes(start, index, rec);
}

void iterateNodes(node* tree, int index, record& rec) {
	if (tree != NULL) {
		iterateNodes(tree->left, index, rec);
		if (tree->index == index) {
			rec = *(tree->info);
		}
		iterateNodes(tree->right, index, rec);
	}
}

//
//END TREE PRINT/////////////////////////////////////////////

int recordList::getlength() {
	return length;
}

bool recordList::isEmpty() {
	if (start == NULL)
		return true;
	else return false;
}

//BEGIN TREE RETRIEVE/////////////////////////////////////////////
//
void retrieve(node*&, record, record*);

record recordList::retrieveRecord(record rec) {
	record* newRec = new record;
	retrieve(start, rec, newRec);
	return *newRec;
}

void retrieve(node*& root, record rec, record* newRec) {
	if (root == NULL)
		cout << "\nNode could not be RETRIEVED.\n" << endl;
	else if (rec.Comparison(*(root->info)) == LESSER)
		retrieve(root->left, rec, newRec);   // Look in left subtree.
	else if (rec.Comparison(*(root->info)) == GREATER)
		retrieve(root->right, rec, newRec);  // Look in right subtree.
	else if (rec.Comparison(*(root->info)) == EQUAL)
		*(newRec) = *(root->info);
}
//
//END TREE RETRIEVE/////////////////////////////////////////////
