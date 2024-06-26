#include "Amortized.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

using namespace std;

Amortized::Amortized(long N, bool inMemory, bool overwrite) 
{
    L = new TwoChoiceClient(ceil(log2(N)), inMemory, overwrite, profile);
    for (long i = 0; i < ceil(log2(N)); i++) 
        keys.push_back(NULL);
    for (long i = 0; i < localSize; i++) 
        data.push_back(unordered_map<string, vector<prf_type> >());
    if (!overwrite) 
    {
        fstream file("/tmp/existStatus.txt", std::ofstream::in);
        if (file.fail()) 
		{
            file.close();
            return;
        }
        for (unsigned long i = localSize; i < L->exist.size(); i++) 
		{
            string data;
            getline(file, data);
            if (data == "true") 
	    	{
                L->exist[i] = true;
                unsigned char* newKey = new unsigned char[16];
                memset(newKey, 0, 16);
                keys[i] = newKey;
            } else 
	    	{
                L->exist[i] = false;
            }
        }
        file.close();
    }
    
}

Amortized::~Amortized() 
{
    fstream file("/tmp/existStatus.txt", std::ofstream::out);
    if (file.fail()) 
    {
        cerr << "Error: " << strerror(errno);
    }
    for (unsigned long i = localSize; i < L->exist.size(); i++) 
    {
        if (L->exist[i]) 
            file << "true" << endl;
        else 
            file << "false" << endl;
    }
    file.close();
}

void Amortized::update(OP op, string keyword, long ind, bool setup) 
{
    totalUpdateCommSize = 0;
    L->totalCommunication = 0;
    long rm0 = log2((~updateCounter & (updateCounter + 1)));
    updateCounter++;
    map<string, vector<prf_type> > previousData;

    for (long i = 0; i < min(rm0, localSize); i++) 
    {
        for (auto item : data[i]) 
	{
            if (previousData.count(item.first) == 0) 
	    {
                previousData[item.first] = vector<prf_type>();
            }
            previousData[item.first].insert(previousData[item.first].end(), item.second.begin(), item.second.end());
        }
        data[i].clear();
    }
    
    for (long i = localSize; i < rm0; i++) 
    {
        vector<prf_type> curData = L->getAllData(i, keys[i]);
        for (auto item : curData) 
	{
            string curKeyword((char*) item.data());
	    if (curKeyword == "") cout<<"[[curKeyword NULL]]"<<endl;
            if (previousData.count(curKeyword) == 0) 
	    {
                previousData[curKeyword] = vector < prf_type>();
            }
            previousData[curKeyword].push_back(item);
        }

        L->destry(i);
	//cout <<"L->destroy("<<i<<") was called"<<endl;
        delete keys[i];
        keys[i] = NULL;
    }
    prf_type value;
    std::fill(value.begin(), value.end(), 0);
    std::copy(keyword.begin(), keyword.end(), value.begin());
    *(long*) (&(value.data()[AES_KEY_SIZE - 5])) = ind;
    value.data()[AES_KEY_SIZE - 6] = (byte) (op == OP::INS ? 0 : 1);

    if (previousData.count(keyword) == 0) 
    {
        previousData[keyword] = vector<prf_type>();
    }
    previousData[keyword].push_back(value);
    if (rm0 < localSize) 
    {
        data[rm0].insert(previousData.begin(), previousData.end());
    } 
    else 
    {
        unsigned char* newKey = new unsigned char[16];
        memset(newKey, 0, 16);
        keys[rm0] = newKey;
        L->setup(rm0, previousData, newKey);
        totalUpdateCommSize += L->totalCommunication;
    }
}

vector<long> Amortized::search(string keyword) 
{
    totalSearchCommSize = 0;
    if (profile) 
    {
        Utilities::startTimer(33);
    }
    L->totalCommunication = 0;
    vector<long> finalRes;
    vector<prf_type> encIndexes;
    long s = data.size();
    for (long i = 0; i < min(localSize, s); i++) 
    {
        if (data[i][keyword].size() > 0) 
	{
            encIndexes.insert(encIndexes.end(), data[i][keyword].begin(), data[i][keyword].end());
        }
    }
    for (unsigned long i = localSize; i < L->exist.size(); i++) 
    {
        if (L->exist[i]) 
	{
            if (profile) 
	    {
                printf("level %d:\n", i);
            }
            auto tmpRes = L->search(i, keyword, keys[i]);
            encIndexes.insert(encIndexes.end(), tmpRes.begin(), tmpRes.end());
        }
    }
    double filterationTime = 0;
    double searchTime = 0;
    if (profile) 
    {
        searchTime = Utilities::stopTimer(33);
        printf("Amortized Search time:%f\n", searchTime);
        Utilities::startTimer(99);
    }
    map<long, long> remove;
    for (auto i = encIndexes.begin(); i != encIndexes.end(); i++) 
    {
        prf_type decodedString = *i;
        long plaintext = *(long*) (&(decodedString.data()[AES_KEY_SIZE - 5]));
        remove[plaintext] += (2 * ((byte) decodedString.data()[AES_KEY_SIZE - 6]) - 1);
    }
    for (auto const& cur : remove) 
    {
        if (cur.second < 0) 
	{
            finalRes.emplace_back(cur.first);
        }
    }
    if (profile) 
    {
        filterationTime = Utilities::stopTimer(99);
        printf("filteration time:%f\n", filterationTime);
    }
    totalSearchCommSize += L->totalCommunication;
    return finalRes;
}

prf_type Amortized::bitwiseXOR(long input1, long op, prf_type input2) 
{
    prf_type result;
    result[3] = input2[3] ^ ((input1 >> 24) & 0xFF);
    result[2] = input2[2] ^ ((input1 >> 16) & 0xFF);
    result[1] = input2[1] ^ ((input1 >> 8) & 0xFF);
    result[0] = input2[0] ^ (input1 & 0xFF);
    result[4] = input2[4] ^ (op & 0xFF);
    for (long i = 5; i < AES_KEY_SIZE; i++) 
    {
        result[i] = (rand() % 255) ^ input2[i];
    }
    return result;
}

prf_type Amortized::bitwiseXOR(prf_type input1, prf_type input2) 
{
    prf_type result;
    for (unsigned long i = 0; i < input2.size(); i++) 
    {
        result[i] = input1.at(i) ^ input2[i];
    }
    return result;
}

double Amortized::getTotalSearchCommSize() const 
{
    return totalSearchCommSize;
}

double Amortized::getTotalUpdateCommSize() const 
{
    return totalUpdateCommSize;
}

void Amortized::endSetup() 
{
}
