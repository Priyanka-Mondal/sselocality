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

OneChoiceClient::OneChoiceClient(int N, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
	int l = ceil((float)log2(N));
    memset(nullKey.data(), 0, AES_KEY_SIZE);
	this->numOfIndices = l;
    server = new OneChoiceServer(numOfIndices, inMemory, overwrite, profile);
	bitonic = new Bitonic(N);
    for (int j = 0; j <=numOfIndices; j++) 
	{
        int curNumberOfBins = j > 1 ? 
			(int) ceil(((float) pow(2, j))/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        int curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*(log2(log2(pow(2, j))))) : pow(2,j);
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
		int is = curNumberOfBins*curSizeOfEachBin;
		indexSize.push_back(is);
        printf("Index:%d #of Bins:%d size of bin:%d is:%d\n", j, curNumberOfBins, curSizeOfEachBin, is);
    }
	exist.resize(numOfIndices+1);
    for (int i = 0; i <=numOfIndices; i++) 
	{
		exist[i].resize(4);
    	for (int j = 0; j < 4; j++) 
		{
            exist[i].push_back(false);
        }
		numNEW.push_back(1);//updtCnt
		NEWsize.push_back(0);
		KWsize.push_back(0);
    }
    for (int i = 0; i <=numOfIndices; i++) 
	{
        bytes<Key> key{0};
        OMAP* omap = new OMAP(max((int) pow(2, i+2), 16), key);
        omaps.push_back(omap);
    }
	exist[0][3] = true;
}

int issorted(vector<prf_type> A)
{
	for(int a=0;a<A.size()-1;a++)
	{
    	int bina = *(int*) (&(A[a].data()[AES_KEY_SIZE - 11]));
    	int binb = *(int*) (&(A[a+1].data()[AES_KEY_SIZE - 11]));
		//cout <<"bin:"<<bina<<"/"<<binb<<endl;
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
		//cout <<"prp:"<<prpa<<"/"<<prpb<<endl;
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
	prf_type K = Utilities::encode(keyword, key);
	int keywordCount = server->getCounter(index, instance, K);
	cout<<"("<<index<<","<<instance<<")"<<keywordCount<<endl;
	vector<prf_type> ciphers = server->search(index, instance, K, keywordCount);
	ciphers = server->getAllData(index, instance);
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
    return finalRes;
}

void OneChoiceClient::move(int index, int toInstance, int fromInstance)
{
	server->destroy(index, toInstance);	
	server->move(index, toInstance, fromInstance, indexSize[index]);
	server->destroy(index, fromInstance);	
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
	NEWsize[index] = NEWsize[index]+1;
	assert(last == NEWsize[index]*AES_KEY_SIZE);
}

void OneChoiceClient::destroy(int index, int instance)
{
    server->destroy(index, instance);
    exist[index][instance] = false;
	if(instance == 3)
	{
		NEWsize[index]=0;
		KWsize[index]=0;
	}
}
void OneChoiceClient::resize(int index, int size)
{
	server->resize(index, size, NEWsize[index]);
	NEWsize[index]=size;
}

void OneChoiceClient::getBin(int index, int instance, int count, int numOfBins,
							 unsigned char* key, unsigned char* keynew)
{
	int start = count * sizeOfEachBin[index-1];
	int readSize = numOfBins*sizeOfEachBin[index-1];
	assert(start + readSize <= indexSize[index-1]);
	vector<prf_type> ciphers = server->getElements(index-1, instance, start, readSize);
	int upCnt = numNEW[index];
	for(prf_type c: ciphers)
	{
	    prf_type plaintext;
	    Utilities::decode(c, plaintext, key);
	    //int ind = *(int*) (&(plaintext.data()[AES_KEY_SIZE - 5]));
	    //int op = ((byte) plaintext.data()[AES_KEY_SIZE - 6]); 
	    string w((char*) plaintext.data());
		int cnt = 0;
		if(w!="")
			cnt = stoi(omaps[index]->incrementCnt(getBid(w, upCnt)));
		//else do dummy omap access
		int newbin = hashKey(w, cnt, index, keynew);		
		//prf_type keyVal;
    	//memset(keyVal.data(), 0, AES_KEY_SIZE);
		//std::copy(w.begin(), w.end(), keyVal.begin());
		//*(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = ind; 
		//keyVal.data()[AES_KEY_SIZE - 6] = (byte) (op == 0 ? OP::INS : OP::DEL);
		//*(int*) (&(keyVal.data()[AES_KEY_SIZE - 11])) = newbin;
		//append(index, keyVal, keynew);
		*(int*) (&(plaintext.data()[AES_KEY_SIZE - 11])) = newbin;
		append(index, plaintext, keynew);
		if(w != "")
			string ob = omaps[index]->incrementCnt(getBid(to_string(newbin),upCnt));
		//else do dummy omap access
	}
}

void OneChoiceClient::kwCount(int index, int count, int bins, unsigned char* key)
{
	int upCnt = numNEW[index];
	int start = 2*count*sizeOfEachBin[index-1];
	vector<prf_type> some = server->getNEW(index, start, bins*sizeOfEachBin[index-1], true);

	assert(NEWsize[index] == 2*indexSize[index-1]);
	assert(start+bins*sizeOfEachBin[index-1] <= 2*indexSize[index-1]);
	assert(count<numberOfBins[index-1]);
	assert(some.size() == bins*sizeOfEachBin[index-1]);

	for(auto c : some)
	{
	    prf_type plaintext;
	    Utilities::decode(c, plaintext, key);
	    string w((char*) plaintext.data());
		int cntw = 0;
		if(w!="")
			cntw = stoi(omaps[index]->find(getBid(w, upCnt)));
		else 
		{
			cntw = 0;
			//dummy access to OMAP
		}
		prf_type keyVal;
    	memset(keyVal.data(), 0, AES_KEY_SIZE);
	    std::copy(w.begin(), w.end(), keyVal.begin());
	    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = cntw; 
	    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 11])) = cntw;//here goes the PRP later
		appendTokwCounter(index, keyVal, key);
	}
	assert(KWsize[index] <=2*indexSize[index-1]);
}
void OneChoiceClient::addDummy(int index, int count, int numOfBins, unsigned char* key)
{
	assert(NEWsize[index] >= 2*indexSize[index-1]);
	assert(NEWsize[index] <= 2*indexSize[index-1]+ indexSize[index]);
    int upCnt = numNEW[index];

	for(int bin = count; bin < count + numOfBins; bin++)
	{
		assert(bin < numberOfBins[index]);
		int cbin;
		string cb = omaps[index]->find(getBid(to_string(bin),upCnt));
		if(cb == "")
			cbin = 0;
		else 
			cbin = stoi(cb);
		assert(cbin <= sizeOfEachBin[index]);
		prf_type realDummy;
    	memset(realDummy.data(), 0, AES_KEY_SIZE);
    	*(int*) (&(realDummy.data()[AES_KEY_SIZE - 11])) = bin;//bin
		for(int k = cbin; k < sizeOfEachBin[index]; k++)
		{
			append(index, realDummy, key); 
			string ob = omaps[index]->incrementCnt(getBid(to_string(bin), upCnt));
		}
		prf_type dummy;
    	memset(dummy.data(), 0, AES_KEY_SIZE);
    	*(int*) (&(dummy.data()[AES_KEY_SIZE - 11])) = INF;//bin
		for(int k = 0; k<cbin ; k++)
		{ 
			append(index, dummy, key);
			//dummy omap access here
		}
	}
	if((count == numberOfBins[index]-1) || index <= 3)
	{
		int newSize = pow(2, ceil((float)log2(NEWsize[index])));
		pad(index, newSize, key);
	}
}

