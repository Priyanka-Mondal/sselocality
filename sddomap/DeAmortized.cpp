#include "DeAmortized.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
//#include <sse/crypto/prg.hpp>
#include <vector>
using namespace std;
//using namespace boost::algorithm;

DeAmortized::DeAmortized(int N, bool inMemory, bool overwrite) 
{
	cout <<"=====================Running SDd+OneChoiceAllocation======================"<<endl;
   	this->deleteFiles = deleteFiles;
	//B = 1;
    l = ceil(log2(N));
    b = ceil(log2(B));
	lb = l-b;
    memset(nullKey.data(), 0, AES_KEY_SIZE);
    for (int j = 0; j < 4; j++) 
	{
        keys.push_back(vector<unsigned char*> ());
        for (int i = 0; i < l; i++) 
		{
            unsigned char* tmpKey = new unsigned char[16];
            keys[j].push_back(tmpKey);
        }
    }
    for (int i = 0; i <=lb; i++) 
	{
        cnt.push_back(0);
        bytes<Key> key{0};
        OMAP* omap = new OMAP(max((int) pow(2, i), 4), key);
        omaps.push_back(omap);
        //setupOMAPS.push_back(map<Bid, string>());
        //setupOMAPSDummies.push_back(0);
    }
    for (int i = 0; i < localSize; i++) 
	{
        localmap.push_back(map<string, string>());
    }
    for (int j = 0; j < 4; j++) 
	{
        vector< unordered_map<string, prf_type> > curVec;
        for (int i = 0; i < localSize; i++) 
		{
            curVec.push_back(unordered_map<string, prf_type>());
        }
        data.push_back(curVec);
    }
    L = new OneChoiceClient(N, omaps, inMemory, overwrite, true);
}

DeAmortized::~DeAmortized() {}

void DeAmortized::update(OP op, string keyword, int ind, bool setup) 
{
	//int s = 2; // for now choose wisely and make sure to not overflow 
	int t;
	for(int i=lb; i>0; i--)
	{
		int j = b+i;
		int mi = numberOfBins(j); // for now
		if(L->exist[i][0] && L->exist[i][1])
		{
			if(i>1)
				t = 3;
			else 
				t = 1;
			if(cnt[i] <(ceil(t*pow(2,j)/s)))
			{
				L->getBin(i-1, 0, cnt[i]*(s/2),(cnt[i]+1)*(s/2)-1, updateCounter, keys[i-1][0]);
				L->getBin(i-1, 1, cnt[i]*(s/2),(cnt[i]+1)*(s/2)-1, updateCounter, keys[i-1][1]);
			}
			else if ((ceil(t*pow(2,j)/s)) <= cnt[i] && cnt[i] < ((ceil(t*pow(2,j)/s))+(ceil(mi/s))))
				L->addDummy(i, cnt[i]-ceil((t*pow(2,j)/s)+ceil(mi/s)), updateCounter);
			else if (ceil(t*pow(2,j)+mi) <= cnt[i] < pow(2,j))
				L->bitonicSort(i);
		
			cnt[i]=cnt[i]+1;
			if(cnt[i]= pow(2,j))
			{
				L->resize(i,3*pow(2,j));
				L->copy(i-1,0,2); 
				L->destroy(i-1,1);
				if(!(L->exist[i][0]))
				{
					L->copy(i,0,3);
					updateKey(i,0,3);
				}
				else if(!(L->exist[i][1]))
				{
					L->copy(i,1,3);
					updateKey(i,1,3);
				}
				else
				{
					L->copy(i,2,i);
					updateKey(i,2,3);
				}
    	    	unsigned char* newKey = new unsigned char[16];
    	    	memset(newKey, 0, 16);
    	    	keys[i][3] = newKey;
				cnt[i] = 0;
			}
		}
	}
    prf_type value;
    std::fill(value.begin(), value.end(), 0);
    std::copy(keyword.begin(), keyword.end(), value.begin());//keyword
    *(int*) (&(value.data()[AES_KEY_SIZE - 5])) = ind;//fileid
    value.data()[AES_KEY_SIZE - 6] = (byte) (op == OP::INS ? 0 : 1);//op
	value.data()[AES_KEY_SIZE - 7] = (byte) (0); // bin
	L->append(0, value);

	cnt[0]=cnt[0]+1;
	if(cnt[0]==B)
	{
		if(!(L->exist[0][0]))
		{
			L->copy(0,0,0);
			updateKey(0,0,3);
		}
		else if(!(L->exist[0][1]))
		{
			L->copy(0,1,0);
			updateKey(0,1,3);
		}
		else
		{
			L->copy(0,2,0);
			updateKey(0,2,3);
		}
       	unsigned char* newKey = new unsigned char[16];
       	memset(newKey, 0, 16);
       	keys[0][3] = newKey;
		cnt[0] = 0;
	}
    updateCounter++;
}

