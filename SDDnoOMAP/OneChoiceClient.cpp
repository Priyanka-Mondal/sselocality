#include "OneChoiceClient.h"
#include<string>
#include<map>
#include<vector>
#include<algorithm>


using namespace::std;
OneChoiceClient::~OneChoiceClient() 
{
    delete server;
}

OneChoiceClient::OneChoiceClient(int N,
		bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
	int l = ceil((float)log2(N));
    memset(nullKey.data(), 0, AES_KEY_SIZE);
	b = ceil((float)log2(B));
	this->numOfIndices = l - b;
    server = new OneChoiceServer(numOfIndices, inMemory, overwrite, profile);
    for (int i = 0; i <=numOfIndices; i++) 
	{
		int j = i + b;
        int curNumberOfBins = j > 1 ? 
			(int) ceil(((float) pow(2, j))/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        int curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : pow(2,j);
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
		int is = curNumberOfBins*curSizeOfEachBin;
		indexSize.push_back(is);
        printf("Index:%d number of Bins:%d size of bin:%d is:%d\n",i, curNumberOfBins, curSizeOfEachBin, is);
    }
	exist.resize(numOfIndices+1);
    for (int i = 0; i <=numOfIndices; i++) 
	{
        //exist.push_back(vector<bool>());
		exist[i].resize(4);
    	for (int j = 0; j < 4; j++) 
		{
            exist[i].push_back(false);
        }
		numNEW.push_back(1);//updtCnt
		NEWsize.push_back(0);
		KWsize.push_back(0);
    }
	exist[0][3] = true;
    for (int i = 0; i <=numOfIndices; i++) 
	{
        bytes<Key> key{0};
        OMAP* omap = new OMAP(max((int) pow(2, i+2), 16), key);
        omaps.push_back(omap);
    }
}

int issorted(vector<prf_type> A)
{
	for(int a=0;a<A.size()-1;a++)
	{
    	int bina = *(int*) (&(A[a].data()[AES_KEY_SIZE - 11]));
    	int binb = *(int*) (&(A[a+1].data()[AES_KEY_SIZE - 11]));
		if(bina>binb)
			return 0;
	}
	return 1;
}
int issortedC(vector<prf_type> A)
{
	for(int a=0;a<A.size()-1;a++)
	{
    	int prpa = *(int*) (&(A[a].data()[AES_KEY_SIZE - 11]));
    	int prpb = *(int*) (&(A[a+1].data()[AES_KEY_SIZE - 11]));
		if(prpa<prpb)
			return 0;
	}
	return 1;
}
vector<prf_type> OneChoiceClient::search(int index, int instance, string keyword, unsigned char* key) 
{
    double searchPreparation = 0, searchDecryption = 0;
    if (profile) 
        Utilities::startTimer(65);
    vector<prf_type> finalRes;
    int keywordCnt = 0;
	int cnt = 0;
	int flag = 0;
	string kw = keyword;
	int cntw = 0;
	do
	{
		int bin = hashKey(kw, cntw, index, key);
		cntw++;
    	vector<prf_type> ciphers = server->searchBin(index, instance, bin);
		flag = 0;
    	totalCommunication += ciphers.size() * sizeof (prf_type) ;
	    for (auto item : ciphers) 
		{
	        prf_type plaintext = item;
	        Utilities::decode(item, plaintext, key);
	       	if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
			{
	           	finalRes.push_back(plaintext);
				flag =1;
				//int op = ((byte) plaintext.data()[AES_KEY_SIZE - 6]);
				//cout<<" MATCH:"<<plaintext.data()<<" op:"<<op<<" bin:"<<bin<<" index:"<<index<<endl;
	       	}
		}
		if(bin == numberOfBins[index]-1)
			bin = 0;
		else
			bin++;
		cnt++;
	}
	while(flag == 1 && cnt < numberOfBins[index]);
    if (profile) 
	{
        searchPreparation = Utilities::stopTimer(65);
        //printf("search preparation time:%f include server time\n", searchPreparation);
        Utilities::startTimer(65);
    }

    if (profile) 
	{
        searchDecryption = Utilities::stopTimer(65);
        //cout<<"search decryption time:"<<searchDecryption<<" for decrypting:"<<ciphers.size()<<" ciphers"<<endl;
    }
    return finalRes;
}

vector<prf_type> OneChoiceClient::NIsearch(int index, int instance, string keyword, unsigned char* key) 
{
    double searchPreparation = 0, searchDecryption = 0;
    if (profile) 
        Utilities::startTimer(65);
    vector<prf_type> finalRes;
    int keywordCnt = 0;
	prf_type K = Utilities::encode(keyword, key);
	int keywordCount = 0;
	vector<prf_type> ciphers = server->search(index, instance, K, keywordCount);
	totalCommunication += ciphers.size() * sizeof (prf_type) ;
	for (auto item : ciphers) 
	{
	    prf_type plaintext = item;
	    Utilities::decode(item, plaintext, key);
	   	if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
		{
	       	finalRes.push_back(plaintext);
	        int ind = *(int*) (&(plaintext.data()[AES_KEY_SIZE - 5]));
			int op = ((byte) plaintext.data()[AES_KEY_SIZE - 6]);
			//cout<<" MATCH:"<<plaintext.data()<<" op:"<<op<<" id:"<<ind<<" index:"<<index<<endl;
	   	}
	}

    if (profile) 
	{
        searchPreparation = Utilities::stopTimer(65);
        //printf("search preparation time:%f include server time\n", searchPreparation);
        Utilities::startTimer(65);
    }

    if (profile) 
	{
        searchDecryption = Utilities::stopTimer(65);
        //cout<<"search decryption time:"<<searchDecryption<<" for decrypting:"<<ciphers.size()<<" ciphers"<<endl;
    }
    return finalRes;
}

void OneChoiceClient::move(int index, int toInstance, int fromInstance)
{
	//cout <<"move:("<<index<<","<<toInstance<<")<-("<<index<<","<<fromInstance<<")"<<endl;
	server->clear(index, toInstance);
	server->move(index, toInstance, fromInstance, indexSize[index]);
	server->clear(index, fromInstance);
	exist[index][toInstance] = true;
	exist[index][fromInstance] = false;
	if(fromInstance == 3)
	{
		numNEW[index] = numNEW[index] + 1;
		NEWsize[index] = 0;
		KWsize[index] = 0;
	}
}

void OneChoiceClient::appendTokwCounter(int index, prf_type keyVal, unsigned char* key)
{
	exist[index][3] = true;
	prf_type encKeyVal;
	encKeyVal = Utilities::encode(keyVal.data(), key);
	int last = server->writeToKW(index, encKeyVal, KWsize[index]);
	KWsize[index]=KWsize[index]+1;
	assert(last == KWsize[index]*AES_KEY_SIZE);
}
void OneChoiceClient::append(int index, prf_type keyVal, unsigned char* key)
{
	exist[index][3] = true;
	prf_type encKeyVal;
	encKeyVal = Utilities::encode(keyVal.data(), key);
	int last = server->writeToNEW(index, encKeyVal, NEWsize[index]);
	NEWsize[index]=NEWsize[index]+1;
	assert(last == NEWsize[index]*AES_KEY_SIZE);
}

void OneChoiceClient::destroy(int index, int instance)
{
    server->clear(index, instance);
    exist[index][instance] = false;
	//delete omaps[index];
    //bytes<Key> key{0};
    //OMAP* omap = new OMAP(max((int) pow(2, index+2), 16), key);
    //omaps[index]=omap;
	if(instance == 3)
	{
		NEWsize[index]=0;
		KWsize[index]=0;
	}
}
void OneChoiceClient::resize(int index, int size)
{
	server->truncate(index,size, NEWsize[index]);
	NEWsize[index]=size;
}

void OneChoiceClient::getBin(int index, int instance, int start, int end,
							 unsigned char* key1, unsigned char* key2)
{
	assert(index>=1);
	if(start <numberOfBins[index-1]*sizeOfEachBin[index-1])
	{
	//cout <<"["<<index-1<<" start:"<<start<<" numOfEl:"<<end<<"]"<<"actual numOfEl:"<<numberOfBins[index-1]*sizeOfEachBin[index-1];
		if(start+end>numberOfBins[index-1]*sizeOfEachBin[index-1])
			end = numberOfBins[index-1]*sizeOfEachBin[index-1] - start;
		vector<prf_type> ciphers = server->getElements(index-1, instance, start, end);
		assert(ciphers.size() == end);
		int upCnt = numNEW[index];
		int cntw=0;
		for(prf_type c: ciphers)
		{
	        prf_type plaintext;// = c;
	        Utilities::decode(c, plaintext, key1);
	        int ind = *(int*) (&(plaintext.data()[AES_KEY_SIZE - 5]));
	        int op = ((byte) plaintext.data()[AES_KEY_SIZE - 6]); 
	        int oldbin = *(int*) (&(plaintext.data()[AES_KEY_SIZE - 11]));
	        string w((char*) plaintext.data());
			int cnt;
			if(w!="")
				cnt = stoi(omaps[index]->incrementCnt(getBid(w, upCnt)));
			else
				cnt = cntw++;
			int bin = hashKey(w, cnt, index, key2);		
			int realbin = bin;
	    	prf_type keyVal;
    		memset(keyVal.data(), 0, AES_KEY_SIZE);
	    	std::copy(w.begin(), w.end(), keyVal.begin());//keyword
	    	*(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = ind; 
	    	keyVal.data()[AES_KEY_SIZE - 6] = (byte) (op == 0 ? OP::INS : OP::DEL);//op
	    	*(int*) (&(keyVal.data()[AES_KEY_SIZE - 11])) = realbin;//bin
			append(index, keyVal, key2);
			if(w!="")
			{
				string ob = omaps[index]->incrementCnt(getBid(to_string(realbin),upCnt));
				//string ob2 = omaps[index]->find(getBid(to_string(realbin),upCnt));
				//cout <<"incremented OMAP:"<<ob<<" to "<< ob2<<" upcnt:"<<upCnt<<" for bin:"<<realbin<<endl;
			}
		}
	}
}
void OneChoiceClient::addDummy(int index, int count, unsigned char* key , int s, int r1, int r2)
{
	cout<<"adding dummy at:"<<index<<":"<<NEWsize[index]<<"|"<<2*numberOfBins[index-1]*sizeOfEachBin[index-1]<<"s:"<<s<<endl;
	assert(index>=1);
	assert(count>=0);
	cout <<"index:"<<index<<" "<<NEWsize[index]<<">="<<2*numberOfBins[index-1]*sizeOfEachBin[index-1]<<endl;
	assert(NEWsize[index] >= 2*numberOfBins[index-1]*sizeOfEachBin[index-1]);
    int upCnt = numNEW[index];
	if(count+r1 == r2)
	{
		//assert(NEWsize[index] == 2*indexSize[index]+2*indexSize[index-1]);
		int newSize = ceil((float)log2(2*indexSize[index]+2*indexSize[index-1]));
		newSize = pow(2,newSize);
		pad(index,newSize,r2, key);
		assert(newSize>=2*indexSize[index]);
	}
	else
	{
		for(int t = 0; t<s; t++)
		{
			int bin = count*s+t;
			if(bin < numberOfBins[index])
			{
				//assert(NEWsize[index]<2*indexSize[index]+2*indexSize[index-1]);
				int cbin;
				string cb = (omaps[index]->find(getBid(to_string(bin),upCnt)));
				if(cb == "")
					cbin = 0;
				else 
					cbin = stoi(cb);
				assert(cbin <= sizeOfEachBin[index]);
				for(int k = cbin; k<sizeOfEachBin[index]; k++)
				{
					prf_type value;
    				memset(value.data(), 0, AES_KEY_SIZE);
    				*(int*) (&(value.data()[AES_KEY_SIZE - 5])) = INF;//dummy-id
		    		value.data()[AES_KEY_SIZE - 6] = (byte) (OP::INS);//op
    				*(int*) (&(value.data()[AES_KEY_SIZE - 11])) = bin;//bin
					append(index, value, key); // double insertion for bitonic sort in future 
					string ob = omaps[index]->incrementCnt(getBid(to_string(bin),upCnt));
				}
				for(int k = cbin; k<sizeOfEachBin[index]; k++)
				{
					prf_type value;
    				memset(value.data(), 0, AES_KEY_SIZE);
    				*(int*) (&(value.data()[AES_KEY_SIZE - 5])) = INF;//dummy-id
		    		value.data()[AES_KEY_SIZE - 6] = (byte) (OP::INS);//op
    				*(int*) (&(value.data()[AES_KEY_SIZE - 11])) = INF;//bin
					append(index, value, key); // double insertion for bitonic sort in future 
					string ob = omaps[index]->incrementCnt(getBid(to_string(bin),upCnt));
				}
				for(int k = 0; k<cbin ; k++)
				{ 
					prf_type value;
    				memset(value.data(), 0, AES_KEY_SIZE);
    				*(int*) (&(value.data()[AES_KEY_SIZE - 5])) = INF;//id
		    		value.data()[AES_KEY_SIZE - 6] = (byte) (OP::INS);//op
    				*(int*) (&(value.data()[AES_KEY_SIZE - 11])) = INF;//bin
					append(index, value, key);
					append(index, value, key);
					//**dummy omap access here
				}
			}
		}
	}
}

void OneChoiceClient::pad(int index, int newSize, int r, unsigned char* key)
{
	assert(index>=1);
	int size = NEWsize[index];
	if(size<newSize)
	{
		for(int k = 0; k<newSize-size ; k++)
		{ 
			prf_type value;
    		memset(value.data(), 0, AES_KEY_SIZE);
    		*(int*) (&(value.data()[AES_KEY_SIZE - 5])) = INF;//id
			value.data()[AES_KEY_SIZE - 6] = (byte) (OP::INS);//op
    		*(int*) (&(value.data()[AES_KEY_SIZE - 11])) = INF;//bin
			append(index, value, key);
			//**dummy omap access here
		}
	}
	//**will have to deamortize it later**//
	//updateCounters(index, key);
}

void OneChoiceClient::updateOMAP(int index, string keyword, unsigned char* key)
{
	int upCnt = numNEW[index];
	string ob = omaps[index]->incrementCnt(getBid(keyword,upCnt));
}

void OneChoiceClient::updateHashTable(int index, unsigned char* key)
{
	vector<prf_type> some = server->getNEW(index, 0, pow(2,index), false);
	map <prf_type, prf_type> kcc;
	for(auto c: some)
	{
	    prf_type plaintext;// = c;
	    Utilities::decode(c, plaintext, key);
	    string w((char*) plaintext.data());
	    int cntw = *(int*) (&(plaintext.data()[AES_KEY_SIZE - 5]));
	    //int prp = ((byte) plaintext.data()[AES_KEY_SIZE - 11]); 
		prf_type K = Utilities::encode(w, key);
		unsigned char cntstr[AES_KEY_SIZE];
		memset(cntstr, 0, AES_KEY_SIZE);
		*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1; // here add the PRP
		prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
		prf_type valueTmp, totalTmp;
		*(int*) (&(valueTmp[0])) = cntw;
		prf_type mapValue = Utilities::encode(valueTmp.data(), K.data());
		kcc[mapKey] = mapValue; 
	}
	server->storeKwCounters(index, 3, kcc);

}
void OneChoiceClient::kwCount(int index, unsigned char* key, int count, int r1, int r2)
{
	int upCnt = numNEW[index];
	int totalSteps = r2-r1+1;
	int size = ceil((float)NEWsize[index]/(float)(totalSteps));
	vector<prf_type> all= server->getNEW(index, count, size, true);
	for(auto c: all)
	{
	    prf_type plaintext;// = c;
	    Utilities::decode(c, plaintext, key);
	    string w((char*) plaintext.data());
		int cntw = 0;
		if(w!="")
			cntw = stoi(omaps[index]->find(getBid(w, upCnt)));
	    prf_type keyVal;
    	memset(keyVal.data(), 0, AES_KEY_SIZE);
	    std::copy(w.begin(), w.end(), keyVal.begin());
	    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = cntw; 
	    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 11])) = cntw;//here goes the PRP later
		appendTokwCounter(index, keyVal,key);
	}
	if(count==r2)
	{
		int ks = KWsize[index];
		int ns = NEWsize[index];
		cout <<ns<<"|"<<ns<<endl;
		if(ks<ns)
		{
			for(int k = ks; k<ns ; k++)
			{ 
				prf_type value;
	    		memset(value.data(), 0, AES_KEY_SIZE);
	    		*(int*) (&(value.data()[AES_KEY_SIZE - 5])) = 0;
	    		*(int*) (&(value.data()[AES_KEY_SIZE - 11])) = 0;
				appendTokwCounter(index, value, key);
				//**dummy omap access here
			}
		}
	}
}

