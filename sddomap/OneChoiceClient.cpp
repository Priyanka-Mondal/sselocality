#include "OneChoiceClient.h"
#include<string>

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
	int prev = 0;
	int cprev = 0;
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
        printf("j:%d OC:%d number of Bins:%d size of bin:%d is:%d\n",j, i, curNumberOfBins, curSizeOfEachBin, is);
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
		numNEW.push_back(1);
		NEWsize.push_back(0);
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
	/*
	cout<<"CHECKING IF SORTED"<<endl;
	for(auto a : A)
	{
    	int ind = *(int*) (&(a.data()[AES_KEY_SIZE - 5]));
    	int bin = *(int*) (&(a.data()[AES_KEY_SIZE - 10]));
	    int op = ((byte) a.data()[AES_KEY_SIZE - 6]); 
		cout <<"{"<<a.data()<<"|"<<bin<<"|"<<op<<"|"<<ind<<"}";
	}
*/
	for(int a=0;a<A.size()-1;a++)
	{
    	int bina = *(int*) (&(A[a].data()[AES_KEY_SIZE - 10]));
    	int binb = *(int*) (&(A[a+1].data()[AES_KEY_SIZE - 10]));
		if(bina>binb)
			return 0;
	}
	return 1;
}
vector<prf_type> OneChoiceClient::search(int index, int instance, string keyword, unsigned char* key) 
{
	//I have to store the keyword counter somewhere, two options
	//1. store the counter
	//2. make search interactive --> For now I made it interactive
    double searchPreparation = 0, searchDecryption = 0;
    if (profile) 
        Utilities::startTimer(65);
    vector<prf_type> finalRes;
    int keywordCnt = 0;
	int cnt = 0;
	int flag = 0;
	string kw = keyword;
	int cntw = 0;
	int bin = map(kw, cntw, index, key);
	do
	{
    	vector<prf_type> ciphers = server->search(index, instance, bin);
		flag = 0;
    	totalCommunication += ciphers.size() * sizeof (prf_type) ;
	    for (auto item : ciphers) 
		{
	        prf_type plaintext = item;
	        //Utilities::decode(item, plaintext, key);
            //if (string((char*) plaintext.data()) == keyword) 
	       	if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
			{
	           	finalRes.push_back(plaintext);
				flag =1;
				//cout<<" MATCH:"<<plaintext.data()<<" "<<bin<<"/"<<finalRes.size()<<endl;
	       	}
		//cout <<"bin:"<<bin<<"/"<<numberOfBins[index]-1<<endl;
		}
		if(bin == numberOfBins[index]-1)
			bin = 0;
		else
			bin++;
		cnt++;
		//cout <<"flag:"<<flag<<"increasing count"<<cnt<<"/"<<numberOfBins[index]<<endl;
	}
	while(flag == 1 && cnt < numberOfBins[index]);
	//while(cnt < numberOfBins[index]);
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
	server->clear(index, toInstance);
	server->move(index, toInstance, fromInstance, indexSize[index]);
	server->clear(index, fromInstance);
	exist[index][toInstance] = true;
	exist[index][fromInstance] = false;
}

void OneChoiceClient::copy(int index, int toInstance)
{
	vector<prf_type> sorted = server->getNEW(index);
	server->copy(index, toInstance);
	assert(sorted.size()==indexSize[index]);
	//server->moveNEW(index, toInstance);
	exist[index][toInstance] = true;
	numNEW[index] = numNEW[index] + 1;
	exist[index][3] = false;	
	NEWsize[index] = 0;
}

void OneChoiceClient::append(int index, prf_type keyVal, unsigned char* key)
{
	prf_type encKeyVal;// = keyVal;
	encKeyVal = Utilities::encode(keyVal.data(), key);
	server->append(index, encKeyVal);
	server->writeToNEW(index, encKeyVal,NEWsize[index]);
	NEWsize[index]=NEWsize[index]+1;
	//cout <<"index:"<<index<<"|"<<NEWsize[index]<<endl;
}

void OneChoiceClient::destroy(int index, int instance)
{
    server->clear(index, instance);
    exist[index][instance] = false;
    totalCommunication += sizeof (int);
}
void OneChoiceClient::resize(int index, int size)
{
	server->resize(index,size);
	server->truncate(index,size);
}

