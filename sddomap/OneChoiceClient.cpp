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
		/*if(curSizeOfEachBin*curNumberOfBins <= 2*prev*cprev)
		{
			curNumberOfBins = ceil((float)(2*prev*cprev+1)/(float)curSizeOfEachBin);
		}
		cprev = curSizeOfEachBin;
		prev = curNumberOfBins;*/
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
	cout<<"CHECKING IF SORTED"<<endl;
	for(auto a : A)
	{
    	int ind = *(int*) (&(a.data()[AES_KEY_SIZE - 5]));
    	int bin = *(int*) (&(a.data()[AES_KEY_SIZE - 10]));
	    int op = ((byte) a.data()[AES_KEY_SIZE - 6]); 
		cout <<"{"<<a.data()<<"|"<<bin<<"|"<<op<<"|"<<ind<<"}";
	}
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
		cntw++;
    	//vector<prf_type> ciphers = server->getAllData(index,instance);
    	vector<prf_type> ciphers = server->search(index, instance, bin);
		assert(issorted(ciphers));
		cout <<index<<":ciphers size:"<<ciphers.size()<<endl;
		flag = 0;
    	totalCommunication += ciphers.size() * sizeof (prf_type) ;
	    for (auto item : ciphers) 
		{
	        prf_type plaintext = item;
	        //Utilities::decode(item, plaintext, key);
	       	//if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
			//if(plaintext.data()!=nullKey.data())
            if (string((char*) plaintext.data()) == keyword) 
			{
			cout <<"{"<<plaintext.data()<<"}";
	           	finalRes.push_back(plaintext);
				flag =1;
				cout<<" MATCH:"<<plaintext.data()<<" "<<bin<<"/"<<finalRes.size()<<endl;
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
	server->move(index, toInstance, fromInstance);
	server->clear(index, fromInstance);
	exist[index][toInstance] = true;
	exist[index][fromInstance] = false;
	vector<prf_type> again = server->getAllData(index,toInstance);
	cout <<"index:"<<index<<"MOVE:"<<again.size()<<endl;
	assert(issorted(again));
}

void OneChoiceClient::copy(int index, int toInstance)
{
	server->copy(index, toInstance);
	exist[index][toInstance] = true;
	numNEW[index] = numNEW[index] + 1;
	exist[index][3] = false;	
	vector<prf_type> again = server->getAllData(index,toInstance);
	cout <<"index:"<<index<<"AGAIN:"<<again.size()<<endl;
	assert(issorted(again));
}

void OneChoiceClient::append(int index, prf_type keyVal, unsigned char* key)
{
	prf_type encKeyVal = keyVal;
	//encKeyVal = Utilities::encode(keyVal.data(), key);
	server->append(index, encKeyVal);
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
}

void OneChoiceClient::getBin(int index, int instance, int start, int end,
							 unsigned char* key1, unsigned char* key2)
{
	//cout <<"["<<index-1<<" start:"<<start<<" numOfEl:"<<end<<"]"<<"actual numOfEl:"<<numberOfBins[index-1]*sizeOfEachBin[index-1];
	if(start <numberOfBins[index-1]*sizeOfEachBin[index-1])
	{
		if(start+end>numberOfBins[index-1]*sizeOfEachBin[index-1])
			end = numberOfBins[index-1]*sizeOfEachBin[index-1] - start;
		//cout <<" new numOfEl:"<<end<<endl;
		vector<prf_type> ciphers = server->getElements(index-1, instance, start, end);
		int upCnt = numNEW[index];
		int cntw=0;
		//cout <<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		for(auto c: ciphers)
		{
	        prf_type plaintext = c;
			//cout <<"[ "<<c.data() <<" ]"<<endl;
	        //Utilities::decode(c, plaintext, key1);
	        prf_type decodedString = plaintext;
	        int ind = *(int*) (&(decodedString.data()[AES_KEY_SIZE - 5]));
	        int op = ((byte) decodedString.data()[AES_KEY_SIZE - 6]); 
	        string w((char*) plaintext.data());
			int cnt;
			if(w!="")
				cnt = stoi(omaps[index]->incrementCnt(getBid(w, upCnt)));
			else
				cnt = cntw++;
			//string newCnt= omaps[index]->find(getBid(w,upCnt));
			int bin = map(w, cnt, index, key2);		
			//cout <<"from index:"<<index-1<<" instance:"<<instance<<" upCnt:"<<upCnt <<" "<<cnt;
			//cout <<"["<<w<<"|"<<ind<<"|"<<bin<<"]/"<<numberOfBins[index]-1<<endl;
			int realbin;
			if(w=="")
				realbin = INF;
			else
				realbin = bin;
	    	prf_type keyVal;
    		memset(keyVal.data(), 0, AES_KEY_SIZE);
	    	std::copy(w.begin(), w.end(), keyVal.begin());//keyword
	    	*(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = ind; 
			//*(int*)(&(decodedString.data()[AES_KEY_SIZE - 5]));//fileid
	    	keyVal.data()[AES_KEY_SIZE - 6] = (byte) (op == 1 ? 1 : 0);//op
	    	*(int*) (&(keyVal.data()[AES_KEY_SIZE - 10])) = realbin;//bin
			append(index, keyVal, key2);
			if(w!="")
			string ob = omaps[index]->incrementCnt(getBid(to_string(bin),upCnt));
		}

		/*if(server->getNEWsize(index) >= (instance+1)*numberOfBins[index]*sizeOfEachBin[index])
		{
			exist[index][instance] = false;
		}*/
	}
}
void OneChoiceClient::addDummy(int index, int count, unsigned char* key , int s)
{
	cout<<"adding dummy at:"<<index<<":"<<server->getNEWsize(index)<<"|"<<2*numberOfBins[index-1]*sizeOfEachBin[index-1]<<endl;
	assert(server->getNEWsize(index) >= 2*numberOfBins[index-1]*sizeOfEachBin[index-1]);
    int upCnt = numNEW[index];
	cout <<"s:"<<s<<" count:"<<count<<endl;
	for(int t = 0; t<s; t++)
	{
		int bin = count*s+t;
		//cout <<"ADDdummy count:"<<count<<" upCnt:"<<upCnt <<" index:"<<index<<" bin:"<<bin<<endl;
		if(bin < numberOfBins[index])
		{
			assert(server->getNEWsize(index)<indexSize[index]+2*indexSize[index-1]);
			int cbin;
			string cb = (omaps[index]->find(getBid(to_string(bin),upCnt)));
			if(cb == "")
				cbin = 0;
			else 
				cbin = stoi(cb);
			cout <<"bin:"<<bin<<"current bin size:"<<cbin<<"/"<<sizeOfEachBin[index]<<endl;
			//if(cbin>sizeOfEachBin[index]);
			//	ensureNEWSize(index,bin, sizeOfEachBin[index]-cbin);
			assert(cbin <= sizeOfEachBin[index]);
			for(int k = cbin; k<sizeOfEachBin[index]; k++)
			{
			//	cout <<"adding REAL dummy:"<<k<<endl;
				prf_type value;
    			memset(value.data(), 0, AES_KEY_SIZE);
    			*(int*) (&(value.data()[AES_KEY_SIZE - 10])) = bin;//bin
				append(index, value, key);
				string ob = omaps[index]->incrementCnt(getBid(to_string(bin),upCnt));
			}
			for(int k = 0; k<cbin ; k++)
			{
			//	cout <<"adding dummy dummy"<<endl;
				prf_type value;
    			memset(value.data(), 0, AES_KEY_SIZE);
    			*(int*) (&(value.data()[AES_KEY_SIZE - 10])) = INF;//bin
				append(index, value, key);
			}
		}
		else
			assert(server->getNEWsize(index)==indexSize[index]+2*indexSize[index-1]);
	}
	//if(count ) add more dummy for bitonic sort
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
	//cout <<"at map1:"<<bin<<"|"<<bin2<<endl;
	return bin;
}
void OneChoiceClient::bitonicSort(int step, int index, int counter)
{
	//do the actual sort here
	//but get the elements from the server
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
	//sort(A.begin(), A.end(), cmpp);
	/*for(auto a : A)
	{
    	int bina = *(int*) (&(a.data()[AES_KEY_SIZE - 10]));
		cout <<"{"<<bina<<"}";
	}*/
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
void OneChoiceClient::nonOblSort(int index, unsigned char* key)
{
	vector<prf_type> encNEWi = server->getNEW(index);
	if(!issorted(encNEWi))
	{
	assert(encNEWi.size() == indexSize[index]+2*indexSize[index-1]);
	int upCnt = numNEW[index];
	for(int k =0; k<numberOfBins[index]; k++)
	{
			int cb = stoi((omaps[index]->find(getBid(to_string(k),upCnt))));
			assert(cb == sizeOfEachBin[index]);
	}
	server->resize(index,0);
    vector<prf_type> decodedNEWi;	
	for(auto n : encNEWi)
	{
		prf_type dec = n;
	    //Utilities::decode(n, dec, key);
		decodedNEWi.push_back(dec);
	}
	sort(decodedNEWi);
	encNEWi.resize(0);
	for(auto n : decodedNEWi)
	{
		prf_type enc = n;
		//enc = Utilities::encode(n.data(), key);
		encNEWi.push_back(enc);
	}
	server->putNEW(index, encNEWi);
	vector<prf_type> sortedNEW = server->getNEW(index);
	for(int i = 0; i<numberOfBins[index]; i++)
	{
		for(int j = 0; j<sizeOfEachBin[index]; j++)
		{
			int k = i*sizeOfEachBin[index]+j;
			prf_type val = sortedNEW[k];
    		int bina = *(int*) (&(val.data()[AES_KEY_SIZE - 10]));
			assert(bina == i);
		}
	}
	}
	else
		cout <<"ALREADY SORTED:"<<index<<endl;
	assert(issorted(encNEWi));
}
void OneChoiceClient::reSize(int index, int size)
{
	cout <<"resizing:"<<index<<endl;
	server->resize(index,size);
	vector<prf_type> encNEWi = server->getNEW(index);
	assert(encNEWi.size() == indexSize[index]);
	assert(issorted(encNEWi));
}
