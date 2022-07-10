#include "OneChoiceClient.h"

OneChoiceClient::~OneChoiceClient() 
{
    delete server;
}

OneChoiceClient::OneChoiceClient(int N, vector<OMAP*> omaps, 
		bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
	int l = ceil((float)log2(N));
    memset(nullKey.data(), 0, AES_KEY_SIZE);
	int b = ceil((float)log2(B));
	int numOfIndices = l - b;
    for (int i = 0; i <=numOfIndices; i++) 
	{
		int j = i + b;
        int curNumberOfBins = i > 1 ? 
			(int) ceil((float) pow(2, j)/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))):1;
        int curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*log2(log2(pow(2, j)))):3;
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
    }
    server = new OneChoiceServer(numOfIndices, omaps, inMemory, overwrite, profile);
}

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
	/*
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
	*/
    //    totalCommunication += ciphers.size() * sizeof (prf_type)*2;
    //server->storeCiphers(index, ciphers, keywprdCntCiphers);
}

vector<prf_type> OneChoiceClient::search(int index, string keyword, unsigned char* key) {
    double searchPreparation = 0, searchDecryption = 0;
    if (profile) {
        Utilities::startTimer(65);
    }
    vector<prf_type> finalRes;
    prf_type token = Utilities::encode(keyword, key);
    int keywordCnt = 0;
    vector<prf_type> ciphers = server->search(index, token, keywordCnt);
    int cnt = 0;
    if (profile) {
        searchPreparation = Utilities::stopTimer(65);
        printf("search preparation time:%f include server time\n", searchPreparation);
        Utilities::startTimer(65);
    }

    //    for (int i = 0; i < ciphers.size(); i++) {
    //        prf_type curToken = token, mapKey;
    //        unsigned char cntstr[AES_KEY_SIZE];
    //        memset(cntstr, 0, AES_KEY_SIZE);
    //        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = cnt;
    //        mapKey = Utilities::generatePRF(cntstr, curToken.data());
    //        existkeys[mapKey] = true;
    //        cnt++;
    //    }

    for (auto item : ciphers) {
        prf_type plaintext;
        Utilities::decode(item, plaintext, key);
        //        if (string((char*) plaintext.data()) == keyword) {
        if (strcmp((char*) plaintext.data(), keyword.data()) == 0) {
            finalRes.push_back(plaintext);
        }
    }

    if (profile) 
	{
        searchDecryption = Utilities::stopTimer(65);
        cout<<"search decryption time:"<<searchDecryption<<" for decrypting:"<<ciphers.size()<<" ciphers"<<endl;
    }
    totalCommunication += ciphers.size() * sizeof (prf_type) + sizeof (prf_type);
    return finalRes;
}

vector<prf_type> OneChoiceClient::getAllData(int index, unsigned char* key) 
{
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

void OneChoiceClient::copy(int index, int toInstance, int fromInstance)
{
	server->copy(index, toInstance, fromInstance);
	exist[index][toInstance] = true;
}
void OneChoiceClient::append(int instance, prf_type keyVal)
{
	server->append(instance, keyVal);
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

void OneChoiceClient::getBin(int index, int instance, int start, int end, int updateCounter, unsigned char* key)
{
	vector<prf_type> ciphers = server->getElements(index, instance, start, end);
    int upCnt = (int)ceil((updateCounter-(6*pow(2, index-1)-2))/pow(2, index))+1; //???????
	for(auto c: ciphers)
	{
        prf_type plaintext;
        Utilities::decode(c, plaintext, key);
        prf_type decodedString = plaintext;
        int ind = *(int*) (&(decodedString.data()[AES_KEY_SIZE - 5]));
        int op = ((byte) decodedString.data()[AES_KEY_SIZE - 6]); 
        string w((char*) plaintext.data());
		int cnt = stoi(omaps[index]->incrementCnt(getBid(w, upCnt)));
		int bin = map(w, cnt, index, key);		

		prf_type value;
	    std::fill(value.begin(), value.end(), 0);
	    std::copy(w.begin(),w.end(), value.begin());//keyword
	    *(int*) (&(value.data()[AES_KEY_SIZE - 5])) = ind;//fileid
	    value.data()[AES_KEY_SIZE - 6] = (byte) (op);//op
		value.data()[AES_KEY_SIZE - 7] = (byte) (bin); // bin
		server->append(index, value);
		omaps[index]->incrementCnt(getBid(to_string(bin),upCnt));
	}
}
void OneChoiceClient::addDummy(int index, int count, int updateCounter)
{
    int upCnt = (int)ceil((updateCounter-(6*pow(2, index-1)-2))/pow(2, index))+1; //???????
	for(int t = 0; t<S; t++)
	{
		int bin = count*S+t;
		if(bin < numberOfBins[index])
		{
			int cbin = stoi(omaps[index]->find(getBid(to_string(bin),upCnt)));
			for(int k = cbin; k<3*sizeOfEachBin[index]; k++)
			{
				prf_type value;
	    		std::fill(value.begin(), value.end(), 0);
				value.data()[AES_KEY_SIZE - 7] = (byte) (bin); // bin
				server->append(index, value);
			}
			for(int k = 0; k<cbin ; k++)
			{
				prf_type value;
				value.data()[AES_KEY_SIZE - 7] = (byte) (0); // bin
				server->append(index, value);
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
    prf_type K = Utilities::encode(w, key);
    prf_type mapKey, mapValue;
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = cnt;
    mapKey = Utilities::generatePRF(cntstr, K.data());
    unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
    int bin = (unsigned int) (*((int*) hash)) % numberOfBins[index];
	return bin;
}
void OneChoiceClient::bitonicSort(int index)
{
	//do the actual sort here
	//but get the elements from the server
}