/*
void OneChoiceClient::updateCounters(int index, unsigned char* key, int count, int r1, int r2)
{
	//cout <<"updCo:("<<index<<")"<<endl;
	int upCnt = numNEW[index];
	int totalSteps = r2-r1+1;
	int size = ceil((float)NEWsize[index]/(float)(totalSteps));
	vector<prf_type> all= server->getNEW(index, count, size, true);
	map <prf_type, prf_type> kcc;
	for(auto c: all)
	{
	    prf_type plaintext;// = c;
	    Utilities::decode(c, plaintext, key);
	    string w((char*) plaintext.data());
		//if(w!="")
		//{
			int cntw = stoi(omaps[index]->find(getBid(w, upCnt)));
			prf_type K = Utilities::encode(w, key);
			unsigned char cntstr[AES_KEY_SIZE];
			memset(cntstr, 0, AES_KEY_SIZE);
			*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1; // here add the PRP
			prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
			prf_type valueTmp, totalTmp;
			*(int*) (&(valueTmp[0])) = cntw;
			if(w=="")
				*(int*) (&(valueTmp[0])) = 0;
			prf_type mapValue = Utilities::encode(valueTmp.data(), K.data());
			kcc[mapKey] = mapValue; 
		//}
	}
	server->storeKwCounters(index, 3, kcc);
}
*/

