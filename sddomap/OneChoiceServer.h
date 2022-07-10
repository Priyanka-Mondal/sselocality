#ifndef ONECHOICESERVER_H
#define ONECHOICESERVER_H

#include "OneChoiceStorage.h"
#include "Storage.h"
#include "OMAP.h"

class OneChoiceServer {
private:
    OneChoiceStorage* storage;
    Storage* keyworkCounters;
    void getAESRandomValue(unsigned char* keyword, int cnt, unsigned char* result);
    int numberOfBins, sizeOfEachBin;
    bool profile = false;
	vector<vector<prf_type>> NEW;

public:
    OneChoiceServer(int dataIndex, vector<OMAP*> omaps, bool inMemory, bool overwrite, bool profile);
    void clear(int index, int instance);
    virtual ~OneChoiceServer();
    void storeCiphers(int dataIndex, vector<vector<pair<prf_type, prf_type> > > ciphers, map<prf_type, prf_type> keywordCounters);
    vector<prf_type> search(int dataIndex, prf_type token, int & keywordCnt);
    vector<pair<prf_type, prf_type> > getAllData(int dataIndex);
	void copy(int index, int toInstance, int fromInstance);
	void append(int instance, prf_type keyVal);
	void destroy(int index, int instance);
	void resize(int index, int size);
	vector<prf_type> getElements(int index, int instance, int start, int end);
	void addDummy(int index, int count, int updateCounter);
	void bitonicSort(int index);
};

#endif /* ONECHOICESERVER_H */

