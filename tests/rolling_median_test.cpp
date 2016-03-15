/*
  rolling_median.h - Tracks a rolling median for a sample of data 
  Created by Team Wall-R-Us, March 11, 2016.
*/
#include <iostream>
#include "rolling_median_test.h"
#include "../rolling_median.h"

using namespace std;

RollingMedianTest::RollingMedianTest(){}

/*
Function: 		output
Description: 	Outputs current sample set information to cout
*/
void RollingMedianTest::output(RollingMedian<float>* rm) {
	/* Output Size */
	cout << "\n" << "SIZE: " << rm->size << "\n";

	/* Output Input List */
	cout << "INPUT:";
	for (int i = 0; i < rm->size; i++)
		cout << " " << rm->inputQueue[i];
	cout << "\n";

	/* Output Sorted List */
	cout << "SORTED:";
	for (int i = 0; i < rm->size; i++)
		cout << " " << rm->sortList[i];
	cout << "\n";

	/* Output Median */
	cout << "MEDIAN: " << rm->getMedian() << "\n";
}	

/* Test demo */
int main() {
	RollingMedianTest test;
	RollingMedian<float>* a = new RollingMedian<float>(6);

	test.output(a);

	a->insertSample(3);
	a->insertSample(5);
	a->insertSample(2);
	a->insertSample(8);
	a->insertSample(10);
	a->insertSample(0);
	test.output(a);

	a->insertSample(7);
	test.output(a);

	a->insertSample(1);
	a->insertSample(3);
	a->insertSample(34);
	a->insertSample(1);
	a->insertSample(555);
	a->insertSample(421);
	test.output(a);

	return 0;
}