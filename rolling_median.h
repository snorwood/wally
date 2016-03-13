/*
  rolling_median.h - Tracks a rolling median for a sample of data 
  Created by Team Wall-R-Us, March 11, 2016.
*/
#ifndef rolling_median_h
#define rolling_median_h

/*
Class: RollingMedian
Description: 	A median filter for a live stream of data
				Insert samples during experiment and get median
				Odd number of samples is recommended
Template:	T type of data to get median for
*/
const int null = 0;
template <class T>
class RollingMedian {
	friend class RollingMedianTest;

public:
	RollingMedian();
	RollingMedian(int max_size);	// Constructor
	~RollingMedian();				// Destructor

	/* Interface */
	void insertSample(T item);		
	T getMedian();

	/* Testing Functions */
	// void output();

private:
	/* Sorted list data structure */
	struct sortItem {
		T value;
		sortItem* next;
		sortItem* prev;

		/* Constructor */
		sortItem():next(null),prev(null){}
	};

	/* Input Ordered List Data Structure */
	struct inputItem {
		inputItem* next;
		inputItem* prev;
		sortItem* sorted;

		/* Constructor */
		inputItem():next(null),prev(null),sorted(null){}
	};
	
	/* Sample Set Properties */
	int size;
	int max_size;

	/* Input Ordered List Nodes */
	inputItem* inputHead;
	inputItem* inputTail;

	/* Sorted List Nodes */
	sortItem* sortHead;
	sortItem* sortTail;
};

/**************************************
				PUBLIC
**************************************/

/*
Function: 		RollingMedian
Description: 	Default constructor placeholder
*/
template <class T>
RollingMedian<T>::RollingMedian() {
}

/*
Function: 		RollingMedian
Description: 	Constructor initializes median tracking
Parameters:
	max_size:	Max number of samples to store median for
*/
template <class T>
RollingMedian<T>::RollingMedian(int max_size) {
	/* Initialize Sample Set Size */
	size = 0;
	this->max_size = max_size;

	/* Initialize Sorted List */
	sortHead = new sortItem();
	sortTail = new sortItem();
	sortHead->next = sortTail;
	sortTail->prev = sortTail;

	/* Initialize Input List */
	inputHead = new inputItem();
	inputTail = new inputItem();
	inputHead->next = inputTail;
	inputTail->prev = inputHead;
}

/*
Function: 		~RollingMedian
Description: 	Destructor frees up all space used by median filter
*/
template <class T>
RollingMedian<T>::~RollingMedian() {
	/* Delete Output Sorted List */
	sortItem* sortTrack = sortHead;
	while (sortTrack->next != sortTail) {
		sortTrack = sortTrack->next;
		delete sortTrack->prev;
	}
	delete sortTail;

	/* Delete Output Input List */
	inputItem* inputTrack = inputHead;
	while (inputTrack->next != inputTail) {
		inputTrack = inputTrack->next;
		delete inputTrack->prev;
	}
	delete inputTail;
}

/*
Function: 		insertSample
Description: 	Adds a new sample data point to the median filter
Parameters:
	value:		Value of type t to add to sample set
*/
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
		/* Explicitly define nodes to pop */
		inputItem* popInput = inputHead->next;
		sortItem* popSort = popInput->sorted;

		/* Pop the input list item */
		inputHead->next = popInput->next;

		/* Pop the sorted list item */
		popSort->prev->next = popSort->next;
		popSort->next->prev = popSort->prev;

		/* Free memory of popped nodes */
		delete popSort;
		delete popInput;

		/* Correct the current size */
		size -= 1;
	}

	/* Locate where to add new value in sorted list */
	while (sortTrack->next != sortTail && sortTrack->next->value < value)
		sortTrack = sortTrack->next;

	/* Insert the new value into sorted list */
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

	/* Correct current size */
	size += 1;
}

/*
Function: 		getMedian
Description: 	Calculates the median value for current sample set
Returns:		Median of type T
*/
template <class T>
T RollingMedian<T>::getMedian() {
	/* Handle 0 size case */
	if (size == 0)
		return 0;

	/* Find the middle sample point */
	sortItem* sortTrack = sortHead;
	for(int i = 0; i < (size + 1) / 2; i++)
		sortTrack = sortTrack->next;

	/* If even number of samples return average of two middle numbers */
	if (size % 2 == 0)
		return (sortTrack->value + sortTrack->next->value) / 2.0;

	return sortTrack->value;
}
#endif