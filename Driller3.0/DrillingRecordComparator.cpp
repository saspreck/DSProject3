#include "DrillingRecordComparator.h"

/*
Constructor for a drilling record comparator that takes a column as a parameter
*/
DrillingRecordComparator::DrillingRecordComparator(unsigned int column) {
	this->column = column;
}

/*
Method to compare two DrillingRecord objects
*/
int DrillingRecordComparator::compare(const DrillingRecord& item1, const DrillingRecord& item2) const {

	//checks whether strings or numbers are being compared
	if (column < 2) {

		//returns appropriate value based on if the first string is greater than, less than, or equal to the second string
		if (item1.getString(column) < item2.getString(column)) {
			return -1;
		}
		else if (item1.getString(column) == item2.getString(column)) {
			return 0;
		}
		else if (item1.getString(column) > item2.getString(column)) {
			return 1;
		}
	}
	else {

		//does the same as above only with numbers
		if (item1.getNum(column - 2) < item2.getNum(column - 2)) {
			return -1;
		}
		else if (item1.getNum(column - 2) == item2.getNum(column - 2)) {
			return 0;
		}
		else if (item1.getNum(column - 2) > item2.getNum(column - 2)) {
			return 1;
		}
	}
	return -2;
}