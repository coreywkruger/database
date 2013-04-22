/*
 * recordImp.cpp
 *
 *      Author: coreykruger
 */
#include <iostream>
#include <string>
#include "record.h"
#include "miscData.h"

record::record() {
	//Initialize all data members to empty or zero.
	setHeader("");
	setBody("");
	setColor(0);
	setSize(0);
}


/*'alphaComp' is used to alphabetically sort two strings. It recursively compares the two strings, and
 returns 'GREATER' if the first string argument ('current') is alphabetically valued greater (example : 'z' > 'a') than the
 second string argument ('newRec'). It returns 'LESSER' if the first argument is alphabetically valued lower than 'newRec'.
 The function returns 'EQUAL' if the two arguments are equal.*/
compRelation alphaComp(int index, string current, string newRec) {
	//At each recursion, characters from each string are indexed and compared.
	//In each successive recursion, the index is incremented by one, and the next two characters in each string are compared.
	//The recursion continues until two characters are encountered that are NOT equal.
	if (current.empty() == true || current == newRec)
		return EQUAL;
	else if (toupper(current[index]) > toupper((newRec)[index]))
		return GREATER;
	else if (toupper(current[index]) < toupper((newRec)[index]))
		return LESSER;
	else//s if (toupper(current[index]) == toupper((newRec)[index]))
		return alphaComp(index + 1, current, newRec);
}

compRelation numbComp(int current, int newRec) {
	if (current > newRec) {
		return GREATER;
	} else if (current < newRec) {
		return LESSER;
	} else {// if (current == newRec) {
		return EQUAL;
	}
}

compRelation record::Comparison(record newRec, itemType item) {
	//'newRec' is the record to be compared to the current record. 'head' is the local record's data member.
	if (item == HEAD) {
		return alphaComp(0, head, newRec.getHeader());
	} else if (item == BODY) {
		return alphaComp(0, body, newRec.getBody());
	} else if (item == COLOR) {
		return numbComp(color, newRec.getColor());
	} else {// if (item == SIZE) {
		return numbComp(size, newRec.getSize());
	}
}

////'alphacomp' is called from the member function 'Comparison'.
//compRelation record::Comparison(record newRec) {
//	//'newRec' is the record to be compared to the current record. 'head' is the local record's data member.
//	return alphaComp(0, head, newRec.head);
//}

///////////////////////////////////////////
//The following functions are for setting the record's data members.
void record::setHeader(string input) {
	head = input;
}

void record::setBody(string input) {
	body = input;
}

void record::setColor(int input) {
	color = input;
}

void record::setSize(int input) {
	size = input;
}
//
///////////////////////////////////////////



///////////////////////////////////////////
//The following functions return each data member.
string record::getHeader() {
	return head;
}

string record::getBody() {
	return body;
}

int record::getColor() {
	return color;
}

int record::getSize() {
	return size;
}
//
///////////////////////////////////////////
