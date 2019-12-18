#include "DrillingRecord.h"
#include "Exceptions.h"
#include <iomanip>
#include <iostream>
using namespace std;

/*
Default constructor, initializes all numbers to 0 and all strings to ""
*/
DrillingRecord::DrillingRecord() {

	//fills the nums array with 0s
	for (unsigned int i = 0; i < MAX_NUMS; i++) {
		nums[i] = 0.0;
	}

	//fills the strings array with ""s
	for (unsigned int i = 0; i < MAX_STRINGS; i++) {
		strings[i] = "";
	}
}

/*
Method to add num to array and increment the counter
*/
void DrillingRecord::addNum(double num) {
	nums[numCtr] = num;
	numCtr++;
}

/*
Method to add string to array and increment the counter
*/
void DrillingRecord::addString(string string) {
	strings[strCtr] = string;
	strCtr++;
}

/*
Returns the double at the given index
*/
double DrillingRecord::getNum(unsigned int index) const {

	//checks if index is less than numCtr and throws an exception otherwise
	if (index < MAX_NUMS) {
		return nums[index];
	}
	else {
		throw new ExceptionIndexOutOfRange();
	}
}

/*
Returns the string at the given index
*/
string DrillingRecord::getString(unsigned int index) const {

	//checks if index is less than strCtr and throws an exception otherwise
	if (index < MAX_NUMS) {
		return strings[index];
	}
	else {
		throw new ExceptionIndexOutOfRange();
	}
}

/*
Method to set the number at a given place in the drilling record
*/
void DrillingRecord::setNum(double num, unsigned int index) {

	//checks if index is less than numCtr and throws an exception if not
	if (index < MAX_NUMS) {
		nums[index] = num;

	}
	else {
		throw new ExceptionIndexOutOfRange();
	}

}

/*
Method to set the string at a given place in the drilling record
*/
void DrillingRecord::setString(string string, unsigned int index) {

	//checks if index is less than strCtr and throws an exception if not
	if (index < MAX_NUMS) {
		strings[index] = string;
		strCtr++;
	}
	else {
		throw new ExceptionIndexOutOfRange();
	}

}

/*
Overloaded ostream operator to create correct output
*/
ostream& operator<<(ostream& os, const DrillingRecord& record) {

	for (unsigned int i = 0; i < record.strCtr; i++) {

		//try/catch block to handle IndexOutOfRange exception in the ostream
		try {
			os << record.getString(i) << ";";
		}
		catch (ExceptionIndexOutOfRange* e) {
			delete e;
		}

	}

	for (unsigned int i = 0; i < record.numCtr; i++) {

		//try/catch block to handle IndexOutOfRange exception in the ostream
		try {
			if (i == record.numCtr - 1) {
				os << fixed << setprecision(2) << record.getNum(i);
			}
			else {
				os << fixed << setprecision(2) << record.getNum(i) << ";";
			}
		}
		catch (ExceptionIndexOutOfRange* e) {
			delete e;
		}


	}

	return os;
}