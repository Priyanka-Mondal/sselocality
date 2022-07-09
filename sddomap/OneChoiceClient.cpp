#include "OneChoiceClient.h"

OneChoiceClient::~OneChoiceClient() {
    delete server;
}

OneChoiceClient::OneChoiceClient(int numOfDataSets, bool inMemory, bool overwrite, bool profile) {
    this->profile = profile;
    server = new OneChoiceServer(numOfDataSets, inMemory, overwrite, profile);
    for (int i = 0; i < numOfDataSets; i++) {
        exist.push_back(false);
        //        int curNumberOfBins = total > 1 ? (int) ceil((float) total / (float) i) : total;
        int curNumberOfBins = i > 0 ? (int) ceil((float) pow(2, i + 1) / (float) (log2(pow(2, i + 1)) * log2(log2(pow(2, i + 1))))) : 1;
        //        int curSizeOfEachBin = i > 0 ? i : 1;
        int curSizeOfEachBin = i > 0 ? (log2(pow(2, i + 1)) * log2(log2(pow(2, i + 1))))*3 : 1;
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
    }
}

void OneChoiceClient::setup(int index, unordered_map<string, vector<prf_type> > pairs, unsigned char* key) {
    exist[index] = true;
    vector<vector<pair<prf_type, prf_type> > > ciphers;
    for (int i = 0; i < numberOfBins[index]; i++) {
        ciphers.push_back(vector<pair<prf_type, prf_type> >());
    }
    map<prf_type, prf_type> keywprdCntCiphers;
    for (auto pair : pairs) {
        prf_type K1 = Utilities::encode(pair.first, key);
        prf_type mapKey, mapValue;
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        mapKey = Utilities::generatePRF(cntstr, K1.data());
        prf_type valueTmp;
        *(int*) (&(valueTmp[0])) = pair.second.size();
        mapValue = Utilities::encode(valueTmp.data(), K1.data());
        keywprdCntCiphers[mapKey] = mapValue;

        unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
        int pos = (unsigned int) (*((int*) hash)) % numberOfBins[index];
        int cipherIndex = pos;
        for (unsigned int i = 0; i < pair.second.size(); i++) {
            prf_type mapKey, mapValue;
            unsigned char cntstr[AES_KEY_SIZE];
            memset(cntstr, 0, AES_KEY_SIZE);
            *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
            mapKey = Utilities::generatePRF(cntstr, K1.data());
            mapValue = Utilities::encode(pair.second[i].data(), key);
            auto p = std::pair<prf_type, prf_type>(mapKey, mapValue);
            ciphers[cipherIndex].push_back(p);
            cipherIndex++;
            if (cipherIndex == numberOfBins[index]) {
                cipherIndex = 0;
            }
        }
    }
    prf_type dummy;
    memset(dummy.data(), 0, AES_KEY_SIZE);
    auto dummypair = pair<prf_type, prf_type>(dummy, dummy);
    for (int i = 0; i < numberOfBins[index]; i++) {
        int curSize = ciphers[i].size();
        for (int j = curSize; j < sizeOfEachBin[index]; j++) {
            ciphers[i].push_back(dummypair);
        }
    }

    prf_type randomKey;
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        randomKey[i] = rand();
    }
    for (int i = keywprdCntCiphers.size(); i < pow(2, index); i++) {
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 9])) = rand();
        prf_type mapKey = Utilities::generatePRF(cntstr, randomKey.data());
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = rand();
        prf_type mapValue = Utilities::generatePRF(cntstr, randomKey.data());
        keywprdCntCiphers[mapKey] = mapValue;
    }
    //    totalCommunication += ciphers.size() * sizeof (prf_type)*2;
    server->storeCiphers(index, ciphers, keywprdCntCiphers);
}

vector<prf_type> OneChoiceClient::search(int index, string keyword, unsigned char* key) {
    double searchPreparation = 0, searchDecryption = 0;
    if (profile) {
        Utilities::startTimer(65);
    }
    vector<prf_type> finalRes;
    prf_type token = Utilities::encode(keyword, key);
    int keywordCnt = 0;
    vector<prf_type> ciphers = server->search(index, token, keywordCnt);
    int cnt = 0;
    if (profile) {
        searchPreparation = Utilities::stopTimer(65);
        printf("search preparation time:%f include server time\n", searchPreparation);
        Utilities::startTimer(65);
    }

    //    for (int i = 0; i < ciphers.size(); i++) {
    //        prf_type curToken = token, mapKey;
    //        unsigned char cntstr[AES_KEY_SIZE];
    //        memset(cntstr, 0, AES_KEY_SIZE);
    //        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = cnt;
    //        mapKey = Utilities::generatePRF(cntstr, curToken.data());
    //        existkeys[mapKey] = true;
    //        cnt++;
    //    }

    for (auto item : ciphers) {
        prf_type plaintext;
        Utilities::decode(item, plaintext, key);
        //        if (string((char*) plaintext.data()) == keyword) {
        if (strcmp((char*) plaintext.data(), keyword.data()) == 0) {
            finalRes.push_back(plaintext);
        }
    }

    if (profile) {
        searchDecryption = Utilities::stopTimer(65);
        printf("search decryption time:%f for decrypting:%d ciphers\n", searchDecryption, ciphers.size());
    }
    totalCommunication += ciphers.size() * sizeof (prf_type) + sizeof (prf_type);
    return finalRes;
}

vector<prf_type> OneChoiceClient::getAllData(int index, unsigned char* key) {
    vector<prf_type> finalRes;
    auto ciphers = server->getAllData(index);
    for (auto cipher : ciphers) {
        prf_type plaintext;
        Utilities::decode(cipher.second, plaintext, key);
        finalRes.push_back(plaintext);
    }
    totalCommunication += ciphers.size() * sizeof (prf_type);
    return finalRes;
}

void OneChoiceClient::destry(int index) {
    server->clear(index);
    exist[index] = false;
    totalCommunication += sizeof (int);
}