void OneChoiceClient::updateCounters(int index, unsigned char* key)
{
	int upCnt = numNEW[index];
	vector<prf_type> all= server->getNEW(index, 0, NEWsize[index], true);
	map <prf_type, prf_type> kcc;
	for(auto c: all)
	{
	    prf_type plaintext;// = c;
		cout <<"decoding:"<<endl;
	    Utilities::decode(c, plaintext, key);
	    string w((char*) plaintext.data());
		if(w!="")
		{
			int cntw = stoi(omaps[index]->find(getBid(w, upCnt)));
			prf_type K = Utilities::encode(w, key);
			unsigned char cntstr[AES_KEY_SIZE];
			memset(cntstr, 0, AES_KEY_SIZE);
			*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
			prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
			prf_type valueTmp, totalTmp;
			*(int*) (&(valueTmp[0])) = cntw;
			prf_type mapValue = Utilities::encode(valueTmp.data(), K.data());
			kcc[mapKey] = mapValue; 
		}
	}
	//pad with dummy omap access
	server->storeKwCounters(index, 3, kcc);
}


Bid OneChoiceClient::getBid(string input, int cnt) 
{
    std::array< uint8_t, ID_SIZE> value;
    std::fill(value.begin(), value.end(), 0);
    std::copy(input.begin(), input.end(), value.begin());
    *(int*) (&value[AES_KEY_SIZE - 4]) = cnt; 
    Bid res(value);
    return res;
}
int OneChoiceClient::hashKey(string w, int cnt, int index, unsigned char* key)
{
	if(w=="")
		return INF;

    prf_type K = Utilities::encode(w, key);
	unsigned char cntstr[AES_KEY_SIZE];		
	memset(cntstr, 0, AES_KEY_SIZE);
	*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
	prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
    unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
    int bin = ((((unsigned int) (*((int*) hash))) +cnt)%numberOfBins[index]);
	return bin;
}

