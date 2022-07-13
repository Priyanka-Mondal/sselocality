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
	int b = ceil((float)log2(B));
	this->numOfIndices = l - b;
    server = new OneChoiceServer(l-b, inMemory, overwrite, profile);
    for (int i = 0; i <=numOfIndices; i++) 
	{
		int j = i + b;
        int curNumberOfBins = j > 1 ? 
			(int) ceil((float) pow(2, j)/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        int curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : pow(2,j);
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
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
	//exist[0][3] = true;
    for (int i = 0; i <=numOfIndices; i++) 
	{
        bytes<Key> key{0};
        OMAP* omap = new OMAP(max((int) pow(2, i+2), 16), key);
        omaps.push_back(omap);
    }
}
/*
void OneChoiceClient::setup(int index, unordered_map<string, vector<prf_type> > pairs, unsigned char* key) 
{
    //exist[index] = true;
    vector<vector<pair<prf_type, prf_type> > > ciphers;
    for (int i = 0; i < numberOfBins[index]; i++) 
	{
        ciphers.push_back(vector<pair<prf_type, prf_type> >());
    }
    //map<prf_type, prf_type> keywprdCntCiphers;
    for (auto pair : pairs) 
	{
        prf_type K1 = Utilities::encode(pair.first, key);
        prf_type mapKey, mapValue;
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        mapKey = Utilities::generatePRF(cntstr, K1.data());
        prf_type valueTmp;
        *(int*) (&(valueTmp[0])) = pair.second.size();
        mapValue = Utilities::encode(valueTmp.data(), K1.data());
        //keywprdCntCiphers[mapKey] = mapValue;

        unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
        int pos = (unsigned int) (*((int*) hash)) % numberOfBins[index];
        int cipherIndex = pos;
        for (unsigned int i = 0; i < pair.second.size(); i++) {
            prf_type mapKey, mapValue;
            unsigned char cntstr[AES_KEY_SIZE];
            memset(cntstr, 0, AES_KEY_SIZE);
            *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
            mapKey = Utilities::generatePRF(cntstr, K1.data());
            mapValue = Utilities::encode(pair.second[i].data(), key);
            auto p = std::pair<prf_type, prf_type>(mapKey, mapValue);
            ciphers[cipherIndex].push_back(p);
            cipherIndex++;
            if (cipherIndex == numberOfBins[index]) {
                cipherIndex = 0;
            }
        }
    }
    prf_type dummy;
    memset(dummy.data(), 0, AES_KEY_SIZE);
    auto dummypair = pair<prf_type, prf_type>(dummy, dummy);
    for (int i = 0; i < numberOfBins[index]; i++) {
        int curSize = ciphers[i].size();
        for (int j = curSize; j < sizeOfEachBin[index]; j++) {
            ciphers[i].push_back(dummypair);
        }
    }

    prf_type randomKey;
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        randomKey[i] = rand();
    }
	
    for (int i = keywprdCntCiphers.size(); i < pow(2, index); i++) 
	{
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 9])) = rand();
        prf_type mapKey = Utilities::generatePRF(cntstr, randomKey.data());
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = rand();
        prf_type mapValue = Utilities::generatePRF(cntstr, randomKey.data());
        keywprdCntCiphers[mapKey] = mapValue;
    }
	
    //    totalCommunication += ciphers.size() * sizeof (prf_type)*2;
    //server->storeCiphers(index, ciphers, keywprdCntCiphers);
}
*/
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
	int bin = map(keyword, cnt, index, key);
	do
	{
    	vector<prf_type> ciphers = server->search(index, instance, bin);
		cout <<"ciphers size:"<<ciphers.size()<<endl;
		flag = 0;
    	totalCommunication += ciphers.size() * sizeof (prf_type) ;
	    for (auto item : ciphers) 
		{
	        prf_type plaintext = item;
	        //Utilities::decode(item, plaintext, key);
	       	if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
			{
	           	finalRes.push_back(plaintext);
				flag =1;
				cout<<" MATCH:"<<plaintext.data()<<" "<<bin<<endl;
	       	}
		//cout <<"bin:"<<bin<<"/"<<numberOfBins[index]<<endl;
		}
		if(bin == numberOfBins[index]-1)
			bin = 0;
		else
			bin++;
		cnt++;
		cout <<"flag:"<<flag<<"increasing count"<<cnt<<"/"<<numberOfBins[index]<<endl;
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
}
void OneChoiceClient::copy(int index, int toInstance)
{
	server->copy(index, toInstance);
	exist[index][toInstance] = true;
	numNEW[index] = numNEW[index] + 1;
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

void OneChoiceClient::getBin(int index, int instance, int start, int end, int updateCounter, 
							 unsigned char* key1, unsigned char* key2)
{
	if(start <=numberOfBins[index]*sizeOfEachBin[index] && end<=numberOfBins[index]*sizeOfEachBin[index])
	{
		vector<prf_type> ciphers = server->getElements(index-1, instance, start, end);
		int upCnt = numNEW[index];
		cout <<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		for(auto c: ciphers)
		{
	        prf_type plaintext = c;
			//cout <<"[ "<<c.data() <<" ]"<<endl;
	        //Utilities::decode(c, plaintext, key1);
	        prf_type decodedString = plaintext;
	        int ind = *(int*) (&(decodedString.data()[AES_KEY_SIZE - 5]));
	        int op = ((byte) decodedString.data()[AES_KEY_SIZE - 6]); 
	        string w((char*) plaintext.data());
			int cnt=0;
			if(w!="")
				cnt = stoi(omaps[index]->incrementCnt(getBid(w, upCnt)));
			string newCnt= omaps[index]->find(getBid(w,upCnt));
			int bin = map(w, cnt, index, key2);		
			cout <<"from index:"<<index-1<<" instance:"<<instance<<" upCnt:"<<upCnt <<" "<<cnt;
			cout <<"["<<w<<"|"<<ind<<"|"<<bin<<"]/"<<numberOfBins[index]-1<<endl;
			int realbin;
			if(w=="")
				realbin = INF;
			else
				realbin = bin;
	    	prf_type keyVal;
	    	std::fill(keyVal.begin(), keyVal.end(), 0);
	    	std::copy(w.begin(), w.end(), keyVal.begin());//keyword
	    	*(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = ind;//fileid
	    	keyVal.data()[AES_KEY_SIZE - 6] = (byte) (op);//op
	    	*(int*) (&(keyVal.data()[AES_KEY_SIZE - 10])) = realbin;//bin
			append(index, keyVal, key2);
			string ob = omaps[index]->incrementCnt(getBid(to_string(bin),upCnt));
		}
		if(server->getNEWsize(index) >= (instance+1)*numberOfBins[index]*sizeOfEachBin[index])
		{
			exist[index][instance] = false;
		}
	}
}
void OneChoiceClient::addDummy(int index, int count, int updateCounter, unsigned char* key)
{
	//cout<<index <<" size of NEW before adding dummy:"<<server->getNEWsize(index)<<endl;
    int upCnt = numNEW[index];
	int s;
	s = index>1 ? 6 : 2;
	for(int t = 0; t<s; t++)
	{
		int bin = count*s+t;
		//cout <<"ADDdummy count:"<<count<<" upCnt:"<<upCnt <<" index:"<<index<<" bin:"<<bin<<endl;
		if(bin < numberOfBins[index])
		{
			int cbin;
			Bid b =getBid(to_string(bin),upCnt);
			string cb = (omaps[index]->find(getBid(to_string(bin),upCnt)));
			if(cb == "")
				cbin = 0;
			else 
				cbin = stoi(cb);
			//cout <<"current bin size:"<<cbin<<"/"<<sizeOfEachBin[index]<<endl;
			for(int k = cbin; k<sizeOfEachBin[index]; k++)
			{
			//	cout <<"adding REAL dummy:"<<k<<endl;
				prf_type value;
	    		std::fill(value.begin(), value.end(), 0);
    			*(int*) (&(value.data()[AES_KEY_SIZE - 10])) = bin;//bin
				append(index, value, key);
			}
			for(int k = 0; k<cbin ; k++)
			{
			//	cout <<"adding dummy dummy"<<endl;
				prf_type value;
	    		std::fill(value.begin(), value.end(), 0);
    			*(int*) (&(value.data()[AES_KEY_SIZE - 10])) = INF;//bin
				append(index, value, key);
			}
		}
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
	if(w=="")
		cnt = 0;

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
	sort(A.begin(), A.end(), cmpp);
	for(auto a : A)
	{
    	int bina = *(int*) (&(a.data()[AES_KEY_SIZE - 10]));
		//cout <<"{"<<bina<<"}";
	}
	return A;
}
void OneChoiceClient::nonOblSort(int index, unsigned char* key)
{
	vector<prf_type> encNEWi = server->getNEW(index);
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
}
vector<prf_type> OneChoiceClient::searchNEW(int index, string keyword)
{
	cout <<"searching new"<<endl;
	vector<prf_type> ni = server->getNEW(index);
	vector<prf_type> nii;
	for(auto a : ni)
	{
	    if (strcmp((char*) a.data(), keyword.data()) == 0) 
			nii.push_back(a);
	}
	return nii;
}

/*
check all the cases of s and t
why is bin assigned are not proper
*/
