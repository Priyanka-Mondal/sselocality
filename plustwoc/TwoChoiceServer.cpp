#include "TwoChoiceServer.h"
#include <string.h>

TwoChoiceServer::TwoChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
    storage = new TwoChoiceStorage(inMemory, dataIndex, "/tmp/", profile);
    storage->setup(overwrite);
    keywordCounters = new Storage(inMemory, dataIndex, "/tmp/keyword-", profile);
    keywordCounters->setup(overwrite);
}

TwoChoiceServer::~TwoChoiceServer() {}


void TwoChoiceServer::storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers, 
				 map<prf_type, prf_type> keywordCounter) 
{
    storage->insertAll(dataIndex, ciphers);
    keywordCounters->insert(dataIndex, keywordCounter);
}

pair<prf_type, vector<prf_type>> TwoChoiceServer::insertCuckooHT(int index, int tableNum, int cuckooID, 
						int hash, prf_type keyw, vector<prf_type> fileids)
{
	return storage->insertCuckooHT(index, tableNum, cuckooID, hash, keyw, fileids);
}

vector<prf_type> TwoChoiceServer::search(int dataIndex, prf_type tokkw, prf_type hashtoken, 
					int& keywordCnt, int num) 
{
    keywordCounters->seekgCount = 0;
    storage->readBytes = 0;
    double keywordCounterTime = 0, serverSearchTime = 0;
    if (profile) 
        Utilities::startTimer(35);
    
    prf_type curToken = tokkw;
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());
    bool found = false;
    prf_type res = keywordCounters->find(dataIndex, keywordMapKey, found);
    if (profile) 
    {
        keywordCounterTime = Utilities::stopTimer(35);
        printf("keyword counter Search Time:%f number of SeekG:%d number of read bytes:%d\n", 
			keywordCounterTime, keywordCounters->seekgCount, 
			keywordCounters->KEY_VALUE_SIZE * keywordCounters->seekgCount);
        Utilities::startTimer(45);
    }
    vector<prf_type> result;
	result.resize(0);
    if (found) 
    {
        prf_type plaintext;
        Utilities::decode(res, plaintext, curToken.data());
        keywordCnt = *(int*) (&(plaintext[0]));
		//cout <<"keywordcnt:"<<keywordCnt<<endl;
	if(keywordCnt > num)
		return result;
        curToken = hashtoken;
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());
        result = storage->find(dataIndex, keywordMapKey, keywordCnt);
        if (profile) 
	{
            serverSearchTime = Utilities::stopTimer(45);
            printf("server Search Time:%f number of SeekG:%d number of read bytes:%d\n", 
			    serverSearchTime, storage->SeekG, storage->readBytes);
        }
    }
    return result;
}


vector<pair<prf_type, prf_type> > TwoChoiceServer::getAllData(int dataIndex) 
{
    return storage->getAllData(dataIndex);
}

vector<prf_type> TwoChoiceServer::getCuckooHT(int dataIndex) 
{
    return storage->getCuckooHT(dataIndex);
}

vector<prf_type> TwoChoiceServer::getStash(int dataIndex) 
{
    return storage->getStash(dataIndex);
}

void TwoChoiceServer::insertCuckooStash(int index, int tableNum, vector<prf_type> ctCiphers)
{
    storage->insertCuckooStash(index, tableNum, ctCiphers);
}

vector<prf_type> TwoChoiceServer::cuckooSearch(int index, int tableNum, 
		        prf_type hashtoken1, prf_type hashtoken2)
{
	vector<prf_type> results;
        unsigned char cntstr[AES_KEY_SIZE];
	int entryNum = pow(2,(index-tableNum));
	int h[2];

        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        prf_type mapKey1 = Utilities::generatePRF(cntstr, hashtoken1.data());
        unsigned char* hash1 = Utilities::sha256((char*) (mapKey1.data()), AES_KEY_SIZE);
    	h[0] = (unsigned int) (*((int*) hash1)) % entryNum;

        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        prf_type mapKey2 = Utilities::generatePRF(cntstr, hashtoken2.data());
        unsigned char* hash2 = Utilities::sha256((char*) (mapKey2.data()), AES_KEY_SIZE);
    	h[1] = (unsigned int) (*((int*) hash2)) % entryNum;

	results = storage->cuckooSearch(index, tableNum, h);
	return results;
}

void TwoChoiceServer::clear(int index) 
{
    storage->clear(index);
    keywordCounters->clear(index);
}
