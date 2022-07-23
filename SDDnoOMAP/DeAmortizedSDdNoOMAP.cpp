#include "DeAmortized.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <vector>
#include<cstdlib>
#include<algorithm>
using namespace std;

DeAmortized::DeAmortized(int N, bool inMemory, bool overwrite) 
{
	cout <<"=====================Running SDd+OneChoiceAllocation======================"<<endl;
    L = new OneChoiceSDdNoOMAPClient(N, inMemory, overwrite, true);
	this->overwrite = overwrite;
   	this->deleteFiles = deleteFiles;
	l = ceil((float)log2(N));
	b = ceil((float)log2(B));
    memset(nullKey.data(), 0, AES_KEY_SIZE);
	numOfIndices = l - b;
    for (int i = 0; i <=numOfIndices; i++) 
	{
		int j = i + b;
        int curNumberOfBins = j > 1 ? 
			(int) ceil(((float) pow(2, j))/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : pow(2,j);
        int curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : 1;
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
		int is = curNumberOfBins*curSizeOfEachBin;
		indexSize.push_back(is);
//        printf("DeAm:%d number of Bins:%d size of bin:%d is:%d\n", i, curNumberOfBins, curSizeOfEachBin, is);
    }
    for (int i = 0; i <= numOfIndices; i++) 
	{
        keys.push_back(vector<unsigned char*> ());
    	for (int j = 0; j < 4; j++) 
		{
            unsigned char* tmpKey = new unsigned char[16];
            keys[i].push_back(tmpKey);
        }
    }
    for (int i = 0; i <=numOfIndices ; i++) 
        cnt.push_back(0);
    for (int i = 0; i < localSize; i++) 
        localmap.push_back(map<string, string>());
    for (int j = 0; j < 4; j++) 
	{
        vector< unordered_map<string, prf_type> > curVec;
        for (int i = 0; i < localSize; i++) 
            curVec.push_back(unordered_map<string, prf_type>());
        data.push_back(curVec);
    }
    if (!overwrite) 
    {
        fstream file("/tmp/existStatus.txt", std::ofstream::in);
        if (file.fail()) 
		{
            file.close();
            return;
        }
        for (unsigned int i = 0; i <= numOfIndices; i++) 
		{
			for(int j = 0; j< 3; j++)
			{
            	string data;
            	getline(file, data);
            	if (data == "true") 
	    		{
                	L->exist[i][j] = true;
                	unsigned char* newKey = new unsigned char[16];
                	memset(newKey, 0, 16);
                	keys[i][j] = newKey;
            	} 
				else 
	    		{
                	L->exist[i][j] = false;
            	}
        	}
    	}
       	file.close();
	}
}

DeAmortized::~DeAmortized() 
{
	if(overwrite)
	{
	    fstream file("/tmp/existStatus.txt", std::ofstream::out);
	    if (file.fail()) 
	        cerr << "Error: " << strerror(errno);
	    for (unsigned int i = localSize; i <= numOfIndices; i++) 
	    {
			for(int j = 0; j<3;j++)
			{
	        	if (L->exist[i][j]) 
	            	file << "true" << endl;
	        	else 
	            	file << "false" << endl;
			}
		}    
    	file.close();
	}
}

float by(int a, int b)
{
	float d = ((float)a/(float)b);
	return d;
}

void DeAmortized::update(OP op, string keyword, int ind, bool setup) 
{
	for(int i=numOfIndices; i>0; i--)
	{
		if(L->exist[i-1][0] && L->exist[i-1][1])
		{
			int t = numberOfBins[i-1];
			int m = numberOfBins[i];
			if(i>3)
			{
				assert(2*t+2*m<pow(2,i));
				if(0 <= cnt[i] && cnt[i] < t)
				{
					L->Phase1();
				}
				else if(t <= cnt[i] && cnt[i] < 2*t)
				{
					L->Phase2();
				}
				else if (2*t <= cnt[i] && cnt[i] < 2*t+m)
				{
					L->LinearScanBinCount();
				}
				else if(2*t+m <= cnt[i] && cnt[i] < 2*t+2*m)
				{
					L->addDummy();
				}
				else if (2*t+2*m <= cnt[i] && cnt[i] <pow(2,i))
				{
					L->deAmortizedBitSort();
				}
			}
			else if(i<=3)
			{
				L->Phase1();
				L->Phase2();
				L->LinearScanBinCount();
				L->addDummy();
				L->deAmortizedBitSort();
			}
			cnt[i] = cnt[i]+1;
			if(cnt[i] == pow(2,i))
			{
				//L->updateHashTable(i, keys[i][3]);
				L->resize(i,indexSize[i]); 
				L->move(i-1,0,2); 
				updateKey(i-1,0,2);
				L->destroy(i-1,1);
				if(!(L->exist[i][0]))
				{
					L->move(i,0,3);
					updateKey(i,0,3);
				}
				else if(!(L->exist[i][1]))
				{
					L->move(i,1,3);
					updateKey(i,1,3);
				}
				else
				{
					L->move(i,2,3);
					updateKey(i,2,3);
				}
				//L->destroy(i,3);
    	    	unsigned char* newKey = new unsigned char[16];
    	    	memset(newKey, 0, 16);
    	    	keys[i][3] = newKey;
				cnt[i] = 0;
			}
		}
	}
	prf_type keyVal;
	createKeyVal(keyword, ind, op, keyVal);
	L->append(0, keyVal, keys[0][3]);
	L->updateOMAP(0,keyword, keys[0][3]);
	L->updateCounters(0, keys[0][3]);
	if(!(L->exist[0][0]))
	{
		L->move(0,0,3);
		updateKey(0,0,3);
	}
	else if(!(L->exist[0][1]))
	{
		L->move(0,1,3);
		updateKey(0,1,3);
	}
	else
	{
		L->move(0,2,3);
		updateKey(0,2,3);
	}
	//L->destroy(0,3);
    unsigned char* newKey = new unsigned char[16];
    memset(newKey, 0, 16);
    keys[0][3] = newKey;
    updateCounter++;
}

void DeAmortized::createKeyVal(string keyword, int ind, OP op, prf_type& keyVal)
{
    memset(keyVal.data(), 0, AES_KEY_SIZE);
    std::copy(keyword.begin(), keyword.end(), keyVal.begin());//keyword
    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = ind;//fileid
    keyVal.data()[AES_KEY_SIZE - 6] = (byte) (op == OP::INS ? 0 : 1);//op
    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 11])) = 0;//index 0 has only bin 0
}

