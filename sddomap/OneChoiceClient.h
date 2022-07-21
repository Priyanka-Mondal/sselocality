#ifndef ONECHOICECLIENT_H
#define ONECHOICECLIENT_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <array>
#include "Server.h"
#include <iostream>
#include <sstream>
#include "Server.h"
#include "Utilities.h"
#include "AES.hpp"
#include "OneChoiceServer.h"
#include "OMAP.h"
#include <unordered_map>

enum OP 
{
    INS, DEL
};

class OneChoiceClient 
{
private:
    OneChoiceServer* server;
    bool profile = false;

public:
    vector<OMAP*> omaps;
    virtual ~OneChoiceClient();
    OneChoiceClient(int maxUpdate, bool inMemory, bool overwrite, bool profile);
    int totalCommunication = 0;
    vector<int> numberOfBins;
    vector<int> sizeOfEachBin;
	vector<int> indexSize;
	prf_type nullKey;
	int numOfIndices;
	vector<int> numNEW;
	vector<int> NEWsize;
	vector<int> KWsize;
	int b;

    vector<vector<bool>> exist;
    void destry(int index);
    void setup(int index, unordered_map<string, vector<prf_type> >pairs, unsigned char* key);
    vector<prf_type> search(int index, int instance, string keyword, unsigned char* key);
	vector<prf_type> NIsearch(int index, int instance, string keyword, unsigned char* key);
    vector<prf_type> getAllData(int index, int instance, unsigned char* key);
	void move(int index, int toInstance, int fromInstance);
	void copy(int index, int toInstance);
	void append(int instance, prf_type keyVal, unsigned char* key);
	void appendTokwCounter(int instance, prf_type keyVal, unsigned char* key);
	void destroy(int index, int instance);
	void resize(int index, int size);
	void reSize(int index, int size);
	void getBin(int newindex, int instance, int start, int end, unsigned char* key1, unsigned char* key2);
	void addDummy(int index, int count, unsigned char* key, int s, int r1, int r2);
	void deAmortizedBitSort(int step, int counter, int size, int index, unsigned char* key);
	void nonOblSort(int index, unsigned char* key);
    int hashKey(string w, int cnt, int index, unsigned char* key);
    Bid getBid(string str, int cnt);
	vector<prf_type> searchNEW(int index, string keyword);
	void ensureNEWSize(int index, int bin, int cnt);
    int getNEWsize(int index);
	void pad(int index, int newSize, int r, unsigned char* key);
	void updateCounters(int index, unsigned char* key);
	void updateCounters(int index, unsigned char* key, int count, int r1, int r2);
	void kwCount(int index, unsigned char* key, int count, int r1, int r2);
	void updateOMAP(int index, string keyword, unsigned char* key);
	bool sorted(int index, unsigned char* key);
};

#endif /* ONECHOICECLIENT_H */

