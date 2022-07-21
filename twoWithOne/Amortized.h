#ifndef BAS_H
#define BAS_H

#include <string>
#include <map>
#include <vector>
#include <array>
#include "Server.h"
#include <iostream>
#include <sstream>
#include "Server.h"
#include "Utilities.h"
#include "TwoChoiceClient.h"
#include "AES.hpp"
#include <set>
#include <unordered_map>

using namespace std;

enum OP {
    INS, DEL
};

class Amortized {
private:
    inline prf_type bitwiseXOR(long input1, long op, prf_type input2);
    inline prf_type bitwiseXOR(prf_type input1, prf_type input2);
    vector<unsigned char*> keys;
    //        AmortizedBASClient* L;
    //OneChoiceClient* L;
    TwoChoiceClient* L;
    long updateCounter = 0;
    double totalUpdateCommSize;
    double totalSearchCommSize;
    vector< unordered_map< string, vector<prf_type > > > data;
    long localSize = 0;
    bool profile = false;

public:
    Amortized(long N, bool inMemory, bool overwrite);
    void update(OP op, string keyword, long ind, bool setup);
    vector<long> search(string keyword);
    virtual ~Amortized();
    double getTotalSearchCommSize() const;
    double getTotalUpdateCommSize() const;
    void endSetup();

};

#endif /* BAS_H */

