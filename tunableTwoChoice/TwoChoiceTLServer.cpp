#include "TwoChoiceTLServer.h"
#include <string.h>

TwoChoiceTLServer::TwoChoiceTLServer(long dataIndex, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
    storage = new TwoChoiceTLStorage(inMemory, dataIndex, "/tmp/", profile);
    storage->setup(overwrite);
    keywordCounters = new Storage(inMemory, dataIndex, "/tmp/keyword-", profile);
    keywordCounters->setup(overwrite);
}

TwoChoiceTLServer::~TwoChoiceTLServer() {}


void TwoChoiceTLServer::storeCiphers(long dataIndex, vector<vector<prf_type> > ciphers, map<prf_type, prf_type> keywordCnters) 
{
    storage->insertAll(dataIndex, ciphers);
    keywordCounters->insert(dataIndex, keywordCnters);
}

void TwoChoiceTLServer::storeKeywordCounters(long dataIndex, map<prf_type, prf_type> kwCounters){
    keywordCounters->insert(dataIndex, kwCounters);
}
void TwoChoiceTLServer::storeCiphers(long dataIndex, vector<vector<prf_type> > ciphers, bool firstRun) {
    storage->insertAll(dataIndex, ciphers, true, firstRun);
}
vector<prf_type> TwoChoiceTLServer::search(long dataIndex, prf_type tokkw, prf_type hashtoken, long& keywordCnt, long num) 
{
    keywordCounters->seekgCount = 0;
    storage->readBytes = 0;
    double keywordCounterTime = 0, serverSearchTime = 0;
    if (profile) 
        Utilities::startTimer(35);
    
    prf_type curToken = tokkw;
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());
    bool found = false;
    prf_type res = keywordCounters->find(dataIndex, keywordMapKey, found);
    if (profile) 
    {
        keywordCounterTime = Utilities::stopTimer(35);
        printf("keyword counter Search Time:%f number of SeekG:%d number of read bytes:%d\n", keywordCounterTime, keywordCounters->seekgCount, keywordCounters->KEY_VALUE_SIZE * keywordCounters->seekgCount);
        Utilities::startTimer(45);
    }
    vector<prf_type> result;
	result.resize(0);
    if (found) 
    {
        prf_type plaintext;
        Utilities::decode(res, plaintext, curToken.data());
        keywordCnt = *(long*) (&(plaintext[0]));
		long kwc = keywordCnt;
		kwc = pow(2, (long)ceil(log2(kwc)));
		if(kwc>num)
			kwc = num;
        curToken = hashtoken;
        memset(cntstr, 0, AES_KEY_SIZE);
        *(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());
        result = storage->find(dataIndex, keywordMapKey, kwc);
        if (profile) 
	{
            serverSearchTime = Utilities::stopTimer(45);
            printf("server Search Time:%f number of SeekG:%d number of read bytes:%d\n", serverSearchTime, storage->SeekG, storage->readBytes);
        }
    }
    return result;
}

vector<prf_type> TwoChoiceTLServer::searchLoc(long dataIndex, prf_type hashToken, long kwc) 
{
    vector<prf_type> result;
	result.resize(0);
	unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type keywordMapKey = Utilities::generatePRF(cntstr, hashToken.data());
    result = storage->find(dataIndex, keywordMapKey, kwc);
    return result;
}

long TwoChoiceTLServer::getCounter(long dataIndex, prf_type tokkw) 
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

vector<prf_type> TwoChoiceTLServer::getAllData(long dataIndex) 
{
    return storage->getAllData(dataIndex);
}

vector<prf_type> TwoChoiceTLServer::getStash(long dataIndex) 
{
//    return storage->getStash(dataIndex);
}

void TwoChoiceTLServer::clear(long index) 
{
    storage->clear(index);
    keywordCounters->clear(index);
}
