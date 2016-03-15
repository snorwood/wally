/*
  rolling_median.h - Tracks a rolling median for a sample of data 
  Created by Team Wall-R-Us, March 11, 2016.
*/
#ifndef rolling_median_h
#define rolling_median_h

const int DEFAULT_SS = 10;
const int REMOVE = 0x01, SWAP = 0x02;

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
	RollingMedian(int max_size=DEFAULT_SS);	// Constructor
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
	int inputHead;
	int inputTail;

	/* Sorted List */
	T* sortList;
};

/**************************************
				PUBLIC
**************************************/

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

	/* State of sorting algorithm */
	int state = 0x00;

	/* Cycle input queue */
	T pop = inputQueue[inputHead];			// Store potential pop value
	inputQueue[inputTail] = value;			// Push new value
	inputTail = (inputTail + 1) % max_size;	// Increment TAIL	
	size += 1;								// Increment size

	/* Handle adding a value when queue is full */
	if (size > max_size) {
		inputHead = (inputHead + 1) % max_size;	// Increment HEAD
		state |= REMOVE;						// Set state correctly
		size -= 1;
	}

	/* Insert new value into sorted list */
	int replace = value; // Value to insert into list at any time
	for(int i=0; i < size; i++) {

		/* Decide when to start swapping values */
		if ((state & REMOVE) && pop == sortList[i]) {
			state |= SWAP;
			state = state & (~REMOVE);
		}

		/* Swap when value is popped */
		if (state & SWAP) {
			sortList[i] = sortList[i+1];
		}

		/* Insert current replace value */
		if ((i == (size - 1) || replace < sortList[i])) {
			int temp = sortList[i];
			sortList[i] = replace;
			replace = temp;
		}
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

	/* Find middle of list */
	int i = (size - 1) / 2;

	/* Handle odd size case */
	if (size % 2)
		return sortList[i];
	
	/* Handle even size case */
	return (sortList[i] + sortList[i+1]) / 2.0;
}
#endif