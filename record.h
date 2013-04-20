/*
 * record.h
 *
 *      Author: coreykruger
 */

#include <string>
#include "miscData.h"
using namespace std;

//This data type is for alphabetic comparisons that take place between record data members.

#ifndef RECORD_H_
#define RECORD_H_

class record {
public:
  record();
	compRelation Comparison(record rec, itemType item);

	//For setting data members.
	void setHeader(string);
	void setBody(string);
	void setColor(int);
	void setSize(int);

	//For getting data members.
	string getHeader();
	string getBody();
	int getColor();
	int getSize();

private:

	//Data contained in each record.
	string head;
	string body;
	int color;
	int size;
};
#endif
