// Driller2.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <array>
#include <algorithm>
#include "ResizableArray.h"
#include "Sorter.h"
#include "Search.h"
#include "DrillingRecord.h"
#include "DrillingRecordComparator.h"
#include "Comparator.h"
#include "OULink.h"
#include "OULinkedList.h"
#include "Enumerator.h"
#include "OULinkedListEnumerator.h"
#include "Exceptions.h"
#include "HashTable.h"
#include "Hasher.h"
#include "HashTableEnumerator.h"
#include "DrillingRecordHasher.h"

using namespace std;

//global variables to keep track of lines read and date stamps
unsigned long totalLinesRead = 0;
unsigned long validLinesRead = 0;
unsigned long recordsCurrentlyStored = 0;
string initialDateStamp = "";

/*============================================================================================
Function to check if the date stamp matches the correct one
*/
int checkDateStamp(string dataLine, string dateStamp, int lineNum) {

	//creates a substring that contains only the date
	string dateCheck = dataLine.substr(0, dataLine.find(','));
	//checks if the substring doesn't match the correct one
	if (dateCheck != dateStamp) {
		//prints out the error message
		std::cout << "Non-matching date stamp " << dateCheck << " at line " << lineNum << "." << endl;
		return false;
	}
	else {
		return true;
	}

}
//============================================================================================


/*============================================================================================
Function to check if the time stamp is unique
*/
bool checkTimeStamp(string dataLine, OULinkedList<DrillingRecord>* linkedList, int lineNum) {

	//makes a substring of everything except the date
	string dataLineMinusDate = dataLine.substr(dataLine.find(',') + 1);

	//makes a substring of the time stamp
	string timeStampToCheck = dataLineMinusDate.substr(0, dataLineMinusDate.find(','));

	//drilling record to store the time to check
	DrillingRecord* temp = new DrillingRecord();
	temp->setString(timeStampToCheck, 1);

	//iterates through the given array to check for matching time stamps
	for (unsigned int i = 0; i < linkedList->getSize(); i++) {

		//checks if time stamp equals one from array and prints error message if so
		if (linkedList->contains(*temp)) {
			std::cout << "Duplicate timestamp " << timeStampToCheck << " at line " << lineNum << "." << endl;
			delete temp;
			temp = nullptr;
			return false;
		}
	}
	//deletes objects
	delete temp;
	temp = nullptr;
	return true;

}
//============================================================================================


/*============================================================================================
Function to check that all the float values are positive
*/
bool checkFloatValues(string dataLine, int lineNum) {
	//declare variables
	string tempData;
	double doubleData;

	//turn input string into a stringstream
	stringstream stringData(dataLine);

	//ignores the date and time stamps
	stringData.ignore(256, ',');
	stringData.ignore(256, ',');

	//iterates through each data point checking
	for (int i = 0; i < 16; i++) {
		//splits each piece of data off from the stringstream and turns it into a double
		getline(stringData, tempData, ',');
		doubleData = stod(tempData);

		//checks if the data is positive and throws an error otherwise
		if (doubleData <= 0) {
			std::cout << "Invalid floating-point data at line " << lineNum << "." << endl;
			return false;
		}
	}
	//returns true if all the data values are positive
	return true;
}
//============================================================================================


/*============================================================================================
Method to output the data using the resizable array
*/
void dataOutput(ResizableArray<DrillingRecord>* mainRecordArray) {

	//priming read for the loop to get the output file name
	string outputFileName = "";
	std::cout << "Enter output file name: "; \
		getline(cin, outputFileName, '\n');
	ofstream outputToFile;
	bool printed = false;

	//loops until the data has been printed
	while (printed == false) {

		//checks if the user entered anything
		if (outputFileName.empty()) {

			//iterates through and prints out data
			for (unsigned long i = 0; i < recordsCurrentlyStored; i++) {

				//try/catch block to catch an IndexOutOfRange exception
				try {
					std::cout << mainRecordArray->get(i) << endl;

				}
				catch (ExceptionIndexOutOfRange* e) {
					delete e;
				}
			}
			//prints out different numbers of lines read in
			cout << "Data lines read: " << totalLinesRead << "; Valid Drilling records read: " << validLinesRead << "; Drilling records in memory: " << recordsCurrentlyStored << endl;
			printed = true;
		}
		else {
			//attempts to open file and if cannot displays warning
			outputToFile.open(outputFileName);
			if (!outputToFile.is_open()) {
				std::cout << "File is not available" << endl;
				std::cout << "Enter output file name: ";
				getline(cin, outputFileName, '\n');
			}
			//if file opens prints to file
			else {

				//loops through and prints out each record to the file
				for (unsigned long i = 0; i < recordsCurrentlyStored; i++) {
					try {
						outputToFile << mainRecordArray->get(i) << endl;
					}
					catch (ExceptionIndexOutOfRange* e) {
						delete e;
					}
				}
				outputToFile << "Data lines read: " << totalLinesRead << "; Valid Drilling records read: " << validLinesRead << "; Drilling records in memory: " << recordsCurrentlyStored << endl;
				printed = true;
			}
			//closes file stream
			outputToFile.close();
		}
	}
}
//============================================================================================