void OneChoiceClient::pad(int index, int newSize, unsigned char* key)
{
	prf_type dummy;
   	memset(dummy.data(), 0, AES_KEY_SIZE);
    *(int*) (&(dummy.data()[AES_KEY_SIZE - 11])) = INF;//bin
	int size = NEWsize[index];
	for(int k = 0; k < newSize-size ; k++)
	{ 
		append(index, dummy, key);
		//**dummy omap access here
	}
	int kwsize = KWsize[index];
   	memset(dummy.data(), 0, AES_KEY_SIZE);
    *(int*) (&(dummy.data()[AES_KEY_SIZE - 11])) = 0;//prp
	for(int k = 0; k <newSize-kwsize; k++)
	{
		appendTokwCounter(index, dummy, key);
	}
	assert(NEWsize[index] == KWsize[index]);
}

void OneChoiceClient::updateHashTable(int index, unsigned char* key)
{
	assert(KWsize[index] < 8*indexSize[index]);
	vector<prf_type> all = server->getNEW(index, 0, KWsize[index], false); // for first pow(2, index) items
	map <prf_type, prf_type> kcc;
	for(auto c: all)
	{
	    prf_type plaintext;
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
		if(kcc.find(mapKey) != kcc.end())
		{
			prf_type oldVal = kcc[mapKey];
			prf_type plaintext;
        	Utilities::decode(kcc[mapKey], plaintext, K.data());
        	int keywordCnt = *(int*) (&(plaintext[0]));
			if(keywordCnt < cntw)
			{
				kcc[mapKey] = mapValue; 
			}		
			if(keywordCnt != cntw)
				cout <<"no upadte required:"<< keywordCnt<< ":"<<cntw<<endl;
		}
		else
			kcc[mapKey] = mapValue;
	}
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

bool OneChoiceClient::sorted(int index, unsigned char* key)
{
	vector<prf_type> els = server->getNEW(index, 0,NEWsize[index], true);
	vector<prf_type> decoded;
	for(auto n :els)
	{
		prf_type plain;
		Utilities::decode(n, plain, key);
		decoded.push_back(plain);
	}
	vector<prf_type> els2 = server->getNEW(index, 0, KWsize[index], false);
	vector<prf_type> decoded2;
	for(auto n :els2)
	{
		prf_type plain;
		Utilities::decode(n, plain, key);
		decoded2.push_back(plain);
	}

	int one =  issorted(decoded);
	int two = 1;
	two =  issortedC(decoded2);
sleep(1);
cout <<index<<":"<<one <<" "<<two<<endl;
if(one+two == 2)
return true;
else return false;
	
}
void OneChoiceClient::deAmortBitSortC(int step, int count, int size, int index, unsigned char* key)
{
	assert(NEWsize[index]==KWsize[index]);
	vector<int> curMem = bitonic->getSeq(step, count, size);
	std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
	vector<int> ncm = bitonic->remDup(curMem);

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

void OneChoiceClient::deAmortBitSort(int step, int count, int size, int index, unsigned char* key)
{
	assert(NEWsize[index]==KWsize[index]);
	vector<int> curMem = bitonic->getSeq(step, count, size);
	std::sort(curMem.begin(), curMem.end(), [](int a, int b) {return a < b;});
	vector<int> ncm = bitonic->remDup(curMem);

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
	for(int i = 0 ; i< ncm.size() ; i++)
	{
		encNEW[ncm[i]] = sorted1[cnt];
		cnt++;
	}
	assert(encNEW.size() == size);
	server->putNEW(index, 3, encNEW);
}

int OneChoiceClient::getNEWsize(int index)
{
	return NEWsize[index];
}
