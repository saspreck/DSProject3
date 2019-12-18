#pragma once
#ifndef HASH_TABLE
#define HASH_TABLE

/*
NOTE: I worked with students such as SungJae Yoon, Bhaven Patel, and Braden Munn on aspects of this project, 
so while aspects and the structure of parts of my code may resemble theirs, all work done is my own
*/

#include <math.h>
#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"

const unsigned int SCHEDULE_SIZE = 25;
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX];
const float DEFAULT_MAX_LOAD_FACTOR = 0.8f;
const float DEFAULT_MIN_LOAD_FACTOR = 0.3f;

template <typename T>
class HashTable {
	template <typename U>
	friend class HashTableEnumerator;
private:
	Comparator<T>* comparator;                                // used to determine item equality
	Hasher<T>* hasher;                                        // used to compute hash value
	unsigned long size = 0;                                    // actual number of items currently in hash table
	float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;
	float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;
	unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;
	unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;        // the size of the array
	unsigned long totalCapacity = baseCapacity;                // the size of the array plus chains of more than one link
	OULinkedList<T>** table = NULL;                            // table will be an array of pointers to OULinkedLists of type T
	// you may add additional member variables and functions here to support the operation of your code
	void increaseHashTableSize();
	void decreaseHashTableSize();
public:
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher);            // creates an empty table of DEFAULT_BASE_CAPACITY
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
		// if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
		unsigned long size,
		float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
		float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
	virtual ~HashTable();

	// if an equivalent item is not already present, insert item at proper location and return true
	// if an equivalent item is already present, leave table unchanged and return false
	bool insert(T item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool replace(T item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool remove(T item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionHashTableAccess
	T find(T item) const;

	unsigned long getSize() const;                            // returns the current number of items in the table
	unsigned long getBaseCapacity() const;                    // returns the current base capacity of the table
	unsigned long getTotalCapacity() const;                    // returns the current total capacity of the table
	float getLoadFactor() const;                            // returns the current load factor of the table
	unsigned long getBucketNumber(T item) const;            // returns the current bucket number for an item
};

// Add your implementation below this line.

/*
Constructor to create an empty hash table
*/
template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher) {

	//set variables
	this->comparator = comparator;
	this->hasher = hasher;
	this->table = new OULinkedList<T>* [baseCapacity];

	//puts an OULinkedList in each bucket
	for (unsigned long i = 0; i < baseCapacity; i++) {
		table[i] = new OULinkedList<T>(comparator);
	}
}


/*
Constructor to create a hash table given the parameters
*/
template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
	// if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
	unsigned long size, float maxLoadFactor, float minLoadFactor) {

	//assigns variables
	this->comparator = comparator;
	this->hasher = hasher;
	this->maxLoadFactor = maxLoadFactor;
	this->minLoadFactor = minLoadFactor;

	int scheduleNum = 0;

	//iterates through and finds the correct size of hash table to make
	while ((float) size > (float) SCHEDULE[scheduleNum] * maxLoadFactor && scheduleIndex < SCHEDULE_SIZE) {
		scheduleNum++;
	}

	//makes a new hash table
	this->table = new OULinkedList<T>* [SCHEDULE[scheduleNum]];
	baseCapacity = SCHEDULE[scheduleNum];
	totalCapacity = baseCapacity;

	//puts an OULinkedList into each bucket
	for (unsigned long i = 0; i < baseCapacity; i++) {
		table[i] = new OULinkedList<T>(comparator);
	}
}


/*
Destructor for the hash table
*/
template <typename T>
HashTable<T>::~HashTable() {

	//table
	delete[] table;
	table = nullptr;
}


/*
Method to insert an item into the hash table
*/
template <typename T>
bool HashTable<T>::insert(T item) {

	//get the bucket number of the item to insert
	unsigned long itemBucketNumber = getBucketNumber(item);

	//branch if the item is inserted successfully
	if (table[itemBucketNumber]->insert(item)) {

		//if bucket already contains something, increase total capacity
		if (table[itemBucketNumber]->getSize() > 1) {
			totalCapacity++;
		}
		size++;

		//check if table needs resizing
		if (getLoadFactor() >= maxLoadFactor) {
			increaseHashTableSize();
		}
		return true;
	}
	return false;
}


/*
Method to replace an item in the hash table
*/
template <typename T>
bool HashTable<T>::replace(T item) {

	return table[getBucketNumber(item)]->replace(item);
}


