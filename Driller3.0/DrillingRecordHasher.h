#pragma once
// Do not add or modify anything in this file.

#ifndef DRILLING_RECORD_HASHER_H
#define DRILLING_RECORD_HASHER_H

#include "Hasher.h"
#include "DrillingRecord.h"

class DrillingRecordHasher : public Hasher<DrillingRecord> {
public:
	unsigned long hash(const DrillingRecord& item) const;
};

// Implement in DrillingRecordHasher.cpp

#endif //!DRILLING_RECORD_HASHER_H