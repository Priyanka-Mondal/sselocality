#ifndef TWOCHOICETLCLIENT_H
#define TWOCHOICETLCLIENT_H

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
#include "OneChoiceServer.h"
#include "TwoChoiceTLServer.h"
#include <unordered_map>

class TwoChoiceTLClient {
private:
    TwoChoiceTLServer* server;
	OneChoiceServer* one;
    bool profile = false;

public:
    virtual ~TwoChoiceTLClient();
    TwoChoiceTLClient(long maxUpdate, bool inMemory, bool overwrite, bool profile);
	prf_type nullKey;
    long totalCommunication = 0;
    vector<long> numberOfBins;
    vector<long> sizeOfEachBin;
    map<long,long> position;
    vector<bool> exist;
    vector<bool> stashExist;
	vector<long> nB;
	vector<long> sEB;
    void destry(long index);
    void setup(long index, unordered_map<string, vector<prf_type>>pairs, unsigned char* key);
	void setup2(long index, unordered_map<string, vector<tmp_prf_type> > pairs, unsigned char* key);
	vector<vector<prf_type> > convertTmpCiphersToFinalCipher(vector<pair<std::pair<string, long>, tmp_prf_type> > ciphers, unsigned char* key);
    vector<prf_type> search(long index, string keyword, unsigned char* key);
    vector<prf_type> getAllData(long index, unsigned char* key);
	void printStashSizes();
	long maxPossibleLen(long index);
};

#endif /* TWOCHOICETLCLIENT_H */

