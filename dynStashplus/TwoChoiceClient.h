#ifndef TWOCHOICECLIENT_H
#define TWOCHOICECLIENT_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <array>
#include "Server.h"
#include <iostream>
#include <sstream>
#include<assert.h>
#include "Server.h"
#include "Utilities.h"
#include "AES.hpp"
#include "TwoChoiceServer.h"
#include <unordered_map>

class TwoChoiceClient {
private:
    TwoChoiceServer* server;
    bool profile = false;

public:
    virtual ~TwoChoiceClient();
    TwoChoiceClient(long maxUpdate, bool inMemory, bool overwrite, bool profile);
    long totalCommunication = 0;
    vector<long> numberOfBins;
    prf_type nullKey;
    vector<long> sizeOfEachBin;
    map<long,long> position;
    vector<bool> exist;
    vector<bool> existone;
    void destry(long index);
    void setup(long index, map<string, vector<prf_type> >pairs, unsigned char* key);
    vector<prf_type> search(long index, string keyword, unsigned char* key);
    vector<prf_type> newsearch(long index, string keyword, unsigned char* key);
    vector<prf_type> getAllData(long index, unsigned char* key);
    void truncateToMpl(long pss, long mpl, long index, string keyword, vector<prf_type> fileids, unsigned char* key);
    void writeToCuckooHT(long index, long mpl, string keyword, vector<prf_type> fileids, unsigned char* key);
    void writeToCuckooStash(vector<prf_type> fileids,long cnt,long index, long tableNum, unsigned char* key);
void place(string keyw,vector<prf_type> fileids,long cuckooID,long cnt,long index, long tableNum, unsigned char* key);
void writeToStash(long pss,long mpl,vector<prf_type> fileids,unsigned char* key,vector<prf_type> &stashCiphers);
long maxPossibleLen(long index);
void printStashSizes();
};

#endif /* TWOCHOICECLIENT_H */

