#include<bits/stdc++.h>
using namespace std;

/*The parameter dir indicates the sorting direction, ASCENDING
or DESCENDING; if (a[i] > a[j]) agrees with the direction,
then a[i] and a[j] are interchanged.*/
void compAndSwap(int a[], int i, int j)
{
	if ((a[i]>a[j]))
		swap(a[i],a[j]);
}

/*It recursively sorts a bitonic sequence in ascending order,
if dir = 1, and in descending order otherwise (means dir=0).
The sequence to be sorted starts at index position low,
the parameter cnt is the number of elements to be sorted.*/
void bitonicMerge(int a[], int low, int cnt)
{
	if (cnt>1)
	{
		int k = cnt/2;
		for (int i=low; i<low+k; i++)
		{
			compAndSwap(a, i, i+k);
			cout <<i<<" "<<i+k<<endl;
		}
		bitonicMerge(a, low, k);
		bitonicMerge(a, low+k, k);
	}
}

void bitMerge(int a[], int low, int cnt)
{
	if (cnt>1)
	{
		int k = cnt/2;
		for (int i=low, j = low+cnt-1; i<low+k,j>=low+k; i++,j--)
		{
			compAndSwap(a, i, j);
			cout <<i<<" "<<j<<endl;
		}
		bitonicMerge(a, low, k);
		bitonicMerge(a, low+k, k);
	}
}

/* This function first produces a bitonic sequence by recursively
	sorting its two halves in opposite sorting orders, and then
	calls bitonicMerge to make them in the same order */
void bitonicSort(int a[],int low, int cnt)
{
	if (cnt>1)
	{
		int k = cnt/2;

		// sort in ascending order since dir here is 1
		bitonicSort(a, low, k);

		// sort in descending order since dir here is 0
		bitonicSort(a, low+k, k);

		// Will merge whole sequence in ascending order
		// since dir=1.
		bitMerge(a,low, cnt);
	}
}

void sort(int a[], int N, int up)
{
	bitonicSort(a,0, N);
}

int main()
{
	int a[]= {3, 7, 4, 8, 6, 2, 1, 5, 9,13,12,16,14,10,15,11, 17,18,19,20,31,32,30,28,26,29,21,25,22,24,23,27
					};
	int N = sizeof(a)/sizeof(a[0]);

	int up = 1; // means sort in ascending order
	sort(a, N, up);

	printf("Sorted array: \n");
	for (int i=0; i<N; i++)
		printf("%d ", a[i]);
	return 0;
}

