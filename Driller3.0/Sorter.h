#pragma once
// Signature must be retained. Implementation must be added.

#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"
#include "DrillingRecord.h"

template <typename T>
class Sorter {
private:
	// additional member functions (methods) and variables (fields) may be added
	static void quickSort(ResizableArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator);
	static unsigned long partition(ResizableArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator);
public:
	static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
};

// implementation goes here

/*
callable sort method to initiate the sorting
*/
template <typename T>
void Sorter<T>::sort(ResizableArray<T>& array, const Comparator<T>& comparator) {

	//varaible for the left and right ends of the array
	unsigned long left = 0;
	unsigned long right = array.getSize() - 1;

	//calls quicksort method
	quickSort(array, left, right, comparator);
}

/*
Quicksort method to sort the array based on the given column
The format of this searching algorithm comes from the one in Zybooks, I modified it to fit this program
however many of the variables and comments remain unchanged
*/
template<typename T>
void Sorter<T>::quickSort(ResizableArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator) {
	unsigned long j = 0;

	// Base case: If there are 1 or zero elements to sort,
	// partition is already sorted
	if (left >= right) {
		return;
	}

	// Partition the data within the array. Value j returned
	// from partitioning is location of last element in low partition.
	j = partition(array, left, right, comparator);

	// Recursively sort low partition (i to j) and
	// high partition (j + 1 to k)
	quickSort(array, left, j, comparator);
	quickSort(array, j + 1, right, comparator);
}

/*
Partitioning file to split the array when sorting
Taken and modified from the method given in zybooks
*/
template <typename T>
unsigned long Sorter<T>::partition(ResizableArray<T>& array, unsigned long left, unsigned long right, const Comparator<T>& comparator) {

	//declare variables
	unsigned long l = 0;
	unsigned long h = 0;
	unsigned long midpoint = 0;
	T pivot;
	T temp;
	bool done = false;

	// Pick middle element as pivot
	midpoint = left + (right - left) / 2;
	pivot = array.get(midpoint);

	l = left;
	h = right;

	//while loop to partition the array
	while (!done) {

		// Increment l while numbers[l] < pivot
		while (l < array.getSize() && (comparator.compare(array.get(l), pivot) < 0)) {
			++l;
		}

		// Decrement h while pivot < numbers[h]
		while (h < array.getSize() && (comparator.compare(array.get(h), pivot) > 0)) {
			--h;
		}

		// If there are zero or one elements remaining,
		// all numbers are partitioned. Return h
		if (l >= h) {
			done = true;
		}
		else {
			// Swap numbers[l] and numbers[h],
			// update l and h
			temp = array.get(l);
			array.replaceAt(array.get(h), l);
			array.replaceAt(temp, h);

			++l;
			--h;
		}
	}

	return h;
}

#endif