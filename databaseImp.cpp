/*
 * databaseImp.cpp
 *
 *  Created on: Apr 15, 2013
 *      Author: coreykruger
 */
#include <iostream>
#include <fstream>
#include <string>
#include "databaseHeader.h"
#include "miscData.h"
using namespace std;

//Constructor
//Initializes list of tags to identify.
database::database() {
	length = 0;
	tags = new tagList("entry", "head", "body", "color", "size");
	data = NULL;
}

//Constructor- overloaded for ID assignment.
//Initializes list of tags to identify.
//'ID' is the name the user gives to the database.
database::database(string name) {
	ID = name;
	length = 0;
	tags = new tagList("entry", "head", "body", "color", "size");
	data = NULL;
}

//Destructor deletes the list of saved record pointers as well as the list of tags.
database::~database() {
	data->deleteListTree(); //List of record objects extracted from file.
	delete tags; //Object containing tags to use.
}

//Returns the number of records contained in the database object.
int database::getLength() {
	return length; //Number of records in database.
}

/*Opens the file indicated by 'fileName'. All opening/closing happens hear. Every other appearance of
the file stream is by reference.*/
void database::openFile(string fileName) {

	//First a file stream is created and opened.
	fstream file;
	file.open(fileName.c_str(), ios::in);

	/*Next a list of records is created. This list will be contained in the data member called 'data'.
	 The member called 'data' is a pointer to a 'recordList()' object, which holds a list of pointers
	 to record objects.*/
	makeTree(file, 10); //The integer '10' is the maximum number of records to look for in the file.

	if (length == 0)
		//If no records have been extracted from file, an error message is printed.
		cerr << "No records found in file : '" << fileName << "' for database : '" << ID << "'" << endl;

	//The file is closed.
	file.close();
}

/*Overloaded 'openFile()' to include optional variable 'MaxNumRecs' which indicates the maximum number of
 records to look for in the open file.*/
void database::openFile(string fileName, int MaxNumRecs) {

	//First a file stream is created and opened.
	fstream file;
	file.open(fileName.c_str(), ios::in);

	/*Next a list of records is created. This list will be contained in the data member called 'data'.
	 The member called 'data' is a pointer to a 'recordList()' object, which holds a list of pointers
	 to record objects.*/
	makeTree(file, MaxNumRecs); //The integer 'MaxNumRecs' is the maximum number of records to look for in the file.

	if (length == 0)
		//If no records have been extracted from file, an error message is printed.
		cerr << "No records found in file : '" << fileName << "' for database : '" << ID << "'" << endl;

	//The file is closed.
	file.close();
}


/*The function 'findTag(fstream&, int, int, string)' locates the given string 'tag' and returns the index
 of its starting point.

 'file' is the file to be searched.
 'line' is the container for characters read from file.
 'start' and 'end' indicate the boundaries of the search area.
 'tag' is the string being searched for.
 */
int findTag(fstream& file, int start, int end, string tag) {
	int i = 0;
	for (i = start; i < end; i++) {

		// 'line' Holds characters read from file.
		char* line = new char;

		// The file is read a section at a time. Each section is as long as the string being sought ( 'tag' ).
		file.seekg(i, ios::beg);
		file.read(line, tag.length());

		/*If 'line' matches the tag being sought ( 'tag' ), the loop is broken, and the current position in the
		file is returned. Else, the loop is continued.*/
		if (line == tag) {
			break;
		} else {
			continue;
		}
		delete line;
	}
	//Return current position in the file.
	return i;
}



/*The function 'extractText' is used to extract a section of text from file, based on the given tags ( 'tag1' and 'tag2').

'tag1' is the used to find the location of the record currently being sought
'tag2' is used to find a particular piece of information inside that record.
'count' indicates which consecutive record is being sought (in order from beginning to end).*/
string database::exractText(fstream& file, string tag1, string tag2, int count) {

	//Get the number of characters in the files.
	file.seekg(0, ios::end);
	unsigned int EndFile = file.tellg();
	file.seekg(0, ios::beg);

	/*The loop below passes over full records in the file (as indicated by 'tag1') and stops when the
	 desired record is reached.

	 'BeginTgtLevel' indicates the position where the desired record starts.
	 'EndFile' indicates at what position the file ends.
	 'count' indicates which consecutive record is being sought (in order from beginning to end).*/
	unsigned int BeginTgtLevel = 0;
	int i = 0;
	for (i = 0; i < count; i++) {

		/*First, the position of the record is found. The length of the tag- a well as its brackets is added
		 to the position. This is so that no characters from the tag itself are extracted.*/
		BeginTgtLevel = findTag(file, BeginTgtLevel, EndFile, "<" + tag1 + ">") + tag1.length() + 2;

		//If the position of the record is beyond the end of the file, then the record does not exist.
		if (BeginTgtLevel > EndFile) {
			//An empty string is returned.
			return string("");
		} else {
			//Else, the loop continues.
			continue;
		}
	}

	/*These variables define the boundaries of the text to be extracted from the record.
	 'EndTgtLevel' defines the ending position of the record.
	 'BeginSubLevel' defines the starting position of the subsection inside the record.The length of the
	 	 tag- as well as its brackets are added to the position. This is so that no characters from the tag
	 	 itself are extracted.
	 'EndSubLevel' defines the ending position of the subsection within the record.*/
	int EndTgtLevel = findTag(file, BeginTgtLevel, EndFile, "</" + tag1 + ">");
	int BeginSubLevel = findTag(file, BeginTgtLevel, EndTgtLevel, "<" + tag2 + ">") + tag2.length() + 2;
	int EndSubLevel = findTag(file, BeginSubLevel, EndTgtLevel, "</" + tag2 + ">");

	/*If there is no information in the subsection, an empty string is returned. The distance between 'BeginSubLevel' and
	 'EndSubLevel' is used to detect this.*/
	if (BeginSubLevel == EndSubLevel){
		return string("");
	} else {
		/*If information exists to be extracted, the starting position of the subsection is moved to
		 ('BeginSubLevel') and the section of text is read from file and returned to the calling function. */
		file.seekg(BeginSubLevel, ios::beg);
		char output[EndSubLevel - BeginSubLevel];
		file.read(output, EndSubLevel - BeginSubLevel);

		//Convert to string and return.
		return string(output);
	}

}


