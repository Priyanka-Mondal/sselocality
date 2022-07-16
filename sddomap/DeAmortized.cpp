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
			(int) ceil(((float) pow(2, j))/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        int curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : pow(2,j);
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
		int is = curNumberOfBins*curSizeOfEachBin;
		indexSize.push_back(is);
        printf("DeAm:%d number of Bins:%d size of bin:%d is:%d\n", i, curNumberOfBins, curSizeOfEachBin, is);
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
	int prof = 1;
	for(int i=numOfIndices; i>0; i--)
	{
		int s = i>1 ? 6 : 2;
		int j = b+i;
		int mi = numberOfBins[j]; 
		int r1 = ceil((float)(2*floor(by(indexSize[i-1],s))));
		int r2 = r1 + ceil(by(mi,s));
		assert(r1<r2);
		assert(r2<=pow(2,j));
		if(L->exist[i-1][0] && L->exist[i-1][1])
		{
			if(cnt[i] <= r1)
			{
				if(i==prof) 
					cout <<j <<" GETBIN:"<<cnt[i]<<"/"<<r1<<"/"<<pow(2,j)<<endl;
				L->getBin(i, 0, cnt[i]*(s/2),(s/2), keys[i-1][0], keys[i][3]);
				L->getBin(i, 1, cnt[i]*(s/2),(s/2), keys[i-1][1], keys[i][3]);
			}
			if (r1 <=cnt[i] &&  cnt[i]<= r2)
			{
				if(r2 == cnt[i])
				{
					assert(L->getNEWsize(i) == 2*indexSize[i]+2*indexSize[i-1]);
					if(i==prof) 
						cout <<j <<" PADDING:"<<cnt[i]<<"/"<<r2<<"/"<<pow(2,j)<<endl;
					int newSize = ceil((float)log2(2*indexSize[i]+2*indexSize[i-1]));
					newSize = pow(2,newSize);
					L->pad(i,newSize, keys[i][3]);
					assert(newSize>=2*indexSize[i]);
				}
				else
				{
					if(i==prof) 
						cout <<j<<" ADDING DUMMY:"<<cnt[i]<<"/"<<r2<<"/"<<pow(2,j)<<endl;
				}
				L->addDummy(i, (cnt[i]-r1), keys[i][3], s);
			}
			if (r2 <= cnt[i] && cnt[i] <pow(2,j))
			{
				int count = cnt[i]-r2;
				int times = pow(2,j)-r2;
				int N = L->getNEWsize(i);
				int totStepsi = 2*ceil(by(N*log2(N)*(log2(N)+1),4));
				int stepi = ceil(by(by(totStepsi, times),2));
				stepi = stepi*2;
				if(i==prof) 
					cout <<j<<" SORTING:"<<cnt[i]<<"/"<<pow(2,j)<<"/"<<pow(2,j)<<" totstep:"<<totStepsi<<" stepi:"<<stepi<<endl;
				//cout <<"j:"<<j<<" cnt[i]:"<<cnt[i]<<" N:"<<N<<
				//	" totStepsi:"<<totStepsi<<" stepi:"<<stepi<<" count:"<<count<<" times:"<< times<<endl;
				L->deAmortizedBitSort(stepi, count, N, i, keys[i][3]);
			}
			cnt[i] = cnt[i]+1;
			if(cnt[i] == pow(2,j))
			{
				L->resize(i,indexSize[i]); //j = i+logB
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
	createKeyVal(keyword,ind,op, keyVal);
	L->append(0, keyVal, keys[0][3]);
	cnt[0]=cnt[0]+1;
	if(cnt[0]==B)
	{
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
		cnt[0] = 0;
	}
    updateCounter++;
}

void DeAmortized::createKeyVal(string keyword, int ind, OP op, prf_type& keyVal)
{
    memset(keyVal.data(), 0, AES_KEY_SIZE);
    std::copy(keyword.begin(), keyword.end(), keyVal.begin());//keyword
    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 5])) = ind;//fileid
    keyVal.data()[AES_KEY_SIZE - 6] = (byte) (op == OP::INS ? 0 : 1);//op
    *(int*) (&(keyVal.data()[AES_KEY_SIZE - 10])) = 0;//bin
}

void DeAmortized::updateKey(int index, int toInstance , int fromInstance)
{
	keys[index][toInstance] = keys[index][fromInstance];
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
				//cout <<"searching at["<< i<<"]["<<j<<"]"<<endl;
                auto tmpRes = L->search(i, j, keyword, keys[i][j]);
                encIndexes.insert(encIndexes.end(), tmpRes.begin(), tmpRes.end());
            }
        }
    }
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
            finalRes.emplace_back(cur.first);
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