/*============================================================================================
Method to sort the data
*/
unsigned int sortMethod(ResizableArray<DrillingRecord>* mainRecordArray, unsigned int sortColumn) {
	//asks the user which field to sort on and converts it to an int
	string sortField = "";
	std::cout << "Enter sort field (0-17): " << endl;
	getline(cin, sortField, '\n');
	unsigned int column = (unsigned)stoi(sortField);

	//makes sure it is valid and if so sorts based on that column
	if (column <= 17) {
		sortColumn = column;
		DrillingRecordComparator sortComp(column);
		Sorter<DrillingRecord>::sort(*mainRecordArray, sortComp);
	}
	return sortColumn;
}
//============================================================================================



/*============================================================================================
Method to search through the data
*/
void searchMethod(ResizableArray<DrillingRecord>* mainRecordArray, HashTable<DrillingRecord>* mainHashTable, unsigned int sortColumn) {

	//asks the user which field to sort on and converts it to an int
	string searchField = "";
	std::cout << "Enter search field (0-17): ";
	getline(cin, searchField, '\n');
	unsigned int column = (unsigned)stoi(searchField);

	//makes sure the field number is valid
	if (column <= 17) {

		//checks to see if a numeric column
		if (column > 1) {

			//asks the user for the value to search for
			string fieldValue = "";
			std::cout << "Enter positive field value: ";
			getline(cin, fieldValue);

			//checks if the user entered something
			if (fieldValue != "") {
				double keyValue = stod(fieldValue);

				//makes sure key is valid and then searches
				if (keyValue > 0) {

					//creates new drilling record and sets the value entered to the correct column
					DrillingRecord* searchRecord = new DrillingRecord;
					try {
						searchRecord->setNum(keyValue, column - 2);
					}
					catch (ExceptionIndexOutOfRange* e) {
						delete e;
					}


					//creates comparator for the search
					Comparator<DrillingRecord>* searchComp = new DrillingRecordComparator(column);
					long long index;
					long long location = 0;

					//checks if the column entered is the same column as the column
					//sorted on and calls correct search
					if (sortColumn == column) {
						index = binarySearch(*searchRecord, *mainRecordArray, *searchComp);
					}
					else {
						index = linearSearch(*searchRecord, *mainRecordArray, *searchComp);
					}

					location = index;
					int numMatches = 0;

					//if the search key is in the array then it iterates through, finding all records with a matching value
					if (index >= 0) {
						while ((unsigned)index < mainRecordArray->getSize()) {

							if (searchComp->compare(mainRecordArray->get((unsigned long)index), mainRecordArray->get((unsigned long)location)) == 0) {
								try {
									cout << mainRecordArray->get((unsigned long)index) << endl;
									numMatches++;
								}
								catch (ExceptionIndexOutOfRange* e) {
									delete e;
								}
							}

							index++;
						}
						//prints out total number of matching records found
						cout << "Drilling records found: " << numMatches << "." << endl;
					}
					else {
						cout << "Drilling records found: 0." << endl;
					}

					//deletes objects
					delete searchRecord;
					searchRecord = nullptr;
				}
			}
		}
		//for if user wants to search for a string
		else {
			//asks user what text they would like to find
			string fieldValue = "";
			std::cout << "Enter exact text on which to search: ";
			getline(cin, fieldValue, '\n');

			//checks if user entered something and then searches for the text entered
			if (fieldValue != "") {

				//creates new drilling record and sets the value entered to the correct column
				DrillingRecord* searchRecord = new DrillingRecord;
				try {
					searchRecord->setString(fieldValue, column);
				}
				catch (ExceptionIndexOutOfRange* e) {
					delete e;
				}

				//creates comparator for the search
				Comparator<DrillingRecord>* searchComp = new DrillingRecordComparator(column);
				long long index;
				long long location;

				//if column to search on is time stamp, hash
				if (column == 1) {
					try {
						cout << mainHashTable->find(*searchRecord) << endl;
					}
					catch (ExceptionHashTableAccess* e) {
						cout << "Drilling records found: 0." << endl;
						delete e;
					}
				}

				//checks if the column entered is the same column as the column
				//sorted on and calls correct search
				if (sortColumn == column) {
					index = binarySearch(*searchRecord, *mainRecordArray, *searchComp);
				}
				else {
					index = linearSearch(*searchRecord, *mainRecordArray, *searchComp);
				}

				location = index;
				int numMatches = 0;
				//iterates through array if search returns a nonnegative printing out matches
				if (index >= 0) {
					while ((unsigned)index < mainRecordArray->getSize()) {

						if (searchComp->compare(mainRecordArray->get((unsigned long)index), mainRecordArray->get((unsigned long)location)) == 0) {
							try {
								cout << mainRecordArray->get((unsigned long)index) << endl;
								numMatches++;
							}
							catch (ExceptionIndexOutOfRange* e) {
								delete e;
							}
						}

						index++;
					}
					//prints out total number of matches found
					cout << "Drilling records found: " << numMatches << "." << endl;
				}
				else {
					cout << "Drilling records found: 0." << endl;
				}

				//deletes objects
				delete searchRecord;
				searchRecord = nullptr;

			}
		}
	}
}
//============================================================================================