/* 'MakeTree()' extracts the necessary bits of text from file, inserts them in newly created 'record' objects,
  and builds the 'recordList()' object to store the new records in.

  'newtree' indicates the new 'recordList()' object to store records in.
  'data' is the local data member which is a 'recordList()' object pointer. Here, it points to the new 'recordList()' object.
  'MaxNumRecs' is the maximum number of records to extract from file.
  'boundary' is a string which tests whether the end of the file has been reached.
  'extractText()' is the function which extracts the indicated bits of text from file (according to what tags are passed as arguments.).
  'tags' is a tagList()' object which contains the tags currently in use as data members.*/
void database::makeTree(fstream& file, int MaxNumRecs) {

	recordList* newtree = new recordList;

	//If 'data' is empty, it is made to point at the new 'recordList()' object.
	if (data == NULL) {
		data = newtree;
	} else {
		/*If 'data' is currently occupied, its content is deleted (by 'recordList()' object's member
		function 'deleteListTree()' which is like a destructor) and 'data' is made to point at the new 'recordList()' object.*/
		data->deleteListTree();
		data = newtree;
	}
	int i;
	for (i = 1; i < MaxNumRecs; i++) {
		record extractedRecord;

		//First the file is tested to see if the sought record exists. If the record does not exist, 'extractText()' will return an empty string.
		string boundary = exractText(file, tags->getTag(0), tags->getTag(1), i);

		//If the sought record is out of range, the loop is continued, and the current record is skipped.
		if (boundary.empty()) {
			continue;
		} else {
			//If the record is found, all desired attributes are extracted from file and initialized in the new record 'extractedRecord'.
			//The 'tagList()' object named 'tags' indicates which tags to use for extraction.
			extractedRecord.setHeader(  		exractText(file, tags->getTag(0), tags->getTag(1), i)  			);
			extractedRecord.setBody(  			exractText(file, tags->getTag(0), tags->getTag(2), i)  			);
			extractedRecord.setColor(	atol(  	exractText(file, tags->getTag(0), tags->getTag(3), i).c_str()  ));
			extractedRecord.setSize(	atol(  	exractText(file, tags->getTag(0), tags->getTag(4), i).c_str()  ));
		}
		//The extracted record is added to the current database object.
		addRecord(extractedRecord);
	}
}


/*'write()' is for writing to file.

 'fileName' indicates the path to the desired file.
 'file' is file stream opened according to 'fileName'.
 'numEntries' indicates the number of records to be written to file.
 'newRecord' is a newly created record to be written to file.
 'tags' is a tagList()' object which contains the tags currently in use as data members.
 'data' is the local data member which is a 'recordList()' object pointer. Here, it points to the database's local 'recordList()' object.
 */

void database::write(string fileName) {

	//The file indicated by 'fileName' is opened.
	fstream file;
	file.open(fileName.c_str(), ios::out);

	if (!file.is_open())
		cerr << "File '" << fileName << "' could not be opened." << endl;

	//The number of records to be written is obtained.
	int numEntries = data->getlength();
	int i;
	for (i = 0; i < numEntries; i++) {

		//New record to be wirtten to file.
		record newRecord;

		//The 'recordList()' is iterated through, and each record is retrieved according to its index 'i'.
		//the contents of each record in the 'recordList()' object are inserted into 'newRecord'.
		data->returnRecords(i, newRecord);

		//Once the record has been retrieved, its contents are written to file. 'tags' simply prints the necessary tag.
		file << "<" + tags->getTag(0) + ">" << endl;//Record's opening tag.
		file << "<" + tags->getTag(1) + ">" << newRecord.getHeader() << "</" + tags->getTag(1) + ">" << endl;
		file << "<" + tags->getTag(2) + ">" << newRecord.getBody() << "</" + tags->getTag(2) + ">" << endl;
		file << "<" + tags->getTag(3) + ">" << newRecord.getColor() << "</" + tags->getTag(3) + ">" << endl;
		file << "<" + tags->getTag(4) + ">" << newRecord.getSize() << "</" + tags->getTag(4) + ">" << endl;
		file << "</" + tags->getTag(0) + ">" << endl;//Record's closing tag.
		file << endl;
	}
	//The file is closed.
	file.close();
}

//Adds a record to the database.
void database::addRecord(record rec) {
	//Record is inserted.
	data->treeInsert(rec);
	//Length of the database is adjusted.
	length = data->getlength();
}

//Deletes the record which matches 'rec'.
void database::deleteRecord(record rec) {
	//Record is deleted.
	data->treeDeleteNode(rec);
	//Length of the database is adjusted.
	length = data->getlength();
}

void database::print() {
	cout << "START: " << ID << endl;
	data->printTree();
	cout << "END: " << ID << endl;
}

//Toggles the option to print out helpful debug information such as pointer addresses.
void database::toggleDebug() {
	data->toggleDebug();
}

//Indicates whether the database is in debug mode or not.
bool database::getDebug() {
	return data->getDebug();
}