int DeAmortized::numberOfBins(int i)
{
    int curNumberOfBins = i > 1 ? 
			(int) ceil((float) pow(2, i)/(float)(log2(pow(2, i))*log2(log2(pow(2, i))))):1;
	return curNumberOfBins;
}
void DeAmortized::updateKey(int index, int toInstance , int fromInstance)
{
	//cout <<"updateKey:"<<index<<" to:"<<toInstance<<" from:"<<fromInstance<<endl;
	keys[index][toInstance] = keys[index][fromInstance];
}
/*
Bid DeAmortized::getBid(string input, int cnt) 
{
    std::array< uint8_t, ID_SIZE> value;
    std::fill(value.begin(), value.end(), 0);
    std::copy(input.begin(), input.end(), value.begin());
    *(int*) (&value[AES_KEY_SIZE - 4]) = cnt;
    Bid res(value);
    return res;
}
*/
prf_type DeAmortized::getElementAt(int instance, int index, int pos) 
{
    auto iter = data[instance][index].begin();
    for (int i = 0; i < pos; i++) {
        iter++;
    }
    return (*iter).second;
}
/*
vector<int> DeAmortized::search(string keyword) {
    for (int i = 0; i < l; i++) {
        omaps[i]->treeHandler->oram->totalRead = 0;
        omaps[i]->treeHandler->oram->totalWrite = 0;
    }
    L->totalCommunication = 0;
    totalSearchCommSize = 0;
    vector<int> finalRes;
    vector<prf_type> encIndexes;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < localSize; i++) {
            if (data[j][i].size() > 0) {
                int curCounter = 1;
                bool exist = true;
                do {
                    if (data[j][i].count(keyword + "-" + to_string(curCounter)) != 0) {
                        encIndexes.push_back(data[j][i][keyword + "-" + to_string(curCounter)]);
                        curCounter++;
                    } else {
                        exist = false;
                    }
                } while (exist);
            }
        }
    }
    for (int j = 0; j < 3; j++) {
        for (int i = localSize; i < l; i++) {
            if (L->exist[j][i]) {
                auto tmpRes = L->search(j, i, keyword, keys[j][i]);
                encIndexes.insert(encIndexes.end(), tmpRes.begin(), tmpRes.end());
            }
        }
    }

    map<int, int> remove;
    for (auto i = encIndexes.begin(); i != encIndexes.end(); i++) {
        prf_type decodedString = *i;
        int plaintext = *(int*) (&(decodedString.data()[AES_KEY_SIZE - 5]));
        remove[plaintext] += (2 * ((byte) decodedString.data()[AES_KEY_SIZE - 6]) - 1);
    }
    for (auto const& cur : remove) {
        if (cur.second < 0) {
            finalRes.emplace_back(cur.first);
        }
    }
    for (int i = 0; i < l; i++) {
        totalSearchCommSize += (omaps[i]->treeHandler->oram->totalRead + omaps[i]->treeHandler->oram->totalWrite)*(sizeof (prf_type) + sizeof (int));
    }
    totalSearchCommSize += L->totalCommunication;
    return finalRes;
}

prf_type DeAmortized::bitwiseXOR(int input1, int op, prf_type input2) {
    prf_type result;
    result[3] = input2[3] ^ ((input1 >> 24) & 0xFF);
    result[2] = input2[2] ^ ((input1 >> 16) & 0xFF);
    result[1] = input2[1] ^ ((input1 >> 8) & 0xFF);
    result[0] = input2[0] ^ (input1 & 0xFF);
    result[4] = input2[4] ^ (op & 0xFF);
    for (int i = 5; i < AES_KEY_SIZE; i++) {
        result[i] = (rand() % 255) ^ input2[i];
    }
    return result;
}

prf_type DeAmortized::bitwiseXOR(prf_type input1, prf_type input2) {
    prf_type result;
    for (unsigned int i = 0; i < input2.size(); i++) {
        result[i] = input1.at(i) ^ input2[i];
    }
    return result;
}

void DeAmortized::getAESRandomValue(unsigned char* keyword, int op, int srcCnt, int fileCnt, unsigned char* result) {
    if (deleteFiles) {
        *(int*) (&keyword[AES_KEY_SIZE - 9]) = srcCnt;
    }
    keyword[AES_KEY_SIZE - 5] = op & 0xFF;
    *(int*) (&keyword[AES_KEY_SIZE - 4]) = fileCnt;
    sse::crypto::Prg::derive((unsigned char*) keyword, 0, AES_KEY_SIZE, result);
}


void DeAmortized::endSetup() {
    for (unsigned int i = 0; i < setupOMAPS.size(); i++) {
        omaps[i]->setDummy(setupOMAPSDummies[i]);
        omaps[i]->setupInsert(setupOMAPS[i]);
    }
}

double DeAmortized::getTotalSearchCommSize() const {
    return totalSearchCommSize;
}

double DeAmortized::getTotalUpdateCommSize() const {
    return totalUpdateCommSize;
}
*/
