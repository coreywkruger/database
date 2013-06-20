/*
 * recordImp.cpp
 *
 *  Created on: Mar 9, 2013
 *      Author: coreykruger
 */
#include <iostream>
#include <string>
#include "record.h"
using namespace std;

record::record() {
	setHeader("");
	setBody("");
	setColor(0);
	setSize(0);
}

alphaRelation alphaComp(int index, string current, string newRec) {
	if (current.empty() == true || current == newRec)
		return EQUAL;
	else if (toupper(current[index]) > toupper((newRec)[index]))
		return GREATER;
	else if (toupper(current[index]) < toupper((newRec)[index]))
		return LESSER;
	else if (toupper(current[index]) == toupper((newRec)[index]))
		return alphaComp(index + 1, current, newRec);
}

alphaRelation record::Comparison(record newRec) {
	return alphaComp(0, head, newRec.head);
}


void record::setHeader(string input) {
	head = input;
}

void record::setBody(string input) {
	body = input;
}

void record::setColor(float input) {
	color = input;
}

void record::setSize(float input) {
	size = input;
}


string record::getHeader() {
	return head;
}

string record::getBody() {
	return body;
}

float record::getColor() {
	return color;
}

float record::getSize() {
	return size;
}
