#include<bits/stdc++.h>
#include<math.h>
#include<stdio.h>

using namespace std;
void compAndSwap(int a[], int i, int j)
{
	if ((a[i]>a[j]))
		swap(a[i],a[j]);
}
vector<int> memseq;
void bitonicMerge(int a[], int low, int cnt)
{
	if (cnt>1)
	{
		int k = cnt/2;
		for (int i=low; i<low+k; i++)
		{
			compAndSwap(a, i, i+k);
			//cout <<i<<" "<<i+k<<endl;
			memseq.push_back(i);
			memseq.push_back(i+k);
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
			memseq.push_back(i);
			memseq.push_back(j);
		}
		bitonicMerge(a, low, k);
		bitonicMerge(a, low+k, k);
	}
}
void bitonicSort(int a[],int low, int cnt)
{
	if (cnt>1)
	{
		int k = cnt/2;
		bitonicSort(a, low, k);
		bitonicSort(a, low+k, k);
		bitMerge(a,low, cnt);
	}
}

void sort(int a[], int N, int up)
{
	bitonicSort(a,0, N);
}

vector<int> getSeq(int count, int step, int N)
{
	int a[N];
	memset(a,0,N);
	memseq.clear();
	bitonicSort(a,0,N);
	cout <<"memseq size:"<<memseq.size()<<endl;
	int start = count*step;
	vector<int> res;
	for(int i = start; i<start+step; i++)
	{
		res.push_back(memseq[i]);
	}
	return res;
}
void deamortizeBitonicSort(int step, int count, vector<int>& p, int N)
{
		vector<int> curMem = getSeq(count, step, N);
		std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
		vector<int> elToSort;
		for(auto el : curMem)
		{
			elToSort.push_back(p[el]);
		}
		std::sort(elToSort.begin(), elToSort.end(), [](int a, int b) {return a < b;});
		int cnt = 0;
		for(int i =0; i<curMem.size(); i++)
		{
			p[curMem[i]] = elToSort[cnt];
			cnt++;
		}
}

int main()
{
	vector<int> p = {3, 7, 4, 8, 6, 2, 1, 5, 9,13,12,16,14,10,15,11, 17,18,19,20,31,32,30,28,26,29,21,25,22,24,23,27};
	int N = p.size();

	int step = 4; // means sort in ascending order

	//for (int i=0; i<N; i++)
	//	printf("%d ", a[i]);;
	int totStep = 2*((float)(N*log2(N)*(log2(N)+1))/(float) 4);
	totStep = ((float)totStep)/((float)step);
	cout <<"totStep:"<<totStep<<endl;
	for(int count = 0; count<totStep; count++)
	{
		deamortizeBitonicSort(step, count, p, N);
	}
	printf("Sorted array: \n");
	for (int i=0; i<N; i++)
		printf("%d ", p[i]);

	return 0;
}

