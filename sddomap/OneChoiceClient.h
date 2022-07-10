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


class OneChoiceClient 
{
private:
    OneChoiceServer* server;
    bool profile = false;
    vector<OMAP*> omaps;

public:
    virtual ~OneChoiceClient();
    OneChoiceClient(int maxUpdate, vector<OMAP*> omaps, bool inMemory, bool overwrite, bool profile);
    int totalCommunication = 0;
    vector<int> numberOfBins;
    vector<int> sizeOfEachBin;
	prf_type nullKey;

    vector<vector<bool>> exist;
    void destry(int index);
    void setup(int index, unordered_map<string, vector<prf_type> >pairs, unsigned char* key);
    vector<prf_type> search(int index, string keyword, unsigned char* key);
    vector<prf_type> getAllData(int index, unsigned char* key);
	void copy(int index, int toInstance, int fromInstance);
	void append(int instance, prf_type keyVal);
	void destroy(int index, int instance);
	void resize(int index, int size);
	void getBin(int index, int instance, int start, int end, int updateCounter, unsigned char* key);
	void addDummy(int index, int count, int updateCounter);
	void bitonicSort(int index);
    int map(string w, int cnt, int index, unsigned char* key);
    Bid getBid(string str, int cnt);
};

#endif /* ONECHOICECLIENT_H */