/*============================================================================================
 Method to read in new files
*/
OULinkedList<DrillingRecord>* dataInput() {

	ifstream fileName;
	string userInput = "";

	//prompt for user to enter name of first file
	cout << "Enter data file name: ";
	getline(cin, userInput, '\n');

	//linked list to read data into
	Comparator<DrillingRecord>* newComp = new DrillingRecordComparator(1);
	OULinkedList<DrillingRecord>* mainLinkedList = new OULinkedList<DrillingRecord>(newComp);

	//loop to read in file from the user
	while (mainLinkedList->getSize() == 0 && !userInput.empty()) {

		//variable to keep track of current file line being used
		int fileLineNum = 1;

		//attempts to open the file and tells the user if that fails
		fileName.open(userInput, ifstream::in);
		if (!fileName.is_open()) {
			std::cout << "File is not available." << endl;

			//repeats prompt for input
			userInput = "";
			std::cout << "Enter data file name: ";
			getline(cin, userInput, '\n');
			continue;
		}

		//declares variable to store file line in and then skips the first line
		string line = "";
		fileName.ignore(10000, '\n');

		//loop to iterate through file collecting data
		while (getline(fileName, line)) {

			//sets the first date stamp to compare against
			if (initialDateStamp == "") {
				initialDateStamp = line.substr(0, line.find(','));
			}
			//calls the checkDateStamp function to ensure date stamps are correct
			if (checkDateStamp(line, initialDateStamp, fileLineNum)) {

				//calls the checkFloatValues function to ensure they are positive are unique
				if (checkTimeStamp(line, mainLinkedList, fileLineNum)) {

					//calls the checkFloatValues method to ensure they are positive
					if (checkFloatValues(line, fileLineNum)) {

						//declares a drilling record to use and a string stream
						DrillingRecord currentRecord;

						//pulls off datestamp and puts it in correct array
						string dateStamp = line.substr(0, line.find(','));
						currentRecord.addString(dateStamp);

						//pulls off timestamp and puts it in correct array
						string noDateStamp = line.substr(line.find(',') + 1);
						string timeStamp = noDateStamp.substr(0, noDateStamp.find(','));
						currentRecord.addString(timeStamp);

						string noTimeStamp = noDateStamp.substr(noDateStamp.find(',') + 1);
						stringstream noStrings(noTimeStamp);

						//splices off each piece of data, turns it to a double, and then puts it into the correct array
						for (int i = 0; i < 16; i++) {
							string tempData = "";
							getline(noStrings, tempData, ',');
							double doubleData = stod(tempData);
							currentRecord.addNum(doubleData);

						}
						//cout << currentRecord << endl;
						//adds the current drilling record to the linked list and increments the number of valid lines
						mainLinkedList->insert(currentRecord);
						validLinesRead++;

					}
				}
			}
			//increases the line number
			fileLineNum++;
			totalLinesRead++;
		}

		//update the value of the recordscurrentlystored variable
		recordsCurrentlyStored = mainLinkedList->getSize();

		//close filestream
		fileName.close();

		//if linked list size is 0 then no valid data was read
		if (mainLinkedList->getSize() == 0) {
			//resets userInput and asks user for new filename
			cout << "No valid records found." << endl;
			userInput = "";
			std::cout << "Enter data file name: ";
			getline(std::cin, userInput, '\n');
		}

	}
	return mainLinkedList;
}
//============================================================================================


