#include "TwoChoiceClient.h"
#include<vector>
#include<algorithm>

TwoChoiceClient::~TwoChoiceClient() 
{
    delete server;
}

TwoChoiceClient::TwoChoiceClient(int numOfDataSets, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
    cout <<"===================RUNNING SDa+TwoChoice====================="<<endl;
    server = new TwoChoiceServer(numOfDataSets, inMemory, overwrite, profile);
    for (int i = 0; i < numOfDataSets; i++) //why not <=
    {
        exist.push_back(false);
        stashExist.push_back(false);
        int curNumberOfBins = i > 3 ? ((int) ceil((float) pow(2, i) / ((log2(log2(pow(2,i))))*(log2(log2(log2(pow(2,i)))))*(log2(log2(log2(pow(2,i)))))))) : pow(2,i);
		curNumberOfBins = pow(2, (int)ceil(log2(curNumberOfBins)));
     	int curSizeOfEachBin = i > 3 ? ceil(2*(log2(log2(pow(2,i))))*(log2(log2(log2(pow(2,i)))))*(log2(log2(log2(pow(2,i)))))) : 2;
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
    }
}

int countTotal(map<int,int> fullness, int bin,int size)
{
	int full = 0;
	for(int i = 0; i< size; i++)
		full = full + fullness[bin+i];
	return full;
}

bool cmpp(pair<string, vector<prf_type>> &a, pair<string, vector<prf_type>> &b)
{
	//cout <<"cmp:["<<a.second.size()<< " "<<b.second.size()<<"]["<<(a.second.size() > b.second.size()) <<"]"<<endl;
    return (a.second.size() > b.second.size());
}

vector<pair<string, vector<prf_type>>> sort(map<string, vector<prf_type>> &M)
{
    vector<pair<string, vector<prf_type>>> A;
    for (auto& it : M) 
    {
	assert(it.first != "");
        A.push_back(it);
    }
    sort(A.begin(), A.end(), cmpp);
    return A;
}

int maxPossibleLen(int N, int bins, int index)
{
	int max;
	if(N<4)
		max = bins;
	else
	{
		float p = (float) ((float)1/log2(log2(N)));
		float m = (float) (1-p);
		max = (float) floor(pow(N,m));
	}
	int maxmax= pow(2, (int)ceil(log2(max)));
	int minmin= pow(2, (int)floor(log2(max)));
	if(maxmax<= bins)
		max = maxmax;
	else
		max = minmin;
	return max;
}


void TwoChoiceClient::writeToStash(int pss, int mpl, vector<prf_type> fileids, unsigned char* key, vector<prf_type> &stashCiphers )
{
	for (unsigned int i = mpl; i < pss; i++) 
	{
            prf_type value;
            value = Utilities::encode(fileids[i].data(), key);
            stashCiphers.push_back(value);
	}
}

