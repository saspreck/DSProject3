#pragma once
#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
	template <typename U>
	friend class OULinkedListEnumerator;
private:
	Comparator<T>* comparator = NULL;               // used to determine list order and item equality
	unsigned long size = 0;                         // actual number of items currently in list
	OULink<T>* first = NULL;                        // pointer to first link in list
	OULink<T>* last = NULL;                         // pointer to last link in list
public:
	OULinkedList(Comparator<T>* comparator);        // creates empty linked list with comparator
	virtual ~OULinkedList();                        // deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave list unchanged and return false
	bool insert(T item);

	// if item is greater than item at last, append item at end and return true
	// if item is less than or equal to item at last, leave list unchanged and return false
	bool append(T item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool replace(T item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool remove(T item);

	// if any items are present, return a copy of the first item
	// if no items are present, throw new ExceptionLinkedListAccess
	T getFirst() const;

	// if any items are present, return a copy of the first item, remove it from list
	// if no items are present, throw new ExceptionLinkedListAccess
	T pullFirst();

	// if any items are present, remove the first item and return true
	// if no items are present, leave list unchanged and return false
	bool removeFirst();

	// if an equivalent item is present, return true
	// if an equivalent item is not present, false
	bool contains(T item) const;

	// if an equivalent item is present, return a copy of that item
	// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
	T find(T item) const;

	void clear();                                     // deletes all links in the list, resets size to 0

	unsigned long getSize() const;                    // returns the current number of items in the list

	OULinkedListEnumerator<T> enumerator() const;     // create an enumerator for this linked list
};

// Add your implementation below this line. Do not add or modify anything above this line.

/*
Constructor for an OULinkedList, takes a comparator as a parameter
*/
template <typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator) {
	this->comparator = comparator;
}

/*
Destructor for an OULinkedList, deletes the head and tail
*/
template <typename T>
OULinkedList<T>::~OULinkedList() {
	
	delete first;

	first = nullptr;
	last = nullptr;
}

/*
Method to insert new link at correct location
*/
template <typename T>
bool OULinkedList<T>::insert(T item) {

	//creates new link with given item
	OULink<T>* newNode = new OULink<T>(item);

	//creates two links to iterate through list with
	OULink<T>* current = first;

	//checks if list is empty
	if (first != nullptr) {

		//checks if data is less than first node
		if (comparator->compare(first->data, item) == 1) {
			//inserts node at head
			newNode->next = first;
			first = newNode;
			size++;
			return true;
		}
		//checks if data equals first node and returns false
		else if (comparator->compare(first->data, item) == 0)
		{
			delete newNode;
			return false;
		} 

		//iterates through until there is not next link
		while (current->next != nullptr) {

			//if item is less than the next link, insert at that position
			if (comparator->compare(current->next->data, item) == 1) {
				newNode->next = current->next;
				current->next = newNode;
				size++;
				return true;
			}
			//if data already exists, return false
			else if(comparator->compare(current->next->data, item) == 0) {
				delete newNode;
				return false;
			}

			//iterates to next link
			current = current->next;
		}
		//else add item to the end of the list
		return append(item);
	}
	//if list empty then add at head
	else {
		first = newNode;
		last = newNode;
		size++;
		return true;
	}

	//returns false if item is not inserted
	delete newNode;
	return false;
}


/*
Method to add an object to end of list if greater than current last object
*/
template <typename T>
bool OULinkedList<T>::append(T item) {
	
	//creates new node to add into list
	OULink<T>* newNode = new OULink<T>(item);

	//checks that list is not empty
	if (last != nullptr) {

		//if item is greater than last, put at end and move pointer, return true
		if (comparator->compare(item, last->data) == 1) {
			
			last->next = newNode;
			last = newNode;
			size++;
			return true;
		}

		//if item is not greater, delete newNode and return false
		else {
			delete newNode;
			newNode = nullptr;
			return false;
		}
	}
	
	//if list empty put node at head
	else {
		first = newNode;
		last = newNode;
		size++;
		return true;
	}
}


/*
Method to replace an existing object in the list if it exists
*/
template <typename T>
bool OULinkedList<T>::replace(T item) {

	//check if list is empty
	if (first != nullptr) {

		//create new link to iterate through list with
		OULink<T>* current = first;

		//if data equals first node, replace
		if (comparator->compare(current->data, item) == 0) {
			current->data = item;
			return true;
		}

		//iterates through until the current link has no next
		while (current->next != nullptr) {

			//if item matches current, replace
			if (comparator->compare(current->data, item) == 0) {
				current->data = item;
				return true;
			}

			//iterates links to next
			current = current->next;
		}
	}
	//return false if not in list
	return false;

}


