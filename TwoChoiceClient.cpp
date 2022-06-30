#include "TwoChoiceClient.h"
#include<vector>
#include<algorithm>

TwoChoiceClient::~TwoChoiceClient() {
    delete server;
}

TwoChoiceClient::TwoChoiceClient(int numOfDataSets, bool inMemory, bool overwrite, bool profile) {
    this->profile = profile;
    cout <<"RUNNING 2 choice====================="<<endl;
    server = new TwoChoiceServer(numOfDataSets, inMemory, overwrite, profile);
    for (int i = 0; i < numOfDataSets; i++) //why not <=
    {
        exist.push_back(false);
        int curNumberOfBins = i > 3 ? ((int) ceil((float) pow(2, i) / (log2(log2(pow(2,i)))*(log2(log2(log2(pow(2,i)))))))) : 1;
	curNumberOfBins = pow(2, (int)ceil(log2(curNumberOfBins))); 
        int curSizeOfEachBin = i > 3 ? (log2(log2(pow(2,i)))*(log2(log2(log2(pow(2,i)))))*3) : 3*pow(2,i);
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
    }
}

int countTotal(map<int,int> fullness, int bin,int size)
{
	int full = 0;
	for(int i = 0; i< size; i++)
	{
		full = full + fullness[bin+i];
	}
	return full;
}

bool cmp(pair<string, vector<prf_type>> &a,
         pair<string, vector<prf_type>> &b)
{
	cout <<"cmp:["<<a.second.size()<< " "<<b.second.size()<<"]["<<(a.second.size() > b.second.size()) <<"]"<<endl;
    return (a.second.size() > b.second.size());
}

vector<pair<string, vector<prf_type>>> sort(map<string, vector<prf_type>> &M)
{
    vector<pair<string, vector<prf_type>> > A;
    for (auto& it : M) {
        A.push_back(it);
    }
    sort(A.begin(), A.end(), cmp);
return A;
}


void TwoChoiceClient::setup(int index, map<string, vector<prf_type> > pairs, unsigned char* key) 
{
    exist[index] = true;
    vector<vector<pair<prf_type, prf_type> > > ciphers;
    for (int i = 0; i < numberOfBins[index]; i++) 
    {
        ciphers.push_back(vector<pair<prf_type,prf_type>>());
    }
    cout <<"pairs size:"<<pairs.size()<<endl;
    map<prf_type, prf_type> keywordCntCiphers;
    map<int, int> fullness;
    vector<pair<string,vector<prf_type>>> sorted = sort(pairs);
    for (auto pair : sorted) 
    {
	    cout <<"pair size:"<<pair.second.size()<<endl;
	int nearPow2 = pow(2, (int)ceil(log2(pair.second.size())));
	//if(nearPow2 <= numberOfBins[index])
	//	pair.second.resize(nearPow2);

        prf_type K1 = Utilities::encode(pair.first.append("1"), key);
        prf_type K2 = Utilities::encode(pair.first.append("2"), key);
        prf_type mapKey1, mapValue1;
        prf_type mapKey2, mapValue2;
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        mapKey1 = Utilities::generatePRF(cntstr, K1.data());
        mapKey2 = Utilities::generatePRF(cntstr, K2.data());
        prf_type valueTmp;
        *(int*) (&(valueTmp[0])) = pair.second.size();
        mapValue1 = Utilities::encode(valueTmp.data(), K1.data());
        mapValue2 = Utilities::encode(valueTmp.data(), K2.data());
        keywordCntCiphers[mapKey1] = mapValue1; //

        unsigned char* hash1 = Utilities::sha256((char*) (mapKey1.data()), AES_KEY_SIZE,0);
        unsigned char* hash2 = Utilities::sha256((char*) (mapKey2.data()), AES_KEY_SIZE,9);//change it
        int superBins = (float) numberOfBins[index]/nearPow2;
        int pos1 = (unsigned int) (*((int*) hash1)) % superBins;
        int pos2 = (unsigned int) (*((int*) hash2)) % superBins; 
	int totalItems1 = countTotal(fullness, pos1*nearPow2, nearPow2);
	int totalItems2 = countTotal(fullness, pos2*nearPow2, nearPow2);
        int cipherIndex = totalItems1>totalItems2 ? pos2*nearPow2 : pos1*nearPow2 ;
        //cout <<"sb:"<<superBins<<" bin:"<< numberOfBins[index]<<" nearpow2:"<<nearPow2<<endl;
	//cout <<"pos:"<<pos1<<" pos:"<<pos2 <<endl;
	//cout <<"tot1:"<<totalItems1<<" tot2:"<<totalItems2<<endl;
        
	for (unsigned int i = 0; i < pair.second.size(); i++) 
	{
            prf_type mapKey, mapValue;
            unsigned char cntstr[AES_KEY_SIZE];
            memset(cntstr, 0, AES_KEY_SIZE);
            *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
            mapKey = Utilities::generatePRF(cntstr, K1.data());
            mapValue = Utilities::encode(pair.second[i].data(), key);
            auto p = std::pair<prf_type, prf_type>(mapKey, mapValue);
            ciphers[cipherIndex].push_back(p);
	    if(fullness.find(cipherIndex) == fullness.end())
		    fullness[cipherIndex] = 1;
	    else
	    	    fullness[cipherIndex] = fullness[cipherIndex]+1;
	    //cout <<"ind:"<<index<<" bin:"<<cipherIndex<<" full:"<<fullness[cipherIndex]<<endl;
            cipherIndex++;
	   //no wrap around in 2-choice
           //if (cipherIndex == numberOfBins[index]) 
             //  cipherIndex = 0;
        }
	for(int i = pair.second.size(); i<nearPow2; i++)
	{
    		prf_type dummy;
    		memset(dummy.data(), 0, AES_KEY_SIZE);
    		auto dummypair = std::pair<prf_type, prf_type>(dummy, dummy);
            	ciphers[cipherIndex].push_back(dummypair);
	    	fullness[cipherIndex] = fullness[cipherIndex]+1;
	        //cout <<"ind:"<<index<<" bin:"<<cipherIndex<<" full:"<<fullness[cipherIndex]<<endl;
            	cipherIndex++;
	}
    }
    prf_type dummy;
    memset(dummy.data(), 0, AES_KEY_SIZE);
    auto dummypair = pair<prf_type, prf_type>(dummy, dummy);
    for (int i = 0; i < numberOfBins[index]; i++) 
    {
        int curSize = ciphers[i].size();
        for (int j = curSize; j < sizeOfEachBin[index]; j++) 
	{
            ciphers[i].push_back(dummypair);
        }
    }

    prf_type randomKey;
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        randomKey[i] = rand();
    }
    for (int i = keywordCntCiphers.size(); i < pow(2, index); i++) 
    {
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 9])) = rand();
        prf_type mapKey = Utilities::generatePRF(cntstr, randomKey.data());
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = rand();
        prf_type mapValue = Utilities::generatePRF(cntstr, randomKey.data());
        keywordCntCiphers[mapKey] = mapValue;
    }
    //    totalCommunication += ciphers.size() * sizeof (prf_type)*2;
    server->storeCiphers(index, ciphers, keywordCntCiphers);
    //cout <<"_____________________________________________"<<endl;
}

