#ifndef ONECHOICESERVER_H
#define ONECHOICESERVER_H

#include "OneChoiceStorage.h"
//#include "Storage.h"

class OneChoiceServer {
private:
    OneChoiceStorage* storage;
    //Storage* keyworkCounters;
    void getAESRandomValue(unsigned char* keyword, long cnt, unsigned char* result);
    long numberOfBins, sizeOfEachBin;
    bool profile = false;

public:
    OneChoiceServer(long dataIndex, bool inMemory, bool overwrite, bool profile);
    void clear(long index);
    virtual ~OneChoiceServer();
    void storeCiphers(long dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers);
    vector<prf_type> search(long dataIndex, prf_type token, long  keywordCnt);
    vector<pair<prf_type, prf_type> > getAllData(long dataIndex);

};

#endif /* ONECHOICESERVER_H */

