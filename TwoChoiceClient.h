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
    vector<int> sizeOfEachBin;
    map<int,map<string,vector<prf_type>>> stash;
    map<int,map<string,pair<int,int>>> counters;
    vector<bool> exist;
    void destry(int index);
    void setup(int index, map<string, vector<prf_type> >pairs, unsigned char* key);
    vector<prf_type> search(int index, string keyword, unsigned char* key);
    vector<prf_type> getAllData(int index, unsigned char* key);
    void trancateToMpl(int pss, int mpl, int index, string keyword, vector<prf_type> fileids);
};

#endif /* TWOCHOICECLIENT_H */

