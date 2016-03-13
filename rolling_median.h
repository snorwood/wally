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
public:
	RollingMedian(int max_size);	// Constructor
	~RollingMedian();				// Destructor

	/* Interface */
	void insertSample(T item);		
	T getMedian();

	/* Testing Functions */
	void output();

private:
	/* Sorted list data structure */
	struct sortItem {
		T value;
		sortItem* next;
		sortItem* prev;

		/* Constructor */
		sortItem():next(NULL),prev(NULL){}
	};


	/* Input Ordered List Data Structure */
	struct inputItem {
		inputItem* next;
		inputItem* prev;
		sortItem* sorted;

		/* Constructor */
		inputItem():next(NULL),prev(NULL),sorted(NULL){}
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

#endif