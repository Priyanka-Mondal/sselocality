#ifndef ONECHOICESTORAGE_H
#define ONECHOICESTORAGE_H

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

class OneChoiceStorage {
private:
    bool inMemoryStorage;
    bool profile = false;
    vector<vector<string>> filenames;
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
	int b;
    OneChoiceStorage(bool inMemory, int dataIndex, string fileAddressPrefix, bool profile);
    bool setup(bool overwrite);
    void insertAll(int dataIndex, int instance, vector<prf_type> ciphers);
    vector<prf_type> getAllData(int dataIndex, int instance);
    void clear(int index, int instance);
    vector<prf_type> find(int index, prf_type mapKey, int cnt);
    vector<prf_type> searchBin(int index, int instance, int bin);
    virtual ~OneChoiceStorage();
    vector<prf_type> getElements(int index, int instance, int start, int end);
	void copy(int index, int toInstance, int fromInstance);
	void writeToNEW(int index, prf_type keyVal, int pos);
	void truncate(int index, int size);

};

#endif /* ONECHOICESTORAGE_H */

