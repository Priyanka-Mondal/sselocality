#include<bits/stdc++.h>
#include<math.h>
#include<stdio.h>
#include<chrono>
#include<unistd.h>

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
	//cout <<"memseq size:"<<memseq.size()<<endl;
	int start = count*step;
	vector<int> res;
	for(int i = start; i<start+step; i++)
	{
		res.push_back(memseq[i]);
	}
	return res;
}
vector<int> remDup(vector<int> v)
{
	int vsize = v.size();
	vector<int>::iterator ip;
    ip = std::unique(v.begin(), v.begin() +vsize );
    v.resize(std::distance(v.begin(), ip));
	return v;
}
void deamortizeBitonicSort(int step, int count, vector<int>& p, int N)
{
		vector<int> curMem = getSeq(count, step, N);
	//	for(int s=0; s<curMem.size();s++)
	//		cout <<"(("<<curMem[s]<<"))";
	//	cout <<endl;
		std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
	//	for(int s=0; s<curMem.size();s++)
	//		cout <<"[["<<curMem[s]<<"]]";
	//	cout <<endl;
		vector<int> ncm = remDup(curMem);
	//	for(int s=0; s<ncm.size();s++)
	//		cout <<"{{"<<ncm[s]<<"}}";
	//		cout <<endl;
		vector<int> elToSort;
		for(auto el : ncm)
		{
			elToSort.push_back(p[el]);
	//		cout<<"("<<el<<")";
		}
	//	cout <<endl;
	//	for(int s=0; s<elToSort.size();s++)
	//		cout <<"["<<elToSort[s]<<"]";
	//	cout <<endl;
		std::sort(elToSort.begin(), elToSort.end(), [](int a, int b) {return a < b;});
	//	for(int s=0; s<elToSort.size();s++)
	//		cout <<"{"<<elToSort[s]<<"}";
	//	cout <<endl;
	//	cout <<endl;
		int cnt = 0;
		for(int i =0; i<ncm.size(); i++)
		{
			p[ncm[i]] = elToSort[cnt];
			cnt++;
		}
}

int issorted(vector<int> A, vector<int> B)
{
	for(int a=0;a<A.size();a++)
	{
		int bina = A[a];
		int binb = B[a];
		if(bina!=binb)
			return 0;
	}
	return 1;
}
void bubble(vector<int>& A)
{
 for (int i = 0; i < A.size(); i++)
 {
        for (int j = 0; j < A.size() - i - 1; j++)
		{
    		int bina = A[j];
    		int binb = A[j+1];
            if (bina>binb)
			{
				int temp;
				temp = A[j];
				A[j] = A[j+1];
				A[j+1] = temp;
			}
		}
 }
}

int main()
{
	//vector<int> p = {3, 7, 4, 8, 6, 2, 1, 5, 9,13,12,16,14,10,15,11, 17,18,19,20,31,32,30,28,26,29,21,25,22,24,23,27};
	//vector<int> pb = {3, 7, 4, 8, 6, 2, 1, 5, 9,13,12,16,14,10,15,11, 17,18,19,20,31,32,30,28,26,29,21,25,22,24,23,27};

    srand(time(NULL));
	int N = 256;
	for(int N = 1024; N<=4096; N=N*2)
	{	
		vector<int> p;
		vector<int> pb;
		for(int k = 0; k< N; k++)
		{
	    	int fileName = ((rand() % 10000)); //////here
			p.push_back(fileName);
			pb.push_back(fileName);
		}
		for(int step = 2; step <=N; step=step+2)
		{
		int totStep = 2*((float)(N*log2(N)*(log2(N)+1))/(float) 4);
		totStep = ((float)totStep)/((float)step);
		cout <<"totStep:"<<totStep<<endl;
		for(int count = 0; count<totStep; count++)
		{
			deamortizeBitonicSort(step, count, p, N);
		}
		bubble(pb);
		/*
		printf("Sorted array: \n");
		for (int i=0; i<N; i++)
			printf("%d ", p[i]);
		cout <<endl;
		cout <<endl;
		for (int i=0; i<N; i++)
			printf("%d ", pb[i]);
		cout <<endl;*/
		assert(issorted(p,pb));
		//sleep(1);
		cout <<"N:"<<N<<" step:"<<step<<"--SORTED"<<endl;
	}
	}
	return 0;
}

