#include "OneChoiceServer.h"
#include <string.h>

OneChoiceServer::OneChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile) {
    this->profile = profile;
    storage = new OneChoiceStorage(inMemory, dataIndex, "/tmp/", profile);
    storage->setup(overwrite);
	cout <<"here:"<<endl;
}

OneChoiceServer::~OneChoiceServer() {
}

void OneChoiceServer::storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers) 
{
    storage->insertAll(dataIndex, ciphers);
}

vector<prf_type> OneChoiceServer::search(int dataIndex, prf_type token, int keywordCnt) 
{
    storage->readBytes = 0;
    double keywordCounterTime = 0, serverSearchTime = 0;
    if (profile) {
        Utilities::startTimer(35);
    }
    prf_type curToken = token;
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
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

vector<pair<prf_type, prf_type> > OneChoiceServer::getAllData(int dataIndex) 
{
    return storage->getAllData(dataIndex);
}

void OneChoiceServer::clear(int index) 
{
    storage->clear(index);
}
