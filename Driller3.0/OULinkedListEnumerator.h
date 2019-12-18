#pragma once
#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"
#include "Exceptions.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
	OULink<T>* current;
public:
	OULinkedListEnumerator(OULink<T>* first);
	bool hasNext() const;
	T next();			// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;		// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// Add your implementation below this line. Do not add or modify anything above this line.

/*
Constructor for an OULinkedListEnumerator object
takes a pointer to an OULink object as a parameter
*/
template <typename T>
OULinkedListEnumerator<T>::OULinkedListEnumerator(OULink<T>* first) {
	current = first;
}

/*
Method to check if the linked list has a next object
*/
template <typename T>
bool OULinkedListEnumerator<T>::hasNext() const {

	if (current != nullptr) {
		return true;
	}
	else {
		return false;
	}
}

/*
Method to get the next object in the linked list
*/
template <typename T>
T OULinkedListEnumerator<T>::next() {

	if (current != nullptr){//->next != nullptr) {
		T temp = current->data;
		current = current->next;
		return temp;
	}
	else {
		throw new ExceptionEnumerationBeyondEnd();
	}
}

/*
Method to get the next object in the list without removing it
*/
template <typename T>
T OULinkedListEnumerator<T>::peek() const {
	
	if (current != nullptr) {
		 
		return current->data;
	}
	else {
		throw new ExceptionEnumerationBeyondEnd();
	}
}
#endif // !OU_LINKED_LIST_ENUMERATOR