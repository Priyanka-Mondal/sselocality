#ifndef OMAP_H
#define OMAP_H
#include <iostream>
#include "ORAM.hpp"
#include <functional>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "AVLTree.h"
using namespace std;

class OMAP {
private:
    Bid rootKey;
    int rootPos;

public:
    AVLTree* treeHandler;
    OMAP(int maxSize, bytes<Key> key);
    virtual ~OMAP();
    void insert(Bid key, string value);
    string find(Bid key);
    void printTree();
    void batchInsert(map<Bid, string> pairs);
    void setupInsert(map<Bid, string> pairs);
    vector<string> batchSearch(vector<Bid> keys);
    void setDummy(int i);
    string incrementCnt(Bid key);
	string decrementCnt(Bid key);
};

#endif /* OMAP_H */

