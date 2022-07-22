#include "TwoChoiceServer.h"
#include <string.h>

TwoChoiceServer::TwoChoiceServer(long dataIndex, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
    storage = new TwoChoiceStorage(inMemory, dataIndex, "/tmp/", profile);
    storage->setup(overwrite);
    keywordCounters = new Storage(inMemory, dataIndex, "/tmp/keyword-", profile);
    keywordCounters->setup(overwrite);
}

TwoChoiceServer::~TwoChoiceServer() {}


void TwoChoiceServer::storeCiphers(long dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers, map<prf_type, prf_type> keywordCnters) 
{
    storage->insertAll(dataIndex, ciphers);
    keywordCounters->insert(dataIndex, keywordCnters);
}

long TwoChoiceServer::getCounter(long dataIndex, prf_type tokkw) 
{
    prf_type curToken = tokkw;
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());
    bool found = false;
    prf_type res = keywordCounters->find(dataIndex, keywordMapKey, found);
	int keywordCnt = 0;
    if (found) 
    {
        prf_type plaintext;
        Utilities::decode(res, plaintext, curToken.data());
        keywordCnt = *(long*) (&(plaintext[0]));
	}
	return keywordCnt;
}

vector<prf_type> TwoChoiceServer::search(long dataIndex, prf_type hashtoken, long keywordCnt, long max) 
{
    keywordCounters->seekgCount = 0;
    storage->readBytes = 0;
    double keywordCounterTime = 0, serverSearchTime = 0;
    if (profile) 
        Utilities::startTimer(35);
    
    if (profile) 
    {
        keywordCounterTime = Utilities::stopTimer(35);
        printf("keyword counter Search Time:%f number of SeekG:%d number of read bytes:%d\n", keywordCounterTime, keywordCounters->seekgCount, keywordCounters->KEY_VALUE_SIZE * keywordCounters->seekgCount);
        Utilities::startTimer(45);
    }
    vector<prf_type> result;
	result.resize(0);
	if(keywordCnt>max)
	{
		cout <<"search in one choice instance, NOT HERE"<<endl;
		return result;
	}
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type keywordMapKey = Utilities::generatePRF(cntstr, hashtoken.data());
    result = storage->find(dataIndex, keywordMapKey, keywordCnt);
    if (profile){
        serverSearchTime = Utilities::stopTimer(45);
        printf("server Search Time:%f number of SeekG:%d number of read bytes:%d\n", serverSearchTime, storage->SeekG, storage->readBytes);
    }
    return result;
}


vector<pair<prf_type, prf_type> > TwoChoiceServer::getAllData(long dataIndex) 
{
    return storage->getAllData(dataIndex);
}

vector<prf_type> TwoChoiceServer::getStash(long dataIndex) 
{
    return storage->getStash(dataIndex);
}

void TwoChoiceServer::clear(long index) 
{
    storage->clear(index);
    keywordCounters->clear(index);
}
