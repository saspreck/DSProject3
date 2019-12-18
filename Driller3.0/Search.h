#pragma once
// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
// if not found, returns -(location to insert + 1)
template <typename T>
long long static binarySearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator) {

	//declares the low, mid, and high of the array to search
	long long mid = 0;
	long long low = 0;
	long long high = array.getSize() - 1L;

	//loops for as long as high is greater than low
	while (high >= low) {
		//gets the midpoint of the array to search
		mid = (high + low) / 2;
		//compares the midpoint and the item to find
		int index = 0;
		//compares the two items and catches an index out of bounds exception
		try {
			index = comparator.compare(array[mid], item);
		}
		catch (ExceptionIndexOutOfRange* e) {
			delete e;
		}

		//checks whether the item is above or below the midpoint
		if (index < 0L) {
			low = mid + 1L;
		}
		else if (index > 0L) {
			high = mid - 1L;
		}
		//if location equals midpoint loops through to find the first location of it in the array and returns that location
		else {
			long long location = mid;

			if (location > 0L) {

				//finds first location of item in the array
				while (location - 1 >= 0 && comparator.compare(array[(unsigned long) location - 1], array[(unsigned long) location]) == 0) {
					location = location - 1L;
				}
				return location;
			}
		}
	}
	//if not found returns the negative location to be inserted
	return -(mid + 1);
}

/*
Linear search method to find the first instance of the given item in the array
*/
template <typename T>
long long static linearSearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator) {

	//loops through the array to find the given item
	for (unsigned int i = 0; i < array.getSize(); i++) {
		if (comparator.compare(array[i], item) == 0) {
			return (long long)i;
		}
	}
	//returns -1 if item cannot be found
	return -1;
}


#endif