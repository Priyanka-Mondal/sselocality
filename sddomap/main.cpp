#include<iostream>
#include<vector>
#include "Bitonic.h"

using namespace std;

bool cmpp(int &a, int &b)
{
	return (a < b);
}

bool cmpp2(int &a, int &b)
{
	return (a > b);
}
int main()
{
	Bitonic* bitonic = new Bitonic();
	vector<int> encKW = {1, 8, 3, 5, 7, 4, 2, 6, 10, 12, 16, 13, 14, 11, 9, 15};
	int totTimes = 160;
	int turns = 110;
	int step = (float)totTimes/(float)turns;
	if(step <= 1)
		step = 2;
	cout<<"init:";
	for(auto a : encKW)
	{
		cout <<a<<".";
	}cout <<endl;
	for(int count = 0; count < turns; count++)
	{
		cout <<"step:"<<step<<endl;
		cout <<count<<":"<<endl;
		vector<int> curMem = bitonic->getSeq(step, count, encKW.size());
		std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
		cout<<"curmem:";
		for(auto c: curMem)
			cout<<c<<",";
		cout<<endl;
		vector<int> ncm = bitonic->remDup(curMem);
		cout<<"remdup:";
		for(auto c: ncm)
			cout<<c<<";";
		cout<<endl;
		vector<int> elToSort2;
		for(int k = 0; k<ncm.size(); k++)
		{
			elToSort2.push_back(encKW[ncm[k]]);
		}
		cout<<"actual:";
		for(auto c: elToSort2)
			cout<<c<<":";
		cout<<endl;
		sort(elToSort2.begin(), elToSort2.end(), cmpp);//
		cout<<"sorted:";
		for(auto c: elToSort2)
			cout<<c<<":";
		cout<<endl;
		int cnt = 0;
		for(int i =0; i<ncm.size(); i++)
		{
			encKW[ncm[i]] = elToSort2[cnt];
			cnt++;
		}
	}
	cout<<"whole:";
	for(auto a : encKW)
	{
		cout <<a<<".";
	}
	cout <<endl;
	return 0;
}
