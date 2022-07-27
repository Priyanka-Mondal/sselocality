#ifndef ONECHOICESERVER_H
#define ONECHOICESERVER_H

#include "OneChoiceStorage.h"
#include "Storage.h"
#include "OMAP.h"

class OneChoiceServer {
private:
    OneChoiceStorage* storage;
    Storage* keywordCounters;
    void getAESRandomValue(unsigned char* keyword, int cnt, unsigned char* result);
    int numberOfBins, sizeOfEachBin, dataIndex;
    bool profile = false;

public:
    OneChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile);
    virtual ~OneChoiceServer();
    void destroy(int index, int instance);
	void storeKwCounters(int dataIndex, int instance, map<prf_type, prf_type> keywordCounters);
	int getCounter(int dataIndex, int instance, prf_type tokkw); 
    vector<prf_type> getAllData(int dataIndex, int instance);
	prf_type findCounter(int dataIndex, int instance, prf_type token);
	void move(int index, int toInstance, int fromInstance, int size);
	vector<prf_type> getElements(int index, int instance, int start, int end);
	vector<prf_type> getNEW(int index);
	vector<prf_type> getNEW(int index, int count, int size, bool NEW);
	void putNEW(int index, int instance, vector<prf_type>);
	int writeToNEW(int index, prf_type keyVal, int pos);
	int writeToKW(int index, prf_type keyVal, int pos);
	void resize(int index, int size, int filesize);
    vector<prf_type> search(int dataIndex, int instance, prf_type token, int keywordCnt);
    //void storeCiphers(int dataIndex, int instance, vector<vector<pair<prf_type, prf_type> > > ciphers, map<prf_type, prf_type> keywordCounters);
};

#endif /* ONECHOICESERVER_H */

