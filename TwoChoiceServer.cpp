#include "TwoChoiceServer.h"
#include <string.h>

TwoChoiceServer::TwoChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
    storage = new TwoChoiceStorage(inMemory, dataIndex, "/tmp/", profile);
    storage->setup(overwrite);
    //stashStorage = new TwoChoiceStorage(inMemory, dataIndex, "/tmp/stash/", profile);
    //stashStorage->setup(overwrite);
    keyworkCounters = new Storage(inMemory, dataIndex, "/tmp/keyword-", profile);
    keyworkCounters->setup(overwrite);
    stashCounters = new Storage(inMemory, dataIndex, "/tmp/stashkeyword-", profile);
    stashCounters->setup(overwrite);
}

TwoChoiceServer::~TwoChoiceServer() {}

void TwoChoiceServer::storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers, vector<prf_type> stashCiphers,map<prf_type, prf_type> keywordCounters, map<prf_type, prf_type> stashCounter) 
{
    storage->insertAll(dataIndex, ciphers);
    keyworkCounters->insert(dataIndex, keywordCounters);
    stashCounters->insert(dataIndex,stashCounter);
    storage->insertStash(dataIndex,stashCiphers);
}

vector<prf_type> TwoChoiceServer::stashSearch(int dataIndex, prf_type tokkw, int& keywordCnt) 
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
    bool founds = false;
    prf_type ress = stashCounters->find(dataIndex, keywordMapKey, founds);
   // if (profile) 
   // {
   //     keywordCounterTime = Utilities::stopTimer(35);
   //     printf("keyword counter Search Time:%f number of SeekG:%d number of read bytes:%d\n", keywordCounterTime, keyworkCounters->seekgCount, keyworkCounters->KEY_VALUE_SIZE * keyworkCounters->seekgCount);
   //     Utilities::startTimer(45);
   // }
    vector<prf_type> result;
    if(founds)
    {
    	curToken = tokkw;
        prf_type plaintext;
        Utilities::decode(ress, plaintext, curToken.data());
        int stashkeywordCnt = *(int*) (&(plaintext[0]));
	if(stashkeywordCnt>0) // will be called twice if pos1 == pos2
	{
	    cout <<"SEARCHING IN STASH...!!!!!:"<<stashkeywordCnt<<endl;
	    vector<prf_type> stashResult = storage->getStash(dataIndex);
	    for(auto s : stashResult)
	    {
		    result.push_back(s);
	    }
	}
    }
    return result;
}
vector<prf_type> TwoChoiceServer::search(int dataIndex, prf_type tokkw, prf_type hashtoken, int& keywordCnt) 
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



vector<prf_type> TwoChoiceServer::newsearch(int dataIndex , prf_type hashtoken,int keywordCnt, int pos) 
{
    keyworkCounters->seekgCount = 0;
    storage->readBytes = 0;
    vector<prf_type> result;
    
   // prf_type curToken = tokkw;
   // unsigned char cntstr[AES_KEY_SIZE];
   // memset(cntstr, 0, AES_KEY_SIZE);
   // *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
   // prf_type keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());

    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type hashKey = Utilities::generatePRF(cntstr, hashtoken.data());
    result = storage->newfind(dataIndex, hashKey, keywordCnt,pos);
    return result;
}



vector<pair<prf_type, prf_type> > TwoChoiceServer::getAllData(int dataIndex) 
{
    return storage->getAllData(dataIndex);
}

void TwoChoiceServer::clear(int index) 
{
    storage->clear(index);
    keyworkCounters->clear(index);
    stashCounters->clear(index);

}
