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
#include <unordered_map>

class OneChoiceClient {
private:
    OneChoiceServer* server;
    bool profile = false;

public:
    virtual ~OneChoiceClient();
    OneChoiceClient(int maxUpdate, bool inMemory, bool overwrite, bool profile);
    int totalCommunication = 0;
    vector<int> numberOfBins;
    vector<int> sizeOfEachBin;

    vector<vector<bool>> exist;
    void destry(int index);
    void setup(int index, unordered_map<string, vector<prf_type> >pairs, unsigned char* key);
    vector<prf_type> search(int index, string keyword, unsigned char* key);
    vector<prf_type> getAllData(int index, unsigned char* key);
};

#endif /* ONECHOICECLIENT_H */

