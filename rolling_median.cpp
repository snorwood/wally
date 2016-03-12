#include "rolling_median.h"
#include <iostream>

using namespace std;

template <class T>
RollingMedian<T>::RollingMedian(int max_size) {
	size = 0;
	this->max_size = max_size;

	sortHead = new sortItem();
	sortTail = new sortItem();
	sortHead->next = sortTail;
	sortTail->prev = sortTail;

	inputTail = new inputItem();
	inputTail->sorted = sortTail;
	
	inputHead = new inputItem();
	inputHead->sorted = sortHead;
	inputHead->next = inputTail;
	inputTail->prev = inputHead;
}

template <class T>
void RollingMedian<T>::insertSample(T value) {
	/* Prevent people trying to break it */
	if (max_size <= 0)
		return;

	/* Initialize temporary tracking variables */
	inputItem* inputTrack = inputHead;
	sortItem* sortTrack = sortHead;

	/* Initialize new sorted list item */
	sortItem* newSort = new sortItem();
	newSort->value = value;

	/* Initialize new input list item */
	inputItem* newInput = new inputItem();

	/* Pop an item from the list if necessary */
	if (size == max_size) {
		inputItem* popInput = inputHead->next;
		sortItem* popSort = popInput->sorted;

		inputHead->next = popInput->next;

		popSort->prev->next = popSort->next;
		popSort->next->prev = popSort->prev;

		delete popSort;
		delete popInput;
		size -= 1;
	}

	/* Insert the new value into sorted list */
	while (sortTrack->next != sortTail && sortTrack->next->value < value)
		sortTrack = sortTrack->next;

	newSort->next = sortTrack->next;
	newSort->prev = sortTrack;
	sortTrack->next = newSort;
	newSort->next->prev = newSort;

	/* Insert the new value into the input queue */
	newInput->sorted = newSort;
	newInput->prev = inputTail->prev;
	newInput->next = inputTail;
	newInput->prev->next = newInput;
	inputTail->prev = newInput;
	size += 1;
}

template <class T>
T RollingMedian<T>::getMedian() {
	if (size == 0)
		return 0;

	sortItem* sortTrack = sortHead;
	for(int i = 0; i < (size + 1) / 2; i++)
		sortTrack = sortTrack->next;

	if (size % 2 == 0)
		return (sortTrack->value + sortTrack->next->value) / 2.0;

	return sortTrack->value;
}

template <class T>
void RollingMedian<T>::output() {
	cout << "SIZE: " << size << "\n";

	sortItem* sortTrack = sortHead;
	cout << "SORTED:";
	while (sortTrack->next != sortTail) {
		sortTrack = sortTrack->next;
		cout << " " << sortTrack->value;
	}
	cout << "\n";

	inputItem* inputTrack = inputHead;
	cout << "INPUT:";
	while (inputTrack->next != inputTail) {
		inputTrack = inputTrack->next;
		cout << " " << inputTrack->sorted->value;
	}
	cout << "\n";

	cout << "MEDIAN: " << getMedian() << "\n";
}	

/* Test demo */
// int main() {
// 	RollingMedian<int> a = RollingMedian<int>(5);
// 	a.output();

// 	a.insertSample(5);
// 	a.insertSample(10);
// 	a.insertSample(11);
// 	a.insertSample(4);
// 	a.insertSample(5);
// 	a.output();

// 	a.insertSample(8);
// 	a.insertSample(16);
// 	a.output();
// 	return 0;
// }