bool cmpp(prf_type &a, prf_type &b)
{
	//cout <<"cmp:["<<a.second.size()<< " "<<b.second.size()<<"]["<<(a.second.size() > b.second.size()) <<"]"<<endl;
    int bina = *(int*) (&(a.data()[AES_KEY_SIZE - 11]));
    int binb = *(int*) (&(b.data()[AES_KEY_SIZE - 11]));
	return (bina < binb);
}

bool cmpp2(prf_type &a, prf_type &b)
{
	//cout <<"cmp:["<<a.second.size()<< " "<<b.second.size()<<"]["<<(a.second.size() > b.second.size()) <<"]"<<endl;
    int prpa = *(int*) (&(a.data()[AES_KEY_SIZE - 11]));
    int prpb = *(int*) (&(b.data()[AES_KEY_SIZE - 11]));
	return (prpa > prpb);
}

vector<prf_type> sort(vector<prf_type> &A)
{
	sort(A.begin(), A.end(), cmpp);
	return A;
}

void compAndSwap(int a[], int i, int j)
{
	if ((a[i]>a[j]))
		swap(a[i],a[j]);
}
void bitonicMerge(int a[], int low, int cnt,vector<int>&memseq)
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
		bitonicMerge(a, low, k,memseq);
		bitonicMerge(a, low+k, k, memseq);
	}
}
void bitMerge(int a[], int low, int cnt,vector<int>&memseq)
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
		bitonicMerge(a, low, k,memseq);
		bitonicMerge(a, low+k, k,memseq);
	}
}
void bitonicSort(int a[],int low, int cnt, vector<int>&memseq)
{
	if (cnt>1)
	{
		int k = cnt/2;
		bitonicSort(a, low, k,memseq);
		bitonicSort(a, low+k, k,memseq);
		bitMerge(a,low, cnt,memseq);
	}
}