/*============================================================================================
Merge method, reads in new file and replaces any duplicates
*/
void mergeMethod(OULinkedList<DrillingRecord>* mainLinkedList, ResizableArray<DrillingRecord>* mainRecordArray, HashTable<DrillingRecord>* mainHashTable) {

	//creates a new linked list and reads file into that list
	OULinkedList<DrillingRecord>* tempList = dataInput();

	//creates an enum to iterate through the list with
	OULinkedListEnumerator<DrillingRecord> tempEnum = tempList->enumerator();

	//loops until there is not next element
	while (tempEnum.hasNext()) {
		try {

			//if the main list contains the element, replace it
			if (mainLinkedList->replace(tempEnum.peek())) {
			}
			//else insert it
			else {
				mainLinkedList->insert(tempEnum.peek());
			}
			//iterates enumerator
			tempEnum.next();

		}
		catch (ExceptionEnumerationBeyondEnd* e) {
			delete e;
		}
	}

	//loop to clear the array
	mainRecordArray->clear();

	//creates new comparator and hasher for hash table
	Comparator<DrillingRecord>* newComp = new DrillingRecordComparator(1);
	Hasher<DrillingRecord>* newHash = new DrillingRecordHasher();

	//deletes old hash table
	delete mainHashTable;

	//reassigns main hash table
	mainHashTable = new HashTable<DrillingRecord>(newComp, newHash, mainLinkedList->getSize());

	//enum to iterate through main list
	OULinkedListEnumerator<DrillingRecord> enum2 = mainLinkedList->enumerator();

	//loops through and adds every item in the list into the array and the hash table
	while (enum2.hasNext()) {
		DrillingRecord current = enum2.next();
		mainRecordArray->add(current);
		mainHashTable->insert(current);
	}


	//sorts array based on time
	DrillingRecordComparator sortComp(1);
	Sorter<DrillingRecord>::sort(*mainRecordArray, sortComp);

	//updates the variable
	recordsCurrentlyStored = mainRecordArray->getSize();

	//deletes objects
	delete tempList;
	tempList = nullptr;


}
//============================================================================================



/*============================================================================================
Purge method, reads in new file and deletes any duplicates
*/
void purgeMethod(OULinkedList<DrillingRecord>* mainLinkedList, ResizableArray<DrillingRecord>* mainRecordArray, HashTable<DrillingRecord>* mainHashTable) {

	//creates a new linked list and reads into that list
	OULinkedList<DrillingRecord>* tempList = dataInput();

	//enum to iterate through list with
	OULinkedListEnumerator<DrillingRecord> tempEnum = tempList->enumerator();

	//loops until there is no next element
	while (tempEnum.hasNext()) {

		//attempts to remove current item and catches an enumeration beyond end exception
		try {

			//attempts to remove link and iterates enumerator
			mainLinkedList->remove(tempEnum.peek());
			tempEnum.next();

		}
		catch (ExceptionEnumerationBeyondEnd* e) {
			delete e;
		}
	}

	//loop to clear the array
	mainRecordArray->clear();

	//deletes hash table
	delete mainHashTable;

	//creates a new hash table
	Comparator<DrillingRecord>* newComp = new DrillingRecordComparator(1);
	Hasher<DrillingRecord>* newHasher = new DrillingRecordHasher();

	//reassigns hash table
	mainHashTable = new HashTable<DrillingRecord>(newComp, newHasher, mainLinkedList->getSize());

	//enum to iterate through main list
	OULinkedListEnumerator<DrillingRecord> enum2 = mainLinkedList->enumerator();

	//loops through and adds every item in the list into the array and hash table
	while (enum2.hasNext()) {
		DrillingRecord current = enum2.next();
		mainRecordArray->add(current);
		mainHashTable->insert(current);
	}

	//sorts the array based on time if the array is not empty
	if (mainRecordArray->getSize() > 0) {
		DrillingRecordComparator sortComp(1);
		Sorter<DrillingRecord>::sort(*mainRecordArray, sortComp);
	}

	//updates the variable
	recordsCurrentlyStored = mainRecordArray->getSize();

	//deletes temp list
	delete tempList;
	tempList = nullptr;

}
//============================================================================================


