#ifndef TWOCHOICESTORAGE_H
#define TWOCHOICESTORAGE_H

#include <string>
#include <map>
#include <vector>
#include <array>
#include "Server.h"
#include <iostream>
#include <sstream>
#include "Server.h"
#include "Utilities.h"
#include "Types.hpp"
#include "AES.hpp"
#include <unordered_map>
#include <iostream>
#include <fstream>
//#include <stxxl/vector>
//#include <stxxl/unordered_map>

using namespace std;

class TwoChoiceStorage {
private:
    bool inMemoryStorage;
    bool profile = false;
    vector<string> filenames;
    vector<string> stashes;
    vector<vector<string>> stashfilenames;
    vector<vector<vector<string>>> cuckoofilenames;
    prf_type nullKey;
    string fileAddressPrefix = "/tmp/";
    int dataIndex;
    vector<int> numberOfBins;
    vector<int> sizeOfEachBin;
    int KEY_VALUE_SIZE = (2 * AES_KEY_SIZE);
    vector< vector<pair<prf_type, prf_type> > > data;
    //stxxl::VECTOR_GENERATOR< pair<prf_type, prf_type>, 4, 8, 1 * 1024 * 1024, stxxl::RC, stxxl::lru >::result** diskData;

public:
    int readBytes = 0;
    int SeekG = 0;
    TwoChoiceStorage(bool inMemory, int dataIndex, string fileAddressPrefix, bool profile);
    bool setup(bool overwrite);
    void insertAll(int dataIndex, vector<vector< pair<prf_type, prf_type> > > ciphers);
    void insertStash(int dataIndex, vector<prf_type> ciphers);
    pair<prf_type, vector<prf_type>> insertCuckooHT(int index, int tableNum, int cuckooID, int hash, prf_type keyw, vector<prf_type> fileids);
    vector<pair<prf_type, prf_type> > getAllData(int dataIndex);
    vector<prf_type> getStash(int index); 
    void insertCuckooStash(int index, int tableNum, vector<prf_type> ctCiphers);
    vector<prf_type> getCuckooHT(int index); 
    vector <prf_type> cuckooSearch(int index, int tableNum, int h[2]);
    void clear(int index);
    vector<prf_type> find(int index, prf_type mapKey, int cnt);
	void printStashSizes();
    virtual ~TwoChoiceStorage();
};

#endif /* TWOCHOICESTORAGE_H */
