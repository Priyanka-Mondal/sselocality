#ifndef TWOCHOICESERVER_H
#define TWOCHOICESERVER_H

#include "TwoChoiceStorage.h"
#include "Storage.h"

class TwoChoiceServer {
private:
    TwoChoiceStorage* storage;
    Storage* keywordCounters;
    void getAESRandomValue(unsigned char* keyword, long cnt, unsigned char* result);
    long numberOfBins, sizeOfEachBin;
    bool profile = false;

public:
    TwoChoiceServer(long dataIndex, bool inMemory, bool overwrite, bool profile);
    void clear(long index);
    virtual ~TwoChoiceServer();
    void storeCiphers(long dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers, map<prf_type, prf_type> keywordCounters);
	long getCounter(long dataIndex, prf_type tokkw);
    vector<prf_type> search(long dataIndex,  prf_type token, long keywordCnt, long num);
    vector<pair<prf_type, prf_type> > getAllData(long dataIndex);
    vector<prf_type> getStash(long dataIndex);
    //vector<prf_type> newsearch(long dataIndex , prf_type hashtoken, long keywordCnt, long pos);
	void printStashSizes();

};

#endif /* TWOCHOICESERVER_H */

