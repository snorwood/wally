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

private:
	/* Sample Set Properties */
	int size;
	int max_size;

	/* Input Ordered Queue */
	T* inputQueue;
	int inputHead = 0;
	int inputTail = 0;

	/* Sorted List */
	T* sortList;
};

/**************************************
				PUBLIC
**************************************/

/*
Function: 		RollingMedian
Description: 	Default constructor placeholder
*/
template <class T>
RollingMedian<T>::RollingMedian() : RollingMedian(10) {
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
	sortList = new T[max_size];

	/* Initialize Input Queue */
	inputQueue = new T[max_size];
	inputHead = 0;
	inputTail = 0;
}

/*
Function: 		~RollingMedian
Description: 	Destructor frees up all space used by median filter
*/
template <class T>
RollingMedian<T>::~RollingMedian() {
	delete inputQueue;
	delete sortList;
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

	/* Cycle Input Queue */
	T pop = inputQueue[inputHead];
	inputQueue[inputTail] = value;
	inputTail = (inputTail + 1) % max_size;
	size += 1;
	if (size > max_size) {
		inputHead = (inputHead + 1) % max_size;
		size -= 1;
	}
	
	int inserted = size == max_size;
	int removed = inserted;
	int replace = value;
	int i = 0;

	while (!inserted && !removed) {
		if (i == size || replace < sortList[i]) {
			int temp = sortList[i];
			sortList[i] = replace;
			replace = temp;
		}
		i++;
	}
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

	return 1;
}
#endif