/*============================================================================================
Method to output data using the linked list
*/
void recordOutput(OULinkedList<DrillingRecord>* mainLinkedList) {

	//priming read for the loop to get the output file name
	string outputFileName = "";
	std::cout << "Enter output file name: ";
	getline(cin, outputFileName, '\n');
	ofstream outputToFile;
	bool printed = false;

	//loops until the data has been printed
	while (printed == false) {

		//enumerator for linked list
		OULinkedListEnumerator<DrillingRecord> _enum = mainLinkedList->enumerator();

		//checks if the user entered anything
		if (outputFileName.empty()) {

			//iterates through and prints out data
			while (_enum.hasNext()) {

				//try/catch block to catch an IndexOutOfRange exception
				try {
					//prints out each object from the enumerator
					std::cout << _enum.next() << endl;

				}
				catch (ExceptionIndexOutOfRange* e) {
					delete e;
				}
			}
			//prints out different numbers of lines read in
			cout << "Data lines read: " << totalLinesRead << "; Valid drilling records read: " << validLinesRead << "; Drilling records in memory: " << recordsCurrentlyStored << endl;
			printed = true;
		}

		//branch if user entered file name
		else {

			//attempts to open file and if cannot displays warning
			outputToFile.open(outputFileName);
			if (!outputToFile.is_open()) {
				std::cout << "File is not available" << endl;
				std::cout << "Enter output file name: ";
				getline(cin, outputFileName, '\n');
			}

			//if file opens prints to file
			else {

				//loops through and prints out each record to the file
				while (_enum.hasNext()) {
					try {
						outputToFile << _enum.next() << endl;
					}
					catch (ExceptionIndexOutOfRange* e) {
						delete e;
					}

				}
				//prints out lines read
				outputToFile << "Data lines read: " << totalLinesRead << "; Valid Drilling records read: " << validLinesRead << "; Drilling records in memory: " << recordsCurrentlyStored << endl;
				printed = true;
			}

			//closes file stream
			outputToFile.close();
		}
	}
}
//============================================================================================

/*
Method to output data from the hash table
*/
void hashOutput(HashTable<DrillingRecord>* mainHashTable) {

	string outputFileName = "";
	cout << "Enter output file name: ";
	getline(cin, outputFileName, '\n');
	ofstream outputFileStream;
	bool printed = false;
	bool firstBucketList = true;

	HashTableEnumerator<DrillingRecord>* outputEnum = new HashTableEnumerator<DrillingRecord>(mainHashTable);
	DrillingRecord current;
	DrillingRecord previous;

	//iterates until loop is printed
	while (printed == false) {

		//if user doesn't enter a file name
		if (outputFileName.empty()) {

			//iterates until the end of the hash table
			while (outputEnum->hasNext()) {

				//iterates to next bucket
				current = outputEnum->next();

				//if item is the first bucket
				if (firstBucketList == true) {

					cout << mainHashTable->getBucketNumber(current) << ": " << current << endl;
					firstBucketList = false;
				}

				else {
					//if item in same bucket as previous item, print overflow
					if (mainHashTable->getBucketNumber(current) == mainHashTable->getBucketNumber(previous)) {
						cout << "OVERFLOW: " << current << endl;
					}

					else {
						cout << endl;
						cout << mainHashTable->getBucketNumber(current) << ": " << current << endl;
					}

				}

				//put previous to current bucket
				previous = current;
			}
			cout << endl;
			cout << "Base Capacity: " << mainHashTable->getBaseCapacity() << "; Total Capacity: " << mainHashTable->getTotalCapacity() << "; Load Factor: " << mainHashTable->getLoadFactor() << endl;
			cout << "Data lines read: " << totalLinesRead << "; Valid Drilling records read: " << validLinesRead << "; Drilling records in memory: " << recordsCurrentlyStored << endl;
			printed = true;
		}

		//if user enters a file name
		else {

			//attempts to open file and if cannot displays warning
			outputFileStream.open(outputFileName);
			if (!outputFileStream.is_open()) {
				std::cout << "File is not available" << endl;
				std::cout << "Enter output file name: ";
				getline(cin, outputFileName, '\n');
			}
			else {

				//iterates until the end of the hash table
				while (outputEnum->hasNext()) {

					//iterates to next bucket
					current = outputEnum->next();

					//if item is the first bucket
					if (firstBucketList == true) {

						outputFileStream << mainHashTable->getBucketNumber(current) << ": " << current << endl;
						firstBucketList = false;
					}

					else {
						//if item in same bucket as previous item, print overflow
						if (mainHashTable->getBucketNumber(current) == mainHashTable->getBucketNumber(previous)) {
							outputFileStream << "OVERFLOW: " << current << endl;
						}

						else {
							outputFileStream << endl;
							outputFileStream << mainHashTable->getBucketNumber(current) << ": " << current << endl;
						}

					}

					//put previous to current bucket
					previous = current;
				}
				outputFileStream << endl;
				outputFileStream << "Base Capacity: " << mainHashTable->getBaseCapacity() << "; Total Capacity: " << mainHashTable->getTotalCapacity() << "; Load Factor: " << mainHashTable->getLoadFactor() << endl;
				outputFileStream << "Data lines read: " << totalLinesRead << "; Valid Drilling records read: " << validLinesRead << "; Drilling records in memory: " << recordsCurrentlyStored << endl;
				printed = true;
			}
		}

	}
}

/*============================================================================================
Main method, runs the full program
*/
int main()
{

	//declare variables
	string userInput = "";
	ifstream fileName;

	//declares ResizableArray object to use
	ResizableArray<DrillingRecord>* mainRecordArray = new ResizableArray<DrillingRecord>;

	//creates main linked list for the program and reads into it
	OULinkedList<DrillingRecord>* mainLinkedList = dataInput();

	//creates an enumerator to loop through the list and add the records to the array
	OULinkedListEnumerator<DrillingRecord> tempEnum = mainLinkedList->enumerator();

	//loops through and adds every item in the list into the array
	while (tempEnum.hasNext()) {
		DrillingRecord current = tempEnum.next();
		mainRecordArray->add(current);
	}

	//if linked list is empty, exit the program
	if (mainLinkedList->getSize() == 0) {
		return 0;
	}

	//creates a hash table to store data in
	Comparator<DrillingRecord>* hashComp = new DrillingRecordComparator(1);
	Hasher<DrillingRecord>* hasher = new DrillingRecordHasher();
	HashTable<DrillingRecord>* mainHashTable = new HashTable<DrillingRecord>(hashComp, hasher, mainLinkedList->getSize());
	tempEnum = mainLinkedList->enumerator();

	//loops through the list hashing each item
	while (tempEnum.hasNext()) {
		DrillingRecord current = tempEnum.next();
		mainHashTable->insert(current);
	}

	//sorts the array based on time
	DrillingRecordComparator sortComp(1);
	Sorter<DrillingRecord>::sort(*mainRecordArray, sortComp);

	//priming read for the main loop 
	cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, (h)ash table, or (q)uit: ";
	getline(cin, userInput, '\n');

	//column to keep track of which column the array of data is sorted
	//initialized to the time stamp since that is the default
	unsigned int sortColumn = 1;

	/*
	Main data loop, asks users whether they would like output, sort, find, merge, purge, records, or quit
	*/
	while (userInput != "q" && userInput != "Q") {

		//method call for if the user enters an o
		if (userInput == "o" || userInput == "O") {
			dataOutput(mainRecordArray);
		}

		//method call for if the user enters an s
		if (userInput == "s" || userInput == "S") {
			sortColumn = sortMethod(mainRecordArray, sortColumn);
		}

		//method call for if the user enters an f
		if (userInput == "f" || userInput == "F") {
			searchMethod(mainRecordArray, mainHashTable, sortColumn);
		}

		//method call for if the user enters an m
		if (userInput == "m" || userInput == "M") {
			mergeMethod(mainLinkedList, mainRecordArray, mainHashTable);
		}

		//method call for if the user enters a p
		if (userInput == "p" || userInput == "P") {
			purgeMethod(mainLinkedList, mainRecordArray, mainHashTable);
		}

		//method call for if the user enters an r
		if (userInput == "r" || userInput == "R") {
			recordOutput(mainLinkedList);
		}

		//method call for if the user enters an h
		if (userInput == "h" || userInput == "H") {
			hashOutput(mainHashTable);
		}

		//gets user input for next pass through the loop 
		cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, (h)ash table, or (q)uit: ";
		getline(cin, userInput, '\n');
	}

	//prints a farewell if user hits quit
	cout << "Thanks for using Driller." << endl;

	//deletes linked list, resizable array, and hash table
	delete mainLinkedList;
	mainLinkedList = nullptr;
	delete mainRecordArray;
	mainRecordArray = nullptr;
	delete mainHashTable;
	mainHashTable = nullptr;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