/*
Method to remove the given item from the list
*/
template <typename T>
bool OULinkedList<T>::remove(T item) {

	//creates two new links to iterate through list with
	OULink<T>* current = first;
	OULink<T>* previous= nullptr;

	//checks if list is empty
	if (first != nullptr) {

		//checks if first link is one to be removed
		if (comparator->compare(first->data, item) == 0) {

			//pull first node out, move first pointer, and delete first node
			OULink<T>* temp = first;
			first = first->next;
			temp->next = NULL;
			delete temp;
			size--;
			return true;
		}

		//iterates through until the current link has no next
		while (current != nullptr) {

			//if item matches current and previous is an link, remove current link
			if (comparator->compare(current->data, item) == 0) {

				//pull out nodes, reassign pointers, and delete
				previous->next = current->next;
				current->next = nullptr;
				delete current;
				size--;
				return true;
			}
			//iterates links to next
			previous = current;
			current = current->next;
		}

	}
	//return false if nothing removed
	return false;
	
}


/*
Method to return the first object in the list without removing it
*/
template <typename T>
T OULinkedList<T>::getFirst() const {

	//if list not empty return first object
	if (first != nullptr) {
		return first;
	}
	//throw error if list empty
	else {
		throw new ExceptionLinkedListAccess();
	}
}

/*
Method to remove and return the first object in the list
*/
template <typename T>
T OULinkedList<T>::pullFirst() {

	//if list not empty remove and return first object
	if (first != nullptr) {
		T newNode = first->data;
		first = first->next;
		size--;
		return newNode;
	}
	//throw error if list empty
	else {
		throw new ExceptionLinkedListAccess();
	}
}

/*
Method to remove the first object in the list without returning it
*/
template <typename T>
bool OULinkedList<T>::removeFirst() {

	//checks if list is empty, adjusts pointer, and returns true
	if (first != nullptr) {
		OULink<T>* temp = first;
		first = first->next;
		temp->next = NULL;
		delete temp;
		size--;
		return true;
	}
	//returns false if list is empty
	else {
		return false;
	}
}


/*
Method to check if the list contains a given object
*/
template <typename T>
bool OULinkedList<T>::contains(T item) const {

	//checks if list is empty
	if (first != nullptr) {

		//creates link to iterate through list with
		OULink<T>* current = first;

		//iterates until the link has no next
		while (current != nullptr) {

			//if items match, return true
			if (comparator->compare(current->data, item) == 0) {
				return true;
			}

			//iterates link to next link
			current = current->next;
		}
		//returns false if item not found in list
		return false;
	}
	//returns false if list is empty
	else {
		return false;
	}
	
}

/*
Method to find item within the list
*/
template <typename T>
T OULinkedList<T>::find(T item) const {

	//checks if list is empty
	if (first != nullptr) {

		//creates link to iterate through list with
		OULink<T>* current = first;

		//iterates until the link has no next
		while (current != nullptr) {

			//if items match, return copy of that data
			if (comparator->compare(current->data, item) == 0) {
				return current->data;
			}

			//iterates link to next link
			current = current->next;
		}
		//throws exception if item is not found
		throw new ExceptionLinkedListAccess();
	}
	//throws exception if item is not found
	else {
		throw new ExceptionLinkedListAccess();
	}
}

/*
Method to clear the list and set size to 0
*/
template <typename T>
void OULinkedList<T>::clear() {

	//checks if list is not empty
	if (first != nullptr) {

		//creates new link to iterate through with
		OULink<T>* current = first;

		//iterates through list until there is no next link
		while (current->next != nullptr) {

			//sets a link to the next link
			OULink<T>* temp = current->next;

			//deletes the current link and moves to the next one
			delete current;
			current = temp;
		}

		//sets first and last to null pointers
		first = nullptr;
		last = nullptr;
		//sets size back to 0
		size = 0;
	}
	
}

/*
Method to return the size of the linked list
*/
template <typename T>
unsigned long OULinkedList<T>::getSize() const {
	return size;
}

/*
Declares an enumerator for the linked list to use
*/
template<typename T>
inline OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const
{
	return OULinkedListEnumerator<T>(first);
}
#endif // !OU_LINKED_LIST