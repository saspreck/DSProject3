#pragma once
#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
	unsigned long bucket = 0;
	OULinkedListEnumerator<T>* chainEnumerator = NULL;
	HashTable<T>* hashTable;
public:
	HashTableEnumerator(HashTable<T>* hashTable);
	virtual ~HashTableEnumerator();
	bool hasNext() const;
	T next();              // throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;        // throws ExceptionEnumerationBeyondEnd if no next item is available
};

// Add your implementation below this line.

/*
Constructor for a hash table enumerator, takes a hash table parameter
*/
template <typename T>
HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable) {

	//checks if hash table is empty or a null pointer
	if (hashTable->getSize() == 0 || hashTable == nullptr) {
		throw new ExceptionHashTableAccess();
	}
	this->hashTable = hashTable;

	//finds first bucket with something
	while (hashTable->table[bucket]->getSize() == 0) {
		bucket++;
	}
	//sets chain enum to the enumerator in that bucket
	OULinkedListEnumerator<DrillingRecord> tempEnum = hashTable->table[bucket]->enumerator();
	chainEnumerator = new OULinkedListEnumerator<DrillingRecord>(tempEnum);
}

/*
Destructor for a hash table enumerator
*/
template <typename T>
HashTableEnumerator<T>::~HashTableEnumerator() {
	delete hashTable;
	hashTable = nullptr;
}

/*
Method to see if the hash table has a next value
*/
template <typename T>
bool HashTableEnumerator<T>::hasNext() const {

	//checks if at end of linked list and in last bucket
	if (chainEnumerator->hasNext() == false && bucket == hashTable->getBaseCapacity() - 1) {
		return false;
	}
	//checks if at end of linked list and not at last bucket
	else if (chainEnumerator->hasNext() == false && bucket < hashTable->getBaseCapacity() - 1) {

		//iterate bucket from current one
		unsigned long tempBucket = bucket;
		tempBucket++;
		
		//iterates through the table until it finds a bucket with a linked list
		while (tempBucket < hashTable->getBaseCapacity()) {
			
			OULinkedList<DrillingRecord>* tempList = hashTable->table[tempBucket];
			//if bucket contains an item return true
			if (tempList->getSize() != 0) {
				return true;
			}
			tempBucket++;
		}
		//if no item is found return false
		return false;
	}
	//if not at end of linked list return true
	else if (chainEnumerator->hasNext() == true) {
		return true;
	}
	//return false if all tests fail
	return false;
}

/*
Method to return the current data and iterate to the next object
*/
template <typename T>
T HashTableEnumerator<T>::next() {

	//checks if table has a next value
	if (hasNext() == false) {
		throw new ExceptionEnumerationBeyondEnd;
	}
	//if linked list has next return next
	else if (chainEnumerator->hasNext() == true) {
		return chainEnumerator->next();
	}
	//checks if at end of linked list and not at last bucket
	else {

		bucket++;
		//if bucket outside table range, throw exception
		if (bucket >= hashTable->getBaseCapacity()) {
			throw new ExceptionEnumerationBeyondEnd;
		}

		//iterates through the table until it finds a bucket with a linked list
		while (bucket < hashTable->getBaseCapacity() - 1 && hashTable->table[bucket]->getSize() == 0) {

			//iterate bucket
			bucket++;
		}

		OULinkedList<DrillingRecord>* tempList = hashTable->table[bucket];
		//if bucket contains an item return 
		if (bucket < hashTable->getBaseCapacity()) {
			*chainEnumerator = tempList->enumerator();
			return chainEnumerator->next();
		}
		else {
			throw new ExceptionEnumerationBeyondEnd;
		}
	}
	
	//throw exception if all else fails
	throw new ExceptionEnumerationBeyondEnd;

}

/*
Method to return the current object in the table
*/
template <typename T>
T HashTableEnumerator<T>::peek() const {
	
	//checks if at end of linked list and in last bucket
	if (chainEnumerator->hasNext() == false && bucket == hashTable->getBaseCapacity() - 1) {
		throw new ExceptionEnumerationBeyondEnd;
	}
	//checks if at end of linked list and not at last bucket
	else if (chainEnumerator->hasNext() == false && bucket < hashTable->getBaseCapacity() - 1) {

		//iterate bucket from current one
		unsigned long tempBucket = bucket;
		tempBucket++;

		//iterates through the table until it finds a bucket with a linked list
		while (tempBucket < hashTable->getBaseCapacity()) {

			OULinkedList<DrillingRecord> *tempList = hashTable->table[tempBucket];
			
			//if bucket contains an item call linked list peek
			if (tempList->getSize() != 0) {
				OULinkedListEnumerator<DrillingRecord> tempEnum = tempList->enumerator();
				return tempEnum.peek();
			}
			tempBucket++;
		}
		//if no item is found throw exception
		throw new ExceptionEnumerationBeyondEnd;
	}
	//if not at end of linked list call enum peek
	else if (chainEnumerator->hasNext() == true) {
		return chainEnumerator->peek();
	}
	//throw exception
	throw new ExceptionEnumerationBeyondEnd;
}


#endif // !HASH_TABLE_ENUMERATOR