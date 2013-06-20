/*
 * node.h
 *
 *  Created on: Mar 9, 2013
 *      Author: coreykruger
 */

#include <string>
using namespace std;

enum alphaRelation {GREATER, LESSER, EQUAL};

#ifndef RECORD_H_
#define RECORD_H_

class record {
public:
	record();
	alphaRelation Comparison(record);

	void setHeader(string);
	void setBody(string);
	void setColor(float);
	void setSize(float);

	string getHeader();
	string getBody();
	float getColor();
	float getSize();

private:
	string head;
	string body;
	float color;
	float size;
};
#endif