void generateSeq(int a[], int N, vector<int>& memseq)
{
	bitonicSort(a,0, N, memseq);
}
vector<int> getSeq(int step, int count, int size)
{
	vector<int> memseq;
	int a[size];
	memset(a,0,size);
	generateSeq(a, size, memseq);
	//cout <<size<<" "<<memseq.size()<<endl;
	assert(memseq.size() == 2*ceil((float)(size*log2(size)*(log2(size)+1)/(float)4)));
	int start = count*step;
	vector<int> res;
	for(int i = start; i<start+step; i++)
	{
		//cout <<"(("<<memseq[i]<<"))";
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
bool OneChoiceClient::sorted(int index, unsigned char* key)
{
	vector<prf_type> els = server->getNEW(index, 0,NEWsize[index], true);
	vector<prf_type> decoded;
	for(auto n :els)
	{
		prf_type plain;
		Utilities::decode(n, plain, key);
		decoded.push_back(plain);
		return issorted(decoded);
	}
}
void OneChoiceClient::deAmortizedBitSortC(int step, int count, int size, int index, unsigned char* key)
{
	assert(NEWsize[index]==KWsize[index]);
	vector<int> curMem = getSeq(step, count, size);
	std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
	vector<int> ncm = remDup(curMem);

	vector<prf_type> encKW = server->getNEW(index,0, KWsize[index], false);
	vector<prf_type> elToSort2;
	for(int k = 0; k<ncm.size(); k++)
	{
		elToSort2.push_back(encKW[ncm[k]]);
	}
	assert(elToSort2.size() == ncm.size());
	vector<prf_type> decodedKW;	
	for(auto n : elToSort2)
	{
		prf_type dec;// = n;
	    Utilities::decode(n, dec, key);
		decodedKW.push_back(dec);
	}
	assert(elToSort2.size() == decodedKW.size());
	sort(decodedKW.begin(), decodedKW.end(), cmpp2);//
	assert(issortedC(decodedKW));
	vector<prf_type> sorted2;
	for(auto n : decodedKW)
	{
		prf_type enc;// = n;
		enc = Utilities::encode(n.data(), key);
		sorted2.push_back(enc);
	}
	int cnt = 0;
	for(int i =0; i<ncm.size(); i++)
	{
		encKW[ncm[i]] = sorted2[cnt];
		cnt++;
	}
	assert(encKW.size() == size);
	server->putNEW(index, 4, encKW);

}

void OneChoiceClient::deAmortizedBitSort(int step, int count, int size, int index, unsigned char* key)
{
	assert(NEWsize[index]==KWsize[index]);
	vector<int> curMem = getSeq(step, count, size);
	std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
	vector<int> ncm = remDup(curMem);

	vector<prf_type> encNEW = server->getNEW(index,0, NEWsize[index], true);

	assert(size == NEWsize[index]);
	assert(encNEW.size() == NEWsize[index]);
	vector<prf_type> elToSort1;
	for(int k = 0; k<ncm.size(); k++)
	{
		elToSort1.push_back(encNEW[ncm[k]]);
	}
	assert(elToSort1.size() == ncm.size());
	vector<prf_type> decodedNEW;	
	for(auto n : elToSort1)
	{
		prf_type dec;// = n;
	    Utilities::decode(n, dec, key);
		decodedNEW.push_back(dec);
	}
	assert(elToSort1.size() == decodedNEW.size());
	sort(decodedNEW.begin(), decodedNEW.end(), cmpp);
	assert(issorted(decodedNEW));
	vector<prf_type> sorted1;
	for(auto n : decodedNEW)
	{
		prf_type enc;// = n;
		enc = Utilities::encode(n.data(), key);
		sorted1.push_back(enc);
	}
	int cnt = 0;
	for(int i =0; i<ncm.size(); i++)
	{
		encNEW[ncm[i]] = sorted1[cnt];
		cnt++;
	}
	assert(encNEW.size() == size);
	server->putNEW(index, 3, encNEW);
}



	/*for(auto a : A)
	{
    	int bina = *(int*) (&(a.data()[AES_KEY_SIZE - 11]));
		cout <<"{"<<bina<<"}";
	}
 for (int i = 0; i < A.size(); i++)
 {
        for (int j = 0; j < A.size() - i - 1; j++)
		{
    		int bina = *(int*) (&(A[j].data()[AES_KEY_SIZE - 11]));
    		int binb = *(int*) (&(A[j+1].data()[AES_KEY_SIZE - 11]));
            if (bina>binb)
			{
				prf_type temp;
				temp = A[j];
				A[j] = A[j+1];
				A[j+1] = temp;
			}
		}
}
return A;
}
*/
void OneChoiceClient::nonOblSort(int index, unsigned char* key)
{
	vector<prf_type> encNEWi = server->getNEW(index,0, NEWsize[index],true);
	int newSize = pow(2,floor((float)log2(2*indexSize[index]+2*indexSize[index-1])));
	assert(encNEWi.size() == NEWsize[index]);
	int upCnt = numNEW[index];
	vector<prf_type> decodedNEWi;	
	for(auto n : encNEWi)
	{
		prf_type dec;// = n;
	    Utilities::decode(n, dec, key);
		decodedNEWi.push_back(dec);
	}
	if(!issorted(decodedNEWi))
	{
		server->resize(index,0);
		sort(decodedNEWi);
		encNEWi.clear();
		for(auto n : decodedNEWi)
		{
			prf_type enc;// = n;
			enc = Utilities::encode(n.data(), key);
			encNEWi.push_back(enc);
		}
		server->putNEW(index, 3, encNEWi);
	}
	/*
	vector<prf_type> sortedNEW = server->getNEW(index);
	vector<prf_type> decodedNEWi;	
	for(auto n : sortedNEW)
	{
		prf_type dec;// = n;
	    Utilities::decode(n, dec, key);
		decodedNEWi.push_back(dec);
	}
	for(int i = 0; i<numberOfBins[index]; i++)
	{
		for(int j = 0; j<sizeOfEachBin[index]; j++)
		{
			int k = i*sizeOfEachBin[index]+j;
			prf_type val = decodedNEWi[k];
			int bina = *(int*) (&(val.data()[AES_KEY_SIZE - 11]));
			assert(bina == i);
		}
	}*/
	//else
	//	cout <<"ALREADY SORTED:"<<index<<endl;
	//assert(issorted(encNEWi));
}
int OneChoiceClient::getNEWsize(int index)
{
	return NEWsize[index];
}
