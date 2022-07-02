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
    void storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers, map<prf_type, prf_type> keywordCounters);
    vector<prf_type> search(int dataIndex, prf_type tokkw, prf_type token, int & keywordCnt);
    vector<pair<prf_type, prf_type> > getAllData(int dataIndex);

};

#endif /* TWOCHOICESERVER_H */

