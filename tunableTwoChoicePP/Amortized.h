#ifndef AMORTIZE_H
#define AMORTIZE_H

#include <string>
#include <map>
#include <vector>
#include <array>
#include "Server.h"
#include <iostream>
#include <sstream>
#include "Utilities.h"
#include "TwoChoicePPWithTunableLocalityClient.h"
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
    //        AmortizedAMORTIZEClient* L;
    //OneChoiceClient* L;
    TwoChoicePPWithTunableLocalityClient* L;
    long updateCounter = 0;
    double totalUpdateCommSize;
    double totalSearchCommSize;
    vector< unordered_map< string, vector<prf_type > > > data;
    long localSize = 0;
    bool profile = false;

public:
    Amortized(long N, bool inMemory, bool overwrite);
    void update(OP op, string keyword, int ind, bool setup);
    vector<int> search(string keyword);
    virtual ~Amortized();
    double getTotalSearchCommSize() const;
    double getTotalUpdateCommSize() const;
    void endSetup(long N);

};

#endif /* AMORTIZE_H */

