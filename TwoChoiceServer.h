#ifndef TWOCHOICESERVER_H
#define TWOCHOICESERVER_H

#include "TwoChoiceStorage.h"
#include "Storage.h"

class TwoChoiceServer {
private:
    TwoChoiceStorage* storage;
    TwoChoiceStorage* stashStorage;
    Storage* keyworkCounters;
    Storage* stashCounters;
    void getAESRandomValue(unsigned char* keyword, int cnt, unsigned char* result);
    int numberOfBins, sizeOfEachBin;
    bool profile = false;

public:
    TwoChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile);
    void clear(int index);
    virtual ~TwoChoiceServer();
    void storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers,vector<prf_type> stashCiphers, map<prf_type, prf_type> keywordCounters, map<prf_type, prf_type> stashCounters);
    vector<prf_type> search(int dataIndex, prf_type tokkw, prf_type token, int & keywordCnt);
    vector<prf_type> stashSearch(int dataIndex, prf_type tokkw, int & keywordCnt);
    vector<pair<prf_type, prf_type> > getAllData(int dataIndex);
    vector<prf_type> newsearch(int dataIndex , prf_type hashtoken, int keywordCnt, int pos);

};

#endif /* TWOCHOICESERVER_H */

