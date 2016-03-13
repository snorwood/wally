/*
  rolling_median.h - Tracks a rolling median for a sample of data 
  Created by Team Wall-R-Us, March 11, 2016.
*/
#include <iostream>
#include "rolling_median_test.h"
#include "rolling_median.h"

using namespace std;

RollingMedianTest::RollingMedianTest(){}

/*
Function: 		output
Description: 	Outputs current sample set information to cout
*/
void RollingMedianTest::output(RollingMedian<float>* rm) {
	/* Output Size */
	cout << "SIZE: " << rm->size << "\n";

	/* Output Sorted List */
	RollingMedian<float>::sortItem* sortTrack = rm->sortHead;
	cout << "SORTED:";
	while (sortTrack->next != rm->sortTail) {
		sortTrack = sortTrack->next;
		cout << " " << sortTrack->value;
	}
	cout << "\n";

	/* Output Input List */
	RollingMedian<float>::inputItem* inputTrack = rm->inputHead;
	cout << "INPUT:";
	while (inputTrack->next != rm->inputTail) {
		inputTrack = inputTrack->next;
		cout << " " << inputTrack->sorted->value;
	}
	cout << "\n";

	/* Output Median */
	cout << "MEDIAN: " << rm->getMedian() << "\n";
}	

/* Test demo */
int main() {
	RollingMedianTest test;
	RollingMedian<float>* a = new RollingMedian<float>(5);

	test.output(a);

	a->insertSample(5);
	a->insertSample(10);
	a->insertSample(11);
	a->insertSample(4);
	a->insertSample(5);
	test.output(a);

	a->insertSample(8);
	a->insertSample(16);
	test.output(a);

	return 0;
}