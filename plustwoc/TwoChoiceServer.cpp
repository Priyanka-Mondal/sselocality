#include "TwoChoiceServer.h"
#include <string.h>

TwoChoiceServer::TwoChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
    storage = new TwoChoiceStorage(inMemory, dataIndex, "/tmp/", profile);
    storage->setup(overwrite);
    keyworkCounters = new Storage(inMemory, dataIndex, "/tmp/keyword-", profile);
    keyworkCounters->setup(overwrite);
}

TwoChoiceServer::~TwoChoiceServer() {}


void TwoChoiceServer::storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers, vector<pair<pair<prf_type,prf_type>,pair<prf_type,vector<prf_type>>>> cuckooCiphers, map<prf_type, prf_type> keywordCounters) 
{
    storage->insertAll(dataIndex, ciphers);
    keyworkCounters->insert(dataIndex, keywordCounters);
    //storage->insertCuckooHT(dataIndex,cuckooCiphers);
}

pair<prf_type, vector<prf_type>> TwoChoiceServer::insertCuckooHT(int index, int tableNum, int hash, int cuckooID, prf_type keyw, vector<prf_type> fileids)
{
	return storage->insertCuckooHT(index, tableNum, hash, cuckooID, keyw, fileids);
}

vector<prf_type> TwoChoiceServer::search(int dataIndex, prf_type tokkw, prf_type hashtoken, int& keywordCnt, int num) 
{
    keyworkCounters->seekgCount = 0;
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
    prf_type res = keyworkCounters->find(dataIndex, keywordMapKey, found);
    if (profile) 
    {
        keywordCounterTime = Utilities::stopTimer(35);
        printf("keyword counter Search Time:%f number of SeekG:%d number of read bytes:%d\n", keywordCounterTime, keyworkCounters->seekgCount, keyworkCounters->KEY_VALUE_SIZE * keyworkCounters->seekgCount);
        Utilities::startTimer(45);
    }
    vector<prf_type> result;
    if (found) 
    {
        prf_type plaintext;
        Utilities::decode(res, plaintext, curToken.data());
        keywordCnt = *(int*) (&(plaintext[0]));
        curToken = hashtoken;
        memset(cntstr, 0, AES_KEY_SIZE);
	if(keywordCnt > num)
		keywordCnt = num;
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());
        result = storage->find(dataIndex, keywordMapKey, keywordCnt);
        if (profile) 
	{
            serverSearchTime = Utilities::stopTimer(45);
            printf("server Search Time:%f number of SeekG:%d number of read bytes:%d\n", serverSearchTime, storage->SeekG, storage->readBytes);
        }
    }
    return result;
}

/*
vector<prf_type> TwoChoiceServer::newsearch(int dataIndex , prf_type hashtoken,int keywordCnt, int pos) 
{
    keyworkCounters->seekgCount = 0;
    storage->readBytes = 0;
    vector<prf_type> result;
    
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type hashKey = Utilities::generatePRF(cntstr, hashtoken.data());
    result = storage->newfind(dataIndex, hashKey, keywordCnt,pos);
    return result;
}
*/

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
    //return storage->getStash(dataIndex);
}

vector<prf_type> TwoChoiceServer::cuckooSearch(int index, int tableNum, prf_type hashtoken1, prf_type hashtoken2)
{
	vector<prf_type> results;
        unsigned char cntstr[AES_KEY_SIZE];
	int entryNum = pow(2,(index-tableNum));
        prf_type mapKey;
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        mapKey = Utilities::generatePRF(cntstr, hashtoken1.data());
        unsigned char* hash1 = Utilities::sha256((char*) (mapKey.data()), AES_KEY_SIZE);
    	int h1 = (unsigned int) (*((int*) hash1)) % entryNum;

        mapKey = Utilities::generatePRF(cntstr, hashtoken2.data());
        unsigned char* hash2 = Utilities::sha256((char*) (mapKey.data()), AES_KEY_SIZE);
    	int h2 = (unsigned int) (*((int*) hash1)) % entryNum;
	results = storage->cuckooSearch(index, tableNum, h1, h2);
	vector<prf_type> results1;
	return results1;
}
void TwoChoiceServer::clear(int index) 
{
    storage->clear(index);
    keyworkCounters->clear(index);
}