vector<prf_type> TwoChoiceClient::search(int index, string keyword, unsigned char* key) {
    double searchPreparation = 0, searchDecryption = 0;
    int flag = 0;
    if (profile) {
        Utilities::startTimer(65);
    }
    vector<prf_type> finalRes;
    for(int s = 1 ;s<=2;s++)
    {
    keyword = keyword.append("s");
    prf_type token = Utilities::encode(keyword, key);
    int keywordCnt = 0;
    vector<prf_type> ciphers = server->search(index, token, keywordCnt);
    int cnt = 0;
    if (profile) 
    {
        searchPreparation = Utilities::stopTimer(65);
        //printf("search preparation time:%f include server time\n", searchPreparation);
        Utilities::startTimer(65);
    }

if(flag == 0)
{
    for (auto item : ciphers) 
    {
        prf_type plaintext;
        Utilities::decode(item, plaintext, key);
        //        if (string((char*) plaintext.data()) == keyword) {
        if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
	{
            finalRes.push_back(plaintext);
	    flag = 1;
        }
    }
}
    if (profile) 
    {
        searchDecryption = Utilities::stopTimer(65);
        //printf("search decryption time:%f for decrypting:%d ciphers\n", searchDecryption, ciphers.size());
    }
    totalCommunication += ciphers.size() * sizeof (prf_type) + sizeof (prf_type);
    }
    return finalRes;
}

vector<prf_type> TwoChoiceClient::getAllData(int index, unsigned char* key) {
    vector<prf_type> finalRes;
    auto ciphers = server->getAllData(index);
    for (auto cipher : ciphers) {
        prf_type plaintext;
        Utilities::decode(cipher.second, plaintext, key);
        finalRes.push_back(plaintext);
    }
    totalCommunication += ciphers.size() * sizeof (prf_type);
    return finalRes;
}

void TwoChoiceClient::destry(int index) {
    server->clear(index);
    exist[index] = false;
    totalCommunication += sizeof (int);
}
