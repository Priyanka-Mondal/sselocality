#include "OneChoiceServer.h"
#include <string.h>

OneChoiceServer::OneChoiceServer(long dataIndex, bool inMemory, bool overwrite, bool profile) {
    this->profile = profile;
    storage = new OneChoiceStorage(inMemory, dataIndex, "/tmp/", profile);
    storage->setup(overwrite);
}

OneChoiceServer::~OneChoiceServer() {
}

void OneChoiceServer::storeCiphers(long dataIndex, vector<vector<prf_type>> ciphers) 
{
    storage->insertAll(dataIndex, ciphers);
}
void OneChoiceServer::storeCiphers(long dataIndex, vector<vector<prf_type> > ciphers, bool firstRun) {
    storage->insertAll(dataIndex, ciphers, true, firstRun);
}

vector<prf_type> OneChoiceServer::search(long dataIndex, prf_type token, long keywordCnt) 
{
    storage->readBytes = 0;
    double keywordCounterTime = 0, serverSearchTime = 0;
    if (profile) {
        Utilities::startTimer(35);
    }
    prf_type curToken = token;
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());
    if (profile) {
        keywordCounterTime = Utilities::stopTimer(35);
        Utilities::startTimer(45);
    }

    vector<prf_type> result;
    result = storage->find(dataIndex, keywordMapKey, keywordCnt);
    if (profile) {
           serverSearchTime = Utilities::stopTimer(45);
            printf("server Search Time:%f number of SeekG:%d number of read bytes:%d\n", serverSearchTime, storage->SeekG, storage->readBytes);
        }
    return result;
}

vector<prf_type> OneChoiceServer::getAllData(long dataIndex) 
{
    return storage->getAllData(dataIndex);
}

void OneChoiceServer::clear(long index) 
{
    storage->clear(index);
}
