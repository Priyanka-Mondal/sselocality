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
#include "AmortizedBASClient.h"
//#include "OneChoiceClient.h"
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
    inline prf_type bitwiseXOR(int input1, int op, prf_type input2);
    inline prf_type bitwiseXOR(prf_type input1, prf_type input2);
    vector<unsigned char*> keys;
    //        AmortizedBASClient* L;
    //OneChoiceClient* L;
    TwoChoiceClient* L;
    int updateCounter = 0;
    double totalUpdateCommSize;
    double totalSearchCommSize;
    vector< unordered_map< string, vector<prf_type > > > data;
    int localSize = 0;
    bool profile = false;

public:
    Amortized(int N, bool inMemory, bool overwrite);
    void update(OP op, string keyword, int ind, bool setup);
    vector<int> search(string keyword);
    virtual ~Amortized();
    double getTotalSearchCommSize() const;
    double getTotalUpdateCommSize() const;
    void endSetup(int N);

};

#endif /* BAS_H */

