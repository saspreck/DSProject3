#pragma once
#ifndef RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H

#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;        // capacity used in no arg constructor

template <typename T>
class ResizableArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
	unsigned long size = 0;                             // actual number of items currently in array
	T* data = NULL;                                     // pointer to array of any type
	void doubleCapacity();                              // method to double array capacity
	void halveCapacity();                               // method to halve array capacity
public:
	ResizableArray();                                   // constructs array with default capacity
	ResizableArray(unsigned long capacity);             // constructs array with specified capacity
	virtual ~ResizableArray();                          // frees array space as object is deleted
	void add(T item);                                   // adds item, increments size, doubles capacity as necessary
	void addAt(T item, unsigned long index);            // adds item at index, shifts following, doubles capacity as necessary
	void replaceAt(T item, unsigned long index);        // replaces item at index, otherwise unchanged
	void removeAt(unsigned long index);                 // removes item at index, shifts following back
	T get(unsigned long index) const;                   // returns (copy of) item at index
	unsigned long getSize() const;                      // returns number of items currently in array
	unsigned long getCapacity() const;                  // returns the current capacity of the array
	T operator[](unsigned long index) const;            // returns (copy of) item at index
	void clear();                                       // removes all items from array, resets to default capacity, set size to 0
};

// Add your implementation below this line. Do not add or modify anything above this line.


/*
Method to double the array size if necessary
*/
template <typename T>
void ResizableArray<T>::doubleCapacity() {
	//doubles capacity for new array
	unsigned long newCapacity = capacity * 2;

	//creates a new drilling record array with updated size
	T* tempRecord = new T[newCapacity];

	//copies data from old array to new one
	for (unsigned long i = 0; i < capacity; i++) {
		tempRecord[i] = data[i];
	}

	//reassigns the pointer and clears the old array
	data = tempRecord;
	tempRecord = nullptr;
	capacity = newCapacity;
}

/*
Method to halve the array size if necessary
*/
template <typename T>
void ResizableArray<T>::halveCapacity() {
	//doubles capacity for new array
	unsigned long newCapacity = capacity / 2;

	//creates a new drilling record array with updated size
	T* tempRecord = new T[newCapacity];

	//copies data from old array to new one
	for (unsigned long i = 0; i < newCapacity; i++) {
		tempRecord[i] = data[i];
	}

	//reassigns the pointer and clears the old array
	data = tempRecord;
	tempRecord = nullptr;
	capacity = newCapacity;
}

/*
Default constructor, initalizes the array
*/
template <typename T>
ResizableArray<T>::ResizableArray() {
	data = new T[DEFAULT_ARRAY_CAPACITY];
}

/*
Constructor that takes an initial array size parameter
*/
template <typename T>
ResizableArray<T>::ResizableArray(unsigned long capacity) {
	data = new T[capacity];
	this->capacity = capacity;
}

/*
Destructor
*/
template <typename T>
ResizableArray<T>::~ResizableArray() {
	delete[] data;
	data = nullptr;
}

/*
Method to add a new object to the array
*/
template <typename T>
void ResizableArray<T>::add(T item) {

	//checks if size of array is greater than or equal to capacity and if so doubles the size of the array
	if (size >= capacity) {
		doubleCapacity();
	}

	//adds item and increments size counter
	data[size] = item;
	size++;

}

/*
Method to add object at specified index of array
*/
template <typename T>
void ResizableArray<T>::addAt(T item, unsigned long index) {

	//checks if array capacity needs to be doubled
	if (size >= capacity) {
		doubleCapacity();
	}

	//makes sure the given index is within the size of the array and throws and error otherwise
	if (index < size) {
		//shifts data to fit in new item at specified location
		for (unsigned long i = size; i >= index; i--) {
			data[i] = data[i - 1];
		}

		//adds item at given index
		data[index] = item;
		size++;
	}
	else {
		throw new ExceptionIndexOutOfRange();
	}

}

/*
Method to replace item at given index with new item
*/
template <typename T>
void ResizableArray<T>::replaceAt(T item, unsigned long index) {

	//makes sure index is within size of array and throws exception otherwise
	if (index < size) {
		data[index] = item;
	}
	else {
		throw new ExceptionIndexOutOfRange();
	}
}

/*
Method to remove object at given index
*/
template <typename T>
void ResizableArray<T>::removeAt(unsigned long index) {

	//checks if array capacity needs to be halved
	if (size <= capacity / 2) {
		halveCapacity();
	}

	//makes sure index is within size of array and throws an exception otherwise
	if (index < size) {
		//shifts data to fit in new item at specified location
		for (unsigned long i = index; i < size; i++) {
			data[i] = data[i + 1];
		}

		size--;
	}
	else {
		throw new ExceptionIndexOutOfRange();
	}

}

/*
Method to get copy of data at given index
*/
template <typename T>
T ResizableArray<T>::get(unsigned long index) const {

	//checks if index is within array size and throws an exception if not
	if (index < size) {
		if (index < 0) {
			std::cout << "Index is " << index << std::endl;
		}
		return data[index];
	}
	else {
		throw new ExceptionIndexOutOfRange();
	}
}

/*
Method to get the current size of the array
*/
template <typename T>
unsigned long ResizableArray<T>::getSize() const {
	return size;
}

/*
Method to get the current capacity of the array
*/
template <typename T>
unsigned long ResizableArray<T>::getCapacity() const {
	return capacity;
}

/*
Overloads the [] operator to return the value at the given index
*/
template <typename T>
T ResizableArray<T>::operator[](unsigned long index) const {
	if (index < size) {
		if (index < 0) {
			std::cout << "Index is " << index << std::endl;
		}
		return data[index];
	}
	else {
		throw new ExceptionIndexOutOfRange();
	}

}

/*
Method to clear the array
*/
template <typename T>
void ResizableArray<T>::clear() {

	delete[] data;
	data = new T[DEFAULT_ARRAY_CAPACITY];
	capacity = DEFAULT_ARRAY_CAPACITY;
	size = 0;
}
#endif