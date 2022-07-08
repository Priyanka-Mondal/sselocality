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
    TwoChoiceClient(int maxUpdate, bool inMemory, bool overwrite, bool profile);
    int totalCommunication = 0;
    vector<int> numberOfBins;
    prf_type nullKey;
    vector<int> sizeOfEachBin;
    map<int,int> position;
    vector<bool> exist;
    void destry(int index);
    void setup(int index, map<string, vector<prf_type> >pairs, unsigned char* key);
    vector<prf_type> search(int index, string keyword, unsigned char* key);
    vector<prf_type> newsearch(int index, string keyword, unsigned char* key);
    vector<prf_type> getAllData(int index, unsigned char* key);
    void truncateToMpl(int pss, int mpl, int index, string keyword, vector<prf_type> fileids, unsigned char* key);
    void writeToCuckooHT(int index, int mpl, string keyword, vector<prf_type> fileids, unsigned char* key);
    void writeToCuckooStash(vector<prf_type> fileids,int cuckooID,int cnt,int index, int tableNum, unsigned char* key);
void place(string keyw,vector<prf_type> fileids,int cuckooID,int cnt,int index, int tableNum, unsigned char* key);
void writeToStash(int pss, int mpl, vector<prf_type> fileids,unsigned char* key,vector<prf_type> &stashCiphers);
};

#endif /* TWOCHOICECLIENT_H */

