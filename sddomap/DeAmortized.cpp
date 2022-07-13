#include "DeAmortized.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <vector>
using namespace std;

DeAmortized::DeAmortized(int N, bool inMemory, bool overwrite) 
{
	cout <<"=====================Running SDd+OneChoiceAllocation======================"<<endl;
    L = new OneChoiceClient(N, inMemory, overwrite, true);
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
			(int) ceil((float) pow(2, j)/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        int curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : pow(2,j);
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
        //printf("Index:%d number of Bins:%d size of bin:%d\n", i, curNumberOfBins, curSizeOfEachBin);
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
		cout <<"READING-"<<overwrite<<endl;
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
    {
        cerr << "Error: " << strerror(errno);
    }
    for (unsigned int i = localSize; i <= numOfIndices; i++) 
    {
		for(int j = 0; j<3;j++)
		{
        	if (L->exist[i][j]) 
			{
            	file << "true" << endl;
			}
        	else 
			{
            	file << "false" << endl;
			}
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
	//int s = 2; // for now choose wisely and make sure to not overflow 
	int t;
	int s; 
	for(int i=numOfIndices; i>0; i--)
	{
		int j = b+i;
		int mi = numberOfBins[j]; // for now
		if(L->exist[i-1][0] && L->exist[i-1][1])
		{
			//t = i>1 ? 3 : 1;
			s = i>1 ? 6 : 2;
			//if(cnt[i] <=(ceil((float)t*(by(pow(2,j),s)))))
			if(cnt[i] <=(ceil((float)(by(numberOfBins[i]*sizeOfEachBin[i],s)))))
			{
				L->getBin(i, 0, cnt[i]*(s/2),(cnt[i]+1)*(s/2), updateCounter, keys[i-1][0], keys[i][3]);
				L->getBin(i, 1, cnt[i]*(s/2),(cnt[i]+1)*(s/2), updateCounter, keys[i-1][1], keys[i][3]);
			}
			//else if ((ceil((float)t*by(pow(2,j),s)))<=cnt[i] && 
			else if ((ceil((float)t*by(numberOfBins[i]*sizeOfEachBin[i],s)))<=cnt[i] && 
					  cnt[i]<((ceil((float)by(numberOfBins[i]*sizeOfEachBin[i],s)))+(ceil(by(mi,s)))))
				L->addDummy(i, (cnt[i]-(ceil((float)by(numberOfBins[i]*sizeOfEachBin[i],s)))), updateCounter, keys[i][3]);
			//else if (((ceil((float)t*by(pow(2,j),s)))+ceil(by(mi,s)))<=cnt[i] && cnt[i]<=pow(2,j))
			else if (((ceil((float)by(numberOfBins[i]*sizeOfEachBin[i],s)))+ceil(by(mi,s)))<=cnt[i] && cnt[i]<=numberOfBins[i]*sizeOfEachBin[i])
				L->nonOblSort(i, keys[i][3]);
				//L->bitonicSort(stepi, i,(cnt[i]-ceil(t*pow(2,j)/s)-ceil(mi/s))));
		
			cnt[i]=cnt[i]+1;
			if(cnt[i]== numberOfBins[i]*sizeOfEachBin[i])
			{
				L->resize(i,numberOfBins[j]*sizeOfEachBin[j]); //j = i+logB
				L->move(i-1,0,2); 
				updateKey(i-1,0,2);
				//cout<<"OLD["<<i-1<<"] to OLDEST["<<i-1<<"]"<<endl;
				L->destroy(i-1,1);
				if(!(L->exist[i][0]))
				{
				//	cout<<"NEW["<<i<<"] to OLDEST["<<i<<"]"<<endl;
					L->copy(i,0);
					updateKey(i,0,3);
				}
				else if(!(L->exist[i][1]))
				{
				//	cout<<"NEW["<<i<<"] to OLDER["<<i<<"]"<<endl;
					L->copy(i,1);
					updateKey(i,1,3);
				}
				else
				{
				//	cout<<"NEW["<<i<<"] to OLD["<<i<<"]"<<endl;
					L->copy(i,2);
					updateKey(i,2,3);
				}
    	    	unsigned char* newKey = new unsigned char[16];
    	    	memset(newKey, 0, 16);
    	    	keys[i][3] = newKey;
				cnt[i] = 0;
			}
		}
	}
	prf_type keyVal;
	createKeyVal(keyword,ind,op, keyVal);
	L->append(0, keyVal, keys[0][3]);
	cnt[0]=cnt[0]+1;
	if(cnt[0]==B)
	{
		if(!(L->exist[0][0]))
		{
			//cout<<"NEW[0] to OLDEST[0]"<<endl;
			L->copy(0,0);
			updateKey(0,0,3);
		}
		else if(!(L->exist[0][1]))
		{
			//cout<<"NEW[0] to OLDER[0]"<<endl;
			L->copy(0,1);
			updateKey(0,1,3);
		}
		else
		{
			//cout<<"NEW[0] to OLD[0]"<<endl;
			L->copy(0,2);
			updateKey(0,2,3);
		}
       	unsigned char* newKey = new unsigned char[16];
       	memset(newKey, 0, 16);
       	keys[0][3] = newKey;
		cnt[0] = 0;
	}
    updateCounter++;
	cout <<"-----------------------------------"<<endl;
}

void DeAmortized::createKeyVal(string keyword, int ind, OP op, prf_type& keyVal)
{
    std::fill(keyVal.begin(), keyVal.end(), 0);
    std::copy(keyword.begin(), keyword.end(), keyVal.begin());//keyword
    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = ind;//fileid
    keyVal.data()[AES_KEY_SIZE - 6] = (byte) (op == OP::INS ? 0 : 1);//op
    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 10])) = 0;//bin
}

void DeAmortized::updateKey(int index, int toInstance , int fromInstance)
{
	keys[index][toInstance] = keys[index][fromInstance];
	//cout <<"key["<<index<<"]["<<fromInstance<<"] to key["<<index<<"]["<<toInstance<<"]"<<endl;
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
    for (int i = localSize; i <= numOfIndices; i++) 
	{
    	for (int j = 0; j < 3; j++) 
		{
            if (L->exist[i][j]) 
			{
				cout <<"searching at["<< i<<"]["<<j<<"]"<<endl;
                auto tmpRes = L->search(i, j, keyword, keys[i][j]);
                encIndexes.insert(encIndexes.end(), tmpRes.begin(), tmpRes.end());
				cout <<"-----------------------------------------------"<<endl;
            }
        }
    }
	auto tmpRes = L->searchNEW(numOfIndices, keyword);
    encIndexes.insert(encIndexes.end(), tmpRes.begin(), tmpRes.end());
    map<int, int> remove;
    for (auto i = encIndexes.begin(); i != encIndexes.end(); i++) 
	{
        prf_type decodedString = *i;
        int plaintext = *(int*) (&(decodedString.data()[AES_KEY_SIZE - 5]));
        remove[plaintext] += (2 * ((byte) decodedString.data()[AES_KEY_SIZE - 6]) - 1);
    }
    for (auto const& cur : remove) 
	{
        if (cur.second < 0) 
		{
            finalRes.emplace_back(cur.first);
        }
    }
    for (int i = 0; i < l; i++) 
	{
        totalSearchCommSize += (L->omaps[i]->treeHandler->oram->totalRead + 
				L->omaps[i]->treeHandler->oram->totalWrite)*(sizeof (prf_type) + sizeof (int));
    }
    totalSearchCommSize += L->totalCommunication;
    return finalRes;
}
/*
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
