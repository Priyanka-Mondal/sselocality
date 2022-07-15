#include<bits/stdc++.h>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
 

using namespace std;


vector<int> memAcc;
void compAndSwap(int a[], int i, int j, int dir)
{
	if (dir==(a[i]>a[j]))
		swap(a[i],a[j]);
}
void bitonicMerge(int a[], int low, int cnt, int dir, int N)
{
	if (cnt>1)
	{
		int k = cnt/2;
		for (int i=low; i<low+k; i++)
		{
			compAndSwap(a, i, i+k, dir);
			memAcc.push_back(i);
			memAcc.push_back(i+k);
		}
		bitonicMerge(a, low, k, dir);
		bitonicMerge(a, low+k, k, dir);
	}
}
void bitonicSort(int a[],int low, int cnt, int dir)
{
	if (cnt>1)
	{
		int k = cnt/2;

		// sort in ascending order since dir here is 1
		bitonicSort(a, low, k, 1);

		// sort in descending order since dir here is 0
		bitonicSort(a, low+k, k, 0);

		// Will merge whole sequence in ascending order
		// since dir=1.
		bitonicMerge(a,low, cnt, dir);
	}
}
void sort(int a[], int N, int up)
{
	bitonicSort(a,0, N, up);
}

// Driver code
vector<int> getSeq(int count, int step)
{
	int start = count*step;
	cout <<"count:"<<count<<"step:"<<step<<" start:"<<start<<" "<<memAcc[start]<<endl;
	vector<int> res;
	for(int i = start; i<start+step; i++)
	{
		res.push_back(memAcc[i]);
		cout <<"("<<memAcc[i]<<")";
	}
	cout <<endl;
	return res;
}

void memseq(int a[] , int n)
{
for (int k = 2; k <= n; k= k*2)
{ 
        for (int j = k/2; j > 0; j /= 2)
		{ // j is halved at every iteration, with truncation of fractional parts
            for (int i = 0; i < n; i++)
			{
                int l = i^j; // in C-like languages this is "i ^ j"
                if (l > i)
				{
						memAcc.push_back(i);
						memAcc.push_back(l);
                    if ((((i & k) == 0) && (a[i] > a[l])) || (((i&k) != 0) && (a[i] < a[l])))
					{
						int temp = a[i];
						a[i]=a[l];
						a[l]=temp;
					}
				}
			}
		}
}
}

int main()
{
	int p[]= {3, 7, 4, 11, 6, 0, 5, 99};
	int N = sizeof(p)/sizeof(p[0]);
	int a[10];
	memset(a,0,N);

	int up = 1; // means sort in ascending order
	//sort(a, N, up);
	memseq(a,N);
	int step = 4;

	for(int i=0; i<memAcc.size(); i++)
		cout <<memAcc[i] <<" ";
	cout <<endl;
	
	int totmemAcc = memAcc.size();
	int totStep = ceil((float)totmemAcc	/(float)step);
	cout <<"tptStep:"<<totStep<<endl;
	for(int count = 0; count<totStep; count++)
	{
		cout<<"count:"<<count<<endl;
		vector<int> curMem;
		curMem = getSeq(count, step);
		cout <<"size of surMem:"<<curMem.size()<<endl;
		vector<int> elToSort;
		std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
		for(auto el : curMem)
		{
			cout <<"["<<p[el]<<"]";
			elToSort.push_back(p[el]);
		}
		cout <<endl;
		std::sort(elToSort.begin(), elToSort.end(), [](int a, int b) {return a < b;});
		cout <<endl;
		int cnt = 0;
		for(int i =0; i<curMem.size(); i++)
		{
			p[curMem[i]] = elToSort[cnt];
			cnt++;
		}
		for(int i =0; i<curMem.size(); i++)
		{
			cout<<"{"<<p[curMem[i]]<<"}";
		}
		cout <<endl;
	}
	for (int i=0; i<N; i++)
		printf("%d ", p[i]);

	return 0;
}
