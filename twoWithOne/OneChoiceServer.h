#ifndef ONECHOICESERVER_H
#define ONECHOICESERVER_H

#include "OneChoiceStorage.h"

class OneChoiceServer {
private:
    OneChoiceStorage* storage;
    void getAESRandomValue(unsigned char* keyword, int cnt, unsigned char* result);
    int numberOfBins, sizeOfEachBin;
    bool profile = false;

public:
    OneChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile);
    void clear(int index);
    virtual ~OneChoiceServer();
    void storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers);
    vector<prf_type> search(int dataIndex, prf_type token, int  keywordCnt);
    vector<pair<prf_type, prf_type> > getAllData(int dataIndex);

};

#endif /* ONECHOICESERVER_H */

