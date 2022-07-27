#ifndef BITONIC_H
#define BITONIC_H

#include<algorithm>
#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <array>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "Utilities.h"

class Bitonic
{
private:
	int N;
public:
    Bitonic(int N);
	virtual ~Bitonic();	

	void compAndSwap(int a[], int i, int j);
	void bitonicMerge(int a[], int low, int cnt, vector<int>&memseq);
	void bitMerge(int a[], int low, int cnt, vector<int>&memseq);
	void bitonicSort(int a[],int low, int cnt, vector<int>&memseq);
	void generateSeq(int a[], int N, vector<int>& memseq);
	vector<int> getSeq(int step, int count, int size);
	vector<int> remDup(vector<int> v);
};

#endif /* BITONIC_H */
