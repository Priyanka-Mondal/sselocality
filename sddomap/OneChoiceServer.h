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
    int numberOfBins, sizeOfEachBin, dataIndex;
    bool profile = false;
	vector<vector<prf_type>> NEW;

public:
    OneChoiceServer(int dataIndex, bool inMemory, bool overwrite, bool profile);
    void clear(int index, int instance);
    virtual ~OneChoiceServer();
    void storeCiphers(int dataIndex, int instance, vector<vector<pair<prf_type, prf_type> > > ciphers, map<prf_type, prf_type> keywordCounters);
    //vector<prf_type> search(int dataIndex, int instance, prf_type token, int & keywordCnt);
    vector<prf_type> search(int dataIndex, int instance, int bin);
    vector<prf_type> getAllData(int dataIndex, int instance);
	void move(int index, int toInstance, int fromInstance, int size);
	void copy(int index, int toInstance);
	void append(int instance, prf_type keyVal);
	void destroy(int index, int instance);
	void resize(int index, int size);
	vector<prf_type> getElements(int index, int instance, int start, int end);
	void addDummy(int index, int count, int updateCounter);
	vector<prf_type> getNEW(int index);
	vector<prf_type> getNEW(int index, int size);
	void putNEW(int index, vector<prf_type>);
	void bitonicSort(int index);
    int getNEWsize(int index);
	int writeToNEW(int index, prf_type keyVal, int pos);
	void moveNEW(int index, int toInstance, int size);
	void truncate(int index, int size, int filesize);
};

#endif /* ONECHOICESERVER_H */

