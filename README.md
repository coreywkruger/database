C++-Database
============

A simple database class which pulls records from .dat files, and organizes/prints them from a binary tree liked-list data structure.

If the main file is run ("databaseUploads.cpp") an example is executed. Basically, the program pulls information from the folder labeled 'data'. The inputs come from the file labeled "input.dat". This file contains text which is organized into entries by a set of tags. The entries are listed in no particular order, and the order of the tag-separated portions of each entry is random. 

A basic read-write operation is done like so:

  database DBA("my-DBA");
  
  DBA.openFile("data/input.dat");
  DBA.print();
  
  cout << endl;
  DBA.write("data/output.dat");


When a database object is created and its openFile() member function is called, information is extracted from the entries in the file (indicated by the given path) and are inserted into a sorted binary tree liked-list. The tree is contained as a private data member of the database object. In this instance, the tree is sorted alphabetically by default according to what is contained inside the 'header' portion of each entry (the option is available to sort by portions other than the header as well). The database object then writes the now sorted content to the file labeled "output.dat". You will notice that this output is now in alphabetical order according to the headers.

NOTE: The relative order/neatness of the input file (the number of new lines, empty space, order of tagged portions) is irrellavant to the algorithms being used. The input file has only been so organized for readability while testing different inputs.