void DeAmortized::updateKey(int index, int toInstance , int fromInstance)
{
	keys[index][toInstance] = keys[index][fromInstance];
	//cout <<"keys:("<<index<<","<<toInstance<<")<-("<<index<<","<<fromInstance<<")"<<endl;
}

vector<int> DeAmortized::search(string keyword) 
{
    for (int i = 0; i < l; i++) 
	{
        L->omaps[i]->treeHandler->oram->totalRead = 0;
        L->omaps[i]->treeHandler->oram->totalWrite = 0;
    }
    L->totalCommunication = 0;
    totalSearchCommSize = 0;
    vector<int> finalRes;
    vector<prf_type> encIndexes;
	/*
    for (int j = 0; j < 3; j++) 
	{
        for (int i = 0; i < localSize; i++) 
		{
            if (data[j][i].size() > 0) 
			{
                int curCounter = 1;
                bool exist = true;
                do 
				{
                    if (data[j][i].count(keyword + "-" + to_string(curCounter)) != 0) 
					{
                        encIndexes.push_back(data[j][i][keyword + "-" + to_string(curCounter)]);
                        curCounter++;
                    } 
					else 
					{
                        exist = false;
                    }
                } 
				while (exist);
            }
        }
    }
	*/
    for (int i = 0; i <= numOfIndices; i++) 
	{
    	for (int j = 0; j < 3; j++) 
		{
            if (L->exist[i][j]) 
			{
				//cout <<"searching at["<< i<<"]["<<j<<"]"<<endl;
                auto tmpRes = L->NIsearch(i, j, keyword, keys[i][j]);
                encIndexes.insert(encIndexes.end(), tmpRes.begin(), tmpRes.end());
            }
        }
    }
    map<int, int> remove;
	int ressize = 0;
    for (auto i = encIndexes.begin(); i != encIndexes.end(); i++) 
	{
        prf_type decodedString = *i;
        int id = *(int*) (&(decodedString.data()[AES_KEY_SIZE - 5]));
        int op = ((byte) decodedString.data()[AES_KEY_SIZE - 6]);
        remove[id] += (2 *op - 1);
	    if ((strcmp((char*) decodedString.data(), keyword.data()) == 0)) 
			ressize++;
    }
	//cout <<"size of remove:"<<remove.size()<<endl;
	int r = 1;
    for (auto const& cur : remove) 
	{
		//cout <<r<<":"<<cur.first <<"::"<<cur.second<<endl;
		//r++;
        if (cur.second < 0) 
		{
			for(int i = 0; i<abs(cur.second); i++)
            	finalRes.emplace_back(cur.first);
		}
    }
    for (int i = 0; i < l; i++) 
	{
        totalSearchCommSize += (L->omaps[i]->treeHandler->oram->totalRead + 
				L->omaps[i]->treeHandler->oram->totalWrite)*(sizeof (prf_type) + sizeof (int));
    }
    totalSearchCommSize += L->totalCommunication;
	cout <<"NUMBER OF TOTAL RETURN:"<<ressize<<endl;
    return finalRes;
}

/*
void DeAmortized::endSetup() 
{
    for (unsigned int i = 0; i < setupOMAPS.size(); i++) 
	{
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
