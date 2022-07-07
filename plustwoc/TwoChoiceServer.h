#ifndef TWOCHOICESERVER_H
#define TWOCHOICESERVER_H

#include "TwoChoiceStorage.h"
#include "Storage.h"

class TwoChoiceServer {
private:
    TwoChoiceStorage* storage;
    Storage* keyworkCounters;
    void getAESRandomValue(unsigned char* keyword, int cnt, unsigned char* result);
    int numberOfBins, sizeOfEachBin;
    bool profile = false;

public:
    TwoChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile);
    void clear(int index);
    virtual ~TwoChoiceServer();
    void storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers,vector<pair<pair<prf_type,prf_type>,pair<prf_type,vector<prf_type>>>> cuckooCiphers, map<prf_type, prf_type> keywordCounters);
    vector<prf_type> search(int dataIndex, prf_type tokkw, prf_type token, int & keywordCnt, int bin);
    vector<pair<prf_type, prf_type> > getAllData(int dataIndex);
    vector<prf_type> getCuckooHT(int dataIndex);
    vector<prf_type> getStash(int dataIndex);
    pair<prf_type, vector<prf_type>> insertCuckooHT(int index, int tableNum, int hash, int cuckooID, prf_type keyw, vector<prf_type> fileids);
    vector<prf_type> cuckooSearch(int index, int tableNum, prf_type hashtoken1, prf_type hashtoken2);
};

#endif /* TWOCHOICESERVER_H */

