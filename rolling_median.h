/*
  rolling_median.h - Tracks a rolling median for a sample of data 
  Created by Team Wall-R-Us, March 11, 2016.
*/
#ifndef rolling_median_h
#define rolling_median_h

const int NULL = 0;

template <class T>
class RollingMedian {
public:
	RollingMedian(int max_size);
	void insertSample(T item);
	T getMedian();
	void output();

private:
	struct sortItem {
		T value;
		sortItem* next;
		sortItem* prev;

		sortItem():next(NULL),prev(NULL){}
	};

	struct inputItem {
		inputItem* next;
		inputItem* prev;
		sortItem* sorted;

		inputItem():next(NULL),prev(NULL),sorted(NULL){}
	};
	
	int size;
	int max_size;
	inputItem* inputHead;
	inputItem* inputTail;
	sortItem* sortHead;
	sortItem* sortTail;
};

#endif