void TwoChoiceClient::setup(int index, map<string, vector<prf_type> > pairs, unsigned char* key) 
{
    exist[index] = true;
    stashExist[index] = true;
    vector<vector<pair<prf_type, prf_type>>> ciphers;
    vector<prf_type> stashCiphers;
    for (int i = 0; i < numberOfBins[index]; i++) 
        ciphers.push_back(vector<pair<prf_type,prf_type>>());
    
    map<prf_type, prf_type> keywordCntCiphers;
    map<prf_type, prf_type> stashCntCiphers;
    map<int, int> fullness;

    vector<pair<string,vector<prf_type>>> sorted = sort(pairs);
    int mpl = maxPossibleLen((pow(2,index)),numberOfBins[index], index);
    //mpl = numberOfBins[index]; // for now

    for (auto pair : sorted) 
    {
		int pss = pair.second.size();
		int newsize = pow(2, (int)ceil(log2(pss)));
		if(pss > mpl)
		{
			writeToStash(pss,mpl,pair.second,key, stashCiphers);
			pss = mpl;
			newsize = mpl;
		}
		if(newsize > mpl)
			newsize = mpl;
		//cout <<"pss:"<<pss<<" ns:"<<newsize<<" mpl:"<<mpl<<endl;
	    prf_type K = Utilities::encode(pair.first, key);
		prf_type mapKey, mapValue;
	    unsigned char cntstr[AES_KEY_SIZE];
	    memset(cntstr, 0, AES_KEY_SIZE);
	    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
	    mapKey = Utilities::generatePRF(cntstr, K.data());
	    prf_type valueTmp, totalTmp;
	    *(int*) (&(valueTmp[0])) = newsize;//pair.second.size(); 
	    mapValue = Utilities::encode(valueTmp.data(), K.data());
	    keywordCntCiphers[mapKey] = mapValue; 
	       
	
	    string temp = pair.first;
		temp = temp.append("1");
	    prf_type K1 = Utilities::encode(temp, key);
	    prf_type mapKey1;
	    memset(cntstr, 0, AES_KEY_SIZE);
	    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
	    mapKey1 = Utilities::generatePRF(cntstr, K1.data());
	    unsigned char* hash1 = Utilities::sha256((char*) (mapKey1.data()), AES_KEY_SIZE);
	
		temp = pair.first;
		temp = temp.append("2");
	    prf_type K2 = Utilities::encode(temp, key);
	    prf_type mapKey2;
	    memset(cntstr, 0, AES_KEY_SIZE);
	    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
	    mapKey2 = Utilities::generatePRF(cntstr, K2.data());
	    unsigned char* hash2 = Utilities::sha256((char*) (mapKey2.data()), AES_KEY_SIZE);
	
	    int superBins = ceil((float) numberOfBins[index]/newsize); 
	    int pos1 = (unsigned int) (*((int*) hash1)) % superBins;
	    int pos2 = (unsigned int) (*((int*) hash2)) % superBins; 
	
		int totalItems1 = countTotal(fullness, pos1*newsize, newsize);
		int totalItems2 = countTotal(fullness, pos2*newsize, newsize);
		//assert(totalItems1 == totalItems2);
	        int cipherIndex ;
		if(totalItems1>totalItems2)
			cipherIndex = pos2*newsize;
		else
			cipherIndex = pos1*newsize;
	        
		for (unsigned int i = 0; i < pss; i++) 
		{
	    	prf_type mapKey, mapValue;
	    	unsigned char cntstr[AES_KEY_SIZE];
	    	memset(cntstr, 0, AES_KEY_SIZE);
	    	*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
	    	mapKey = Utilities::generatePRF(cntstr, K.data());
	    	mapValue = Utilities::encode(pair.second[i].data(), key);
	
	    	auto p = std::pair<prf_type, prf_type>(mapKey, mapValue);
	    	ciphers[cipherIndex].push_back(p);
	
		    if(fullness.find(cipherIndex) == fullness.end())
			    fullness[cipherIndex] = 1;
		    else
		    	    fullness[cipherIndex] = fullness[cipherIndex]+1;
	            cipherIndex++;
	    }
		for(int i = pss; i<newsize; i++)
		{
	    	prf_type dummy;
	    	memset(dummy.data(), 0, AES_KEY_SIZE);
	    	auto dummypair = std::pair<prf_type, prf_type>(dummy, dummy);
	        	ciphers[cipherIndex].push_back(dummypair);
		    fullness[cipherIndex] = fullness[cipherIndex]+1;
	        	cipherIndex++;
		}
    }
    prf_type dummy;
    memset(dummy.data(), 0, AES_KEY_SIZE);
    auto dummypair = pair<prf_type, prf_type>(dummy, dummy);
    for (int i = 0; i < numberOfBins[index]; i++)  //filling up each bin to maximum capacity
    {
        int curSize = ciphers[i].size();
        for (int j = curSize; j < sizeOfEachBin[index]; j++) 
		{
            ciphers[i].push_back(dummypair);
        }
    }
    prf_type randomKey;
    for (int i = 0; i < AES_KEY_SIZE; i++) 
        randomKey[i] = rand();

    for (int i = keywordCntCiphers.size(); i < pow(2, index); i++)  // what is this loop doing ?
    {
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 9])) = rand();
        prf_type mapKey = Utilities::generatePRF(cntstr, randomKey.data());
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = rand();
        prf_type mapValue = Utilities::generatePRF(cntstr, randomKey.data());
        keywordCntCiphers[mapKey] = mapValue;
    }
    totalCommunication += ciphers.size() * sizeof (prf_type)*2+ stashCiphers.size() * sizeof(prf_type);
    server->storeCiphers(index, ciphers, stashCiphers, keywordCntCiphers, stashCntCiphers);
}