void OneChoiceClient::getBin(int index, int instance, int start, int end,
							 unsigned char* key1, unsigned char* key2)
{
	//cout <<"["<<index-1<<" start:"<<start<<" numOfEl:"<<end<<"]"<<"actual numOfEl:"<<numberOfBins[index-1]*sizeOfEachBin[index-1];
	if(start <numberOfBins[index-1]*sizeOfEachBin[index-1])
	{
		if(start+end>numberOfBins[index-1]*sizeOfEachBin[index-1])
			end = numberOfBins[index-1]*sizeOfEachBin[index-1] - start;
		vector<prf_type> ciphers = server->getElements(index-1, instance, start, end);
		int upCnt = numNEW[index];
		int cntw=0;
		for(prf_type c: ciphers)
		{
	        prf_type plaintext;// = c;
	        Utilities::decode(c, plaintext, key1);
	        int ind = *(int*) (&(plaintext.data()[AES_KEY_SIZE - 5]));
	        int op = ((byte) plaintext.data()[AES_KEY_SIZE - 6]); 
	        int oldbin = *(int*) (&(plaintext.data()[AES_KEY_SIZE - 10]));
	        string w((char*) plaintext.data());
			int cnt;
			if(w!="")
				cnt = stoi(omaps[index]->incrementCnt(getBid(w, upCnt)));
			else
				cnt = cntw++;
			int bin = map(w, cnt, index, key2);		
			int realbin;
			if(w=="")
				realbin = INF;
			else
				realbin = bin;
	    	prf_type keyVal;
    		memset(keyVal.data(), 0, AES_KEY_SIZE);
	    	std::copy(w.begin(), w.end(), keyVal.begin());//keyword
	    	*(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = ind; 
	    	keyVal.data()[AES_KEY_SIZE - 6] = (byte) (op == 1 ? 1 : 0);//op
	    	*(int*) (&(keyVal.data()[AES_KEY_SIZE - 10])) = realbin;//bin
			append(index, keyVal, key2);
			if(w!="")
			{
				string ob = omaps[index]->incrementCnt(getBid(to_string(realbin),upCnt));
				string ob2 = omaps[index]->find(getBid(to_string(realbin),upCnt));
				//cout <<"incremented OMAP:"<<ob<<" to "<< ob2<<" upcnt:"<<upCnt<<" for bin:"<<realbin<<endl;
			}
		}

		/*if(server->getNEWsize(index) >= (instance+1)*numberOfBins[index]*sizeOfEachBin[index])
		{
			exist[index][instance] = false;
		}*/
	}
}
void OneChoiceClient::addDummy(int index, int count, unsigned char* key , int s)
{
	//cout<<"adding dummy at:"<<index<<":"<<server->getNEWsize(index)<<"|"<<2*numberOfBins[index-1]*sizeOfEachBin[index-1]<<endl;
	assert(server->getNEWsize(index) >= 2*numberOfBins[index-1]*sizeOfEachBin[index-1]);
    int upCnt = numNEW[index];
	for(int t = 0; t<s; t++)
	{
		int bin = count*s+t;
		if(bin < numberOfBins[index])
		{
			//assert(server->getNEWsize(index)<2*indexSize[index]+2*indexSize[index-1]);
			int cbin;
			string cb = (omaps[index]->find(getBid(to_string(bin),upCnt)));
			if(cb == "")
				cbin = 0;
			else 
				cbin = stoi(cb);
			//cout <<"index:"<<index<<" cbin:"<<cbin<<" upcnt:"<<upCnt<<" for bin:"<<bin<<endl;
			assert(cbin <= sizeOfEachBin[index]);
			for(int k = cbin; k<sizeOfEachBin[index]; k++)
			{
				prf_type value;
    			memset(value.data(), 0, AES_KEY_SIZE);
    			*(int*) (&(value.data()[AES_KEY_SIZE - 5])) = 9999;//dummy-id
	    		value.data()[AES_KEY_SIZE - 6] = (byte) (1);//op
    			*(int*) (&(value.data()[AES_KEY_SIZE - 10])) = bin;//bin
				append(index, value, key);
				append(index, value, key);
				string ob = omaps[index]->incrementCnt(getBid(to_string(bin),upCnt));
			}
			for(int k = 0; k<cbin ; k++)
			{ 
				prf_type value;
    			memset(value.data(), 0, AES_KEY_SIZE);
    			*(int*) (&(value.data()[AES_KEY_SIZE - 5])) = 9999;//id
	    		value.data()[AES_KEY_SIZE - 6] = (byte) (1);//op
    			*(int*) (&(value.data()[AES_KEY_SIZE - 10])) = INF;//bin
				append(index, value, key);
				append(index, value, key);
				//**dummy omap access here
			}
		}
		//else
		//	assert(server->getNEWsize(index)==2*indexSize[index]+2*indexSize[index-1]);
	}
	
	//if(count ) add more dummy for bitonic sort
}

void OneChoiceClient::pad(int index, int newSize, unsigned char* key)
{
	int size = NEWsize[index];
	//cout <<"newSize:"<<newSize<<" old size:"<<size<<endl;
	if(size<newSize)
	{
		for(int k = 0; k<newSize-size ; k++)
		{ 
			prf_type value;
    		memset(value.data(), 0, AES_KEY_SIZE);
    		*(int*) (&(value.data()[AES_KEY_SIZE - 5])) = 9999;//id
			value.data()[AES_KEY_SIZE - 6] = (byte) (1);//op
    		*(int*) (&(value.data()[AES_KEY_SIZE - 10])) = INF;//bin
			append(index, value, key);
			//**dummy omap access here
		}
	}
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
int OneChoiceClient::map(string w, int cnt, int index, unsigned char* key)
{
	//if(w=="")
	//	cnt = 0;

    prf_type K = Utilities::encode(w, key);
    prf_type mapKey, mapValue;
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(int*) (&(cntstr[AES_KEY_SIZE - 4])) = 0;
    mapKey = Utilities::generatePRF(cntstr, K.data());
    //unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
    unsigned char* hash = Utilities::sha256((char*) K.data(), AES_KEY_SIZE);
    //int bin = (((((unsigned int) (*((int*) hash))) % numberOfBins[index]))%numberOfBins[index]);
    int bin = (((((unsigned int) (*((int*) hash))) % numberOfBins[index])+cnt)%numberOfBins[index]);
	return bin;
}

bool cmpp(prf_type &a, prf_type &b)
{
	//cout <<"cmp:["<<a.second.size()<< " "<<b.second.size()<<"]["<<(a.second.size() > b.second.size()) <<"]"<<endl;
    int bina = *(int*) (&(a.data()[AES_KEY_SIZE - 10]));
    int binb = *(int*) (&(b.data()[AES_KEY_SIZE - 10]));
	return (bina < binb);
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
	//cout <<size<<endl;
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
void OneChoiceClient::deAmortizedBitSort(int step, int count, int size, int index, unsigned char* key)
{
	//cout<<"index:"<<index<<" size:"<<size<<endl;
	vector<int> curMem = getSeq(step, count, size);
	std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
	vector<int> ncm = remDup(curMem);
	//for(auto n : ncm)
	//	cout<<"("<<n<<")";
	//cout<<endl;
	//cout<<endl;
	vector<prf_type> encNEW = server->getNEW(index);
	assert(encNEW.size() == size);
	int initSize = encNEW.size();
	vector<prf_type> elToSort;
	for(int k = 0; k<ncm.size(); k++)
	{
		elToSort.push_back(encNEW[ncm[k]]);
	}
	assert(elToSort.size() == ncm.size());
	vector<prf_type> decodedNEW;	
	for(auto n : elToSort)
	{
		prf_type dec;// = n;
	    Utilities::decode(n, dec, key);
		decodedNEW.push_back(dec);
	}
	assert(elToSort.size() == decodedNEW.size());
	sort(decodedNEW.begin(), decodedNEW.end(), cmpp);
	assert(issorted(decodedNEW));
	vector<prf_type> sorted;
	for(auto n : decodedNEW)
	{
		prf_type enc;// = n;
		enc = Utilities::encode(n.data(), key);
		sorted.push_back(enc);
	}
	int cnt = 0;
	for(int i =0; i<ncm.size(); i++)
	{
		encNEW[ncm[i]] = sorted[cnt];
		cnt++;
	}
	assert(encNEW.size() == initSize);
	server->putNEW(index, encNEW);
}



	/*for(auto a : A)
	{
    	int bina = *(int*) (&(a.data()[AES_KEY_SIZE - 10]));
		cout <<"{"<<bina<<"}";
	}
 for (int i = 0; i < A.size(); i++)
 {
        for (int j = 0; j < A.size() - i - 1; j++)
		{
    		int bina = *(int*) (&(A[j].data()[AES_KEY_SIZE - 10]));
    		int binb = *(int*) (&(A[j+1].data()[AES_KEY_SIZE - 10]));
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
	vector<prf_type> encNEWi = server->getNEW(index);
	int newSize = pow(2,floor((float)log2(2*indexSize[index]+2*indexSize[index-1])));
	//assert(encNEWi.size() <= 2*indexSize[index]+2*indexSize[index-1]);
	//assert(encNEWi.size() == newSize);
	int upCnt = numNEW[index];
	for(int k =0; k<numberOfBins[index]; k++)
	{
		int cb = stoi((omaps[index]->find(getBid(to_string(k),upCnt))));
		//assert(cb == sizeOfEachBin[index]);
	}
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
		server->putNEW(index, encNEWi);
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
			int bina = *(int*) (&(val.data()[AES_KEY_SIZE - 10]));
			assert(bina == i);
		}
	}*/
	//else
	//	cout <<"ALREADY SORTED:"<<index<<endl;
	//assert(issorted(encNEWi));
}
void OneChoiceClient::reSize(int index, int size)
{
	server->resize(index,size);
}
int OneChoiceClient::getNEWsize(int index)
{
	return NEWsize[index];
}
