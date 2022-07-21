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

class OneChoiceClient 
{
private:
    OneChoiceServer* server;
    bool profile = false;

public:
    virtual ~OneChoiceClient();
    OneChoiceClient(long maxUpdate, bool inMemory, bool overwrite, bool profile);
    long totalCommunication = 0;
    vector<long> numberOfBins;
    vector<long> sizeOfEachBin;

    vector<bool> exist;
    void destry(long index);
    void setup(long index, unordered_map<string, vector<prf_type> >pairs, unsigned char* key);
    vector<prf_type> search(long index, string keyword, unsigned char* key);
    vector<prf_type> getAllData(long index, unsigned char* key);
};

#endif /* ONECHOICECLIENT_H */