vector<prf_type> TwoChoiceClient::search(int index, string keyword, unsigned char* key) 
{
    double searchPreparation = 0, searchDecryption = 0;
    int flag = 0;
    if (profile) 
        Utilities::startTimer(65);
    vector<prf_type> finalRes;
    int keywordCnt = 0;
    prf_type hashtoken;
    prf_type token = Utilities::encode(keyword, key);
    vector<prf_type> ciphers;
    vector<prf_type> stashCiphers;
    for(int s = 1 ;s<=2; s++)
    {
	string newkeyword = keyword;
	if(s==1) 
	{
	      newkeyword = newkeyword.append("1");
              hashtoken = Utilities::encode(newkeyword, key);
	}
	else if(s==2)
	{
	      newkeyword = keyword;
	      newkeyword = newkeyword.append("2");
              hashtoken = Utilities::encode(newkeyword, key);
	}
        ciphers = server->search(index, token, hashtoken, keywordCnt);
        stashCiphers = server->getStash(index);
       	if(flag == 0)
       	{
       		for (auto item : ciphers) 
       		{
       	 		prf_type plaintext;
       	 		Utilities::decode(item, plaintext, key);
       	 		if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
       			{
       	     			finalRes.push_back(plaintext);
       	    			flag++;
       	 		}
       		}
		if(flag !=0)//found in one superBin will imply NOT found in the other
		{
				cout<<"Index:"<<index<<" Searching in stash, size:"<<stashCiphers.size()*AES_KEY_SIZE<<endl;
       			for (auto item : stashCiphers) 
       			{
       		 		prf_type plaintext;
       		 		Utilities::decode(item, plaintext, key);
       		 		if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
       				{
       		     			finalRes.push_back(plaintext);
       		    			flag++;
       		 		}
       			}
		}
       	}
    totalCommunication += ciphers.size() * sizeof (prf_type) + sizeof (prf_type) + stashCiphers.size()* sizeof(prf_type);
    }
    return finalRes;
}

vector<prf_type> TwoChoiceClient::getAllData(int index, unsigned char* key) 
{
    vector<prf_type> finalRes = vector<prf_type>();
    auto ciphers = server->getAllData(index);
    vector<prf_type> stashCiphers = server->getStash(index);

    for (auto cipher : ciphers) 
    {
        prf_type plaintext;
        Utilities::decode(cipher.second, plaintext, key);
        finalRes.push_back(plaintext);
    }
    if(stashCiphers.size()>0)
    {
    	for(auto b : stashCiphers)
    	{
       	    prf_type plaintext;
       	    Utilities::decode(b, plaintext, key);
	    finalRes.push_back(plaintext);
        }
    }
    totalCommunication += (ciphers.size() + stashCiphers.size())* sizeof (prf_type);
    return finalRes;
}

void TwoChoiceClient::destry(int index) 
{
    server->clear(index);
    exist[index] = false;
    stashExist[index] = false;
    totalCommunication += sizeof (int);
}
void TwoChoiceClient::printStashSizes() 
{
	server->printStashSizes();
}
