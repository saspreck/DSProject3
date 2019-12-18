#pragma once
// Do not add or modify anything in this file.

#ifndef DRILLING_RECORD_COMPARATOR_H
#define DRILLING_RECORD_COMPARATOR_H

#include "Comparator.h"
#include "DrillingRecord.h"

class DrillingRecordComparator : public Comparator<DrillingRecord> {
private:
	unsigned int column = 0;
public:
	DrillingRecordComparator(unsigned int column);

	// returns -1 if item1 < item2, 0 if item1 == item2, +1 if item1 > item2
	int compare(const DrillingRecord& item1, const DrillingRecord& item2) const;
};


// You must create a corresponding DrillingRecordComparator.cpp file to implement the constructor and member function compare().

#endif