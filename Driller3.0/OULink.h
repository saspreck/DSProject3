#pragma once
#ifndef OU_LINK_H
#define OU_LINK_H

template <typename T>
class OULink {
	template <typename U>
	friend class OULinkedList;
	template <typename U>
	friend class OULinkedListEnumerator;
private:
		T data{};                  // data item of any type
	OULink* next = NULL;       // pointer to next link
public:
	OULink(T item);
	virtual ~OULink();
};

// Add your implementation below this line. Do not add or modify anything above this line.

/*
Constructor for an OULink object, takes an item as its parameter
*/
template <typename T>
OULink<T>::OULink(T item) {
	data = item;
}

/*
Destructor for an OULink object, clears the next pointer
*/
template <typename T>
OULink<T>::~OULink() {
	delete next;
	next = nullptr;
	
}

#endif // !OU_LINK