#include "DrillingRecordHasher.h"
#include <string>

using namespace std;

/*
Method to calculate the hash value of the given item's timestamp
*/
unsigned long DrillingRecordHasher::hash(const DrillingRecord& item) const {

	//variable to store ascii value of string
	int asciiTotal = 0;
	
	//puts drilling record's timestamp into a string
	string timeStamp = item.getString(1);

	//iterates through the timestamp and adds up the ascii values of each character
	for (unsigned long i = 0; i < timeStamp.length(); i++) {
		asciiTotal += int(timeStamp.at(i));
	}

	//returns the total ascii value
	return (unsigned long)asciiTotal;
}