/*
Method to remove item from the hash table
*/
template <typename T>
bool HashTable<T>::remove(T item) {

	//if bucket is empty, return false
	if (table[getBucketNumber(item)]->getSize() == 0) {

		return false;
	}
	//if bucket contains item, remove it
	else if (table[getBucketNumber(item)]->remove(item)) {

		//checks if bucket isn't empty after removing
		if (table[getBucketNumber(item)]->getSize() != 0) {
			totalCapacity--;
		}
		size--;
		
		//decrease the size of the hash table is the load factor is two low
		if (getLoadFactor() <= minLoadFactor) {
			decreaseHashTableSize();
		}
		return true;
	}
	//else return false
	return false;
}


/*
Method to find an item in the hash table
*/
template <typename T>
T HashTable<T>::find(T item) const {

	//gets bucket number of item
	unsigned long itemBucketNumber = getBucketNumber(item);

	//checks if bucket linked list contains item
	if (table[itemBucketNumber]->contains(item)) {
		return table[itemBucketNumber]->find(item);
	}
	//throws exception if not
	else {
		throw new ExceptionHashTableAccess;
	}
}


/*
Method to return size of the hash table
*/
template <typename T>
unsigned long HashTable<T>::getSize() const {
	return size;
}


/*
Method to return the base capacity of the table
*/
template <typename T>
unsigned long HashTable<T>::getBaseCapacity() const {
	return baseCapacity;
}


/*
Method to return the total capacity of the table
*/
template <typename T>
unsigned long HashTable<T>::getTotalCapacity() const {
	return totalCapacity;
}


/*
Method to return the load factor of the table
*/
template <typename T>
float HashTable<T>::getLoadFactor() const {
	return (float) size / (float) totalCapacity;
}


/*
Method to return the bucket number of a given item
*/
template <typename T>
unsigned long HashTable<T>::getBucketNumber(T item) const {
	return hasher->hash(item) % baseCapacity;
}


/*
Method to increase the size of the hash table
*/
template <typename T>
void HashTable<T>::increaseHashTableSize() {

	//stores old base capacity
	unsigned long oldBaseCapacity = baseCapacity;

	//increment schedule index and reassign base and total capacity
	scheduleIndex++;
	baseCapacity = SCHEDULE[scheduleIndex];
	totalCapacity = baseCapacity;
	size = 0;

	//declares a new hash table of new base capacity size
	OULinkedList<T>** newTable = new OULinkedList<T> * [baseCapacity];

	//puts an OULinkedList in each bucket
	for (unsigned long i = 0; i < baseCapacity; i++) {
		newTable[i] = new OULinkedList<T>(comparator);
	}

	
	//iterates through the old hash table and rehashes all of the items
	for (unsigned long i = 0; i < oldBaseCapacity; i++) {

		//checks if table already contains something
		if (table[i]->getSize() > 0) {

			OULinkedListEnumerator<T> enumerator = table[i]->enumerator();

			//iterates through each linked list individually
			while (enumerator.hasNext()) {

				//gets current item and its hash value
				T currItem = enumerator.next();
				unsigned long newBucket = getBucketNumber(currItem);

				//if the bucket already has something, increase total capacity
				if (newTable[newBucket]->getSize() > 1) {
					totalCapacity++;
				}

				//insert the item into the right bucket
				newTable[newBucket]->insert(currItem);
				size++;

			}

			//delete the linked list at that index
			delete table[i];
		}

	}

	//delete old table and reassign pointer
	delete[] table;
	table = newTable;
}

/*
Method to decrease the size of the hash table
*/
template <typename T>
void HashTable<T>::decreaseHashTableSize() {

	//stores old base capacity
	unsigned long oldBaseCapacity = baseCapacity;

	//increment schedule index and reassign base and total capacity
	scheduleIndex--;
	baseCapacity = SCHEDULE[scheduleIndex];
	totalCapacity = baseCapacity;

	//declares a new hash table of new base capacity size
	OULinkedList<T>** newTable = new OULinkedList<T> * [baseCapacity];

	//puts an OULinkedList in each bucket
	for (unsigned long i = 0; i < baseCapacity; i++) {
		newTable[i] = new OULinkedList<T>(comparator);
	}

	//iterates through the old hash table and rehashes all of the items
	for (unsigned long i = 0; i < oldBaseCapacity; i++) {
		OULinkedListEnumerator<T> enumerator = table[i]->enumerator();

		//iterates through each linked list individually
		while (enumerator.hasNext()) {

			//gets current item and its hash value
			T currItem = enumerator.next();
			unsigned long newBucket = getBucketNumber(currItem);

			//if the bucket already has something, increase total capacity
			if (newTable[newBucket]->getSize() > 0) {
				totalCapacity++;
			}

			//insert the item into the right bucket
			newTable[newBucket]->insert(currItem);

		}

		//delete the linked list at that index
		delete table[i];
	}

	//delete old table and reassign pointer
	delete[] table;
	table = newTable;
}


#endif // !HASH_TABLE