#include "TwoChoiceClient.h"
#include<vector>
#include<algorithm>

TwoChoiceClient::~TwoChoiceClient() 
{
	delete server;
	delete one;
}

TwoChoiceClient::TwoChoiceClient(long numOfDataSets, bool inMemory, bool overwrite, bool profile) 
{
	cout <<"===============RUNNING SDa+TwoChoice+(OneChoice version 2)(long) ================="<<endl;
	this->profile = profile;
	server = new TwoChoiceServer(numOfDataSets, inMemory, overwrite, profile);
    one = new OneChoiceServer(numOfDataSets, inMemory, overwrite, profile);
	memset(nullKey.data(), 0, AES_KEY_SIZE);
    for (long i = 0; i < numOfDataSets; i++) 
    {
        exist.push_back(false);
        long curNumberOfBins = i > 3 ? ((long) ceil((float) pow(2, i) / ((log2(log2(pow(2,i))))*(log2(log2(log2(pow(2,i)))))*(log2(log2(log2(pow(2,i)))))))) : pow(2,i);
	curNumberOfBins = pow(2, (long)ceil(log2(curNumberOfBins))); 
     	long curSizeOfEachBin = i > 3 ? ceil(SO*(log2(log2(pow(2,i))))*(log2(log2(log2(pow(2,i)))))*(log2(log2(log2(pow(2,i)))))) : SO;
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
    //    printf("Level:%d number of Bins:%d size of bin:%d\n", i, curNumberOfBins, curSizeOfEachBin);
    }
   	for (long j = 0; j <numOfDataSets; j++) 
	{
        long curNumberOfBins = j > 1 ? 
			(long) ceil(((float) pow(2, j))/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        long curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : pow(2,j);
        nB.push_back(curNumberOfBins);
        sEB.push_back(curSizeOfEachBin);
    }
	sleep(1);
}

long countTotal(vector<long> fullness, long bin, long size)
{
	long full = 0;
	for(long i = 0; i< size; i++)
		full = full + fullness[bin+i];
	return full;
}

bool cmpp(pair<string, vector<prf_type>> &a, pair<string, vector<prf_type>> &b)
{
	//cout <<"cmp:["<<a.second.size()<< " "<<b.second.size()<<"]["<<(a.second.size() > b.second.size()) <<"]"<<endl;
	return (a.second.size() > b.second.size());
}

vector<pair<string, vector<prf_type>>> sort(map<string, vector<prf_type>> &M)
{
	vector<pair<string, vector<prf_type>>> A;
	for (auto& it : M) 
	{
		assert(it.first != "");
		A.push_back(it);
	}
	sort(A.begin(), A.end(), cmpp);
	return A;
}


long TwoChoiceClient::maxPossibleLen(long index)
{
	long N = pow(2,index);
	long bins = numberOfBins[index];
	long max;
	if(N<4)
		max = bins;
	else
	{
		float p = (float) ((float)1/log2(log2(N)));
		float m = (float) (1-p);
		max = (float) floor(pow(N,m));
	}
	long maxmax= pow(2, (long)ceil(log2(max)));
	long minmin= pow(2, (long)floor(log2(max)));
	if(maxmax<= bins)
		max = maxmax;
	else
		max = minmin;
	return max;
}


void TwoChoiceClient::writeToStash(long pss, long mpl, vector<prf_type> fileids, 
		unsigned char* key, vector<prf_type> &stashCiphers)
{
	for (unsigned long i = mpl; i < pss; i++) 
	{
		prf_type value;
		value = Utilities::encode(fileids[i].data(), key);
		stashCiphers.push_back(value);
	}
}

void TwoChoiceClient::setup(long index, map<string, vector<prf_type> > pairs, unsigned char* key) 
{
	exist[index] = true;
	vector<vector<pair<prf_type, prf_type>>> ciphers;
	vector<vector<pair<prf_type, prf_type>>> ciphersOne;
	for (long i = 0; i < numberOfBins[index]; i++) 
		ciphers.push_back(vector<pair<prf_type,prf_type>>());
	for (long i = 0; i < nB[index]; i++) 
		ciphersOne.push_back(vector<pair<prf_type,prf_type>>());
	
	map<prf_type, prf_type> keywordCntCiphers;
	vector<long> fullness;
	fullness.resize(0);
	for(long f = 0; f<numberOfBins[index]; f++)
	{
		fullness.push_back(0);
	}

	long mpl = maxPossibleLen(index);
	vector<pair<string,vector<prf_type>>> sorted = sort(pairs);
	for (auto pair : sorted) 
	{
		long pss = pair.second.size();
		long newsize = pss;
		if(pss > mpl)
		{
	   		assert(pair.first != "");
       		prf_type K1 = Utilities::encode(pair.first, key);
       		prf_type mapKey, mapValue;
       		unsigned char cntstr[AES_KEY_SIZE];
       		memset(cntstr, 0, AES_KEY_SIZE);
       		*(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
       		mapKey = Utilities::generatePRF(cntstr, K1.data());
        	unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
        	long pos = ((unsigned long) (*((long*) hash))) % nB[index];
        	long cipherIndex = pos;
       		for (unsigned long i = mpl; i < pair.second.size(); i++) 
	   	 	{
       	        prf_type mapKey, mapValue;
       	        unsigned char cntstr[AES_KEY_SIZE];
       	        memset(cntstr, 0, AES_KEY_SIZE);
       	        *(long*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
       	        mapKey = Utilities::generatePRF(cntstr, K1.data());
       	        mapValue = Utilities::encode(pair.second[i].data(), key);
       	        auto p = std::pair<prf_type, prf_type>(mapKey, mapValue);
       	        ciphersOne[cipherIndex].push_back(p);
       	        cipherIndex++;
       	        if (cipherIndex == nB[index]) 
	   	 	    {
       	            cipherIndex = 0;
       	        }
       	 	}
    	}
		if(pss>mpl)	
			pss = mpl;
		newsize = pow(2, (long)ceil(log2(pss)));
		if(newsize > mpl)
			newsize = mpl;
	   
		string temp = pair.first;
		temp = temp.append("1");
		prf_type K1 = Utilities::encode(temp, key);
    	unsigned char cntstr[AES_KEY_SIZE];
		memset(cntstr, 0, AES_KEY_SIZE);
		*(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
		prf_type mapKey1 = Utilities::generatePRF(cntstr, K1.data());
		unsigned char* hash1 = Utilities::sha256((char*) (mapKey1.data()), AES_KEY_SIZE);

		temp = pair.first;
		temp = temp.append("2");
		prf_type K2 = Utilities::encode(temp, key);
		memset(cntstr, 0, AES_KEY_SIZE);
		*(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
		prf_type mapKey2 = Utilities::generatePRF(cntstr, K2.data());
		unsigned char* hash2 = Utilities::sha256((char*) (mapKey2.data()), AES_KEY_SIZE);

		long superBins = ceil((float) numberOfBins[index]/newsize); 
		long pos1 = (unsigned long) (*((long*) hash1)) % superBins;
		long pos2 = (unsigned long) (*((long*) hash2)) % superBins; 

		long totalItems1 = countTotal(fullness, pos1*newsize, newsize);
		long totalItems2 = countTotal(fullness, pos2*newsize, newsize);
		long cipherIndex ;
		if(totalItems1>totalItems2)
			cipherIndex = pos2*newsize;
		else
			cipherIndex = pos1*newsize;
		if(fullness[cipherIndex]<sizeOfEachBin[index])
		{
		   	for (unsigned long i = 0; i < pss; i++) 
		   	{
				prf_type K = Utilities::encode(pair.first, key);
				unsigned char cntstr[AES_KEY_SIZE];
				memset(cntstr, 0, AES_KEY_SIZE);
				*(long*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
				prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
				prf_type mapValue = Utilities::encode(pair.second[i].data(), key);
				auto p = std::pair<prf_type, prf_type>(mapKey, mapValue);
				assert(fullness[cipherIndex]<sizeOfEachBin[index]);
				ciphers[cipherIndex].push_back(p);
				fullness[cipherIndex] = fullness[cipherIndex]+1;
				assert(cipherIndex <ciphers.size());
				cipherIndex++;
			}
		   	for(long i = pss; i<newsize; i++)
		   	{
		   		prf_type dummy;
				memset(dummy.data(), 0, AES_KEY_SIZE);
				auto dummypair = std::pair<prf_type, prf_type>(dummy, dummy);
				ciphers[cipherIndex].push_back(dummypair);
				fullness[cipherIndex] = fullness[cipherIndex]+1;
			   	cipherIndex++;
			}
		 }
		 else
		 {	
			 cout <<fullness[cipherIndex]<<"/"<<sizeOfEachBin[index]<<" BIN OVERFLOW, index:"<<index<<endl;
		 }
		prf_type K = Utilities::encode(pair.first, key);
		memset(cntstr, 0, AES_KEY_SIZE);
		*(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
		prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
		prf_type valueTmp, totalTmp;
		*(long*) (&(valueTmp[0])) = pair.second.size(); 
		prf_type mapValue = Utilities::encode(valueTmp.data(), K.data());
		keywordCntCiphers[mapKey] = mapValue; 
	}
	prf_type dummy;
	memset(dummy.data(), 0, AES_KEY_SIZE);
	auto dummypair = pair<prf_type, prf_type>(dummy, dummy);
	for (long i = 0; i < numberOfBins[index]; i++)  //filling up each bin to maximum capacity
	{
		long curSize = ciphers[i].size();
		for (long j = curSize; j < sizeOfEachBin[index]; j++) 
			ciphers[i].push_back(dummypair);
	}
    for (long i = 0; i < nB[index]; i++) 
	{
        long curSize = ciphersOne[i].size();
        for (long j = curSize; j < sEB[index]; j++) 
		{
            ciphersOne[i].push_back(dummypair);
        }
    }
	prf_type randomKey;
	for (long i = 0; i < AES_KEY_SIZE; i++) 
		randomKey[i] = rand();

	for (long i = keywordCntCiphers.size(); i < pow(2, index); i++) 
	{
		unsigned char cntstr[AES_KEY_SIZE];
		memset(cntstr, 0, AES_KEY_SIZE);
		*(long*) (&(cntstr[AES_KEY_SIZE - 9])) = rand();
		prf_type mapKey = Utilities::generatePRF(cntstr, randomKey.data());
		*(long*) (&(cntstr[AES_KEY_SIZE - 5])) = rand();
		prf_type mapValue = Utilities::generatePRF(cntstr, randomKey.data());
		keywordCntCiphers[mapKey] = mapValue;
	}
	totalCommunication += ciphers.size() * sizeof (prf_type)*2 + ciphersOne.size() * sizeof(prf_type);
	server->storeCiphers(index, ciphers, keywordCntCiphers);
	one->storeCiphers(index,ciphersOne);
}

vector<prf_type> TwoChoiceClient::search(long index, string keyword, unsigned char* key) 
{
	double searchPreparation = 0, searchDecryption = 0;
	long flag = 0;
	if (profile) 
		Utilities::startTimer(65);
	vector<prf_type> finalRes;
	long keywordCnt = 0;
	prf_type hashtoken;
	prf_type token = Utilities::encode(keyword, key);
	vector<prf_type> ciphers;
	ciphers.resize(0);
	vector<prf_type> cuckooCiphers;
	vector<prf_type> oneChoiceCiphers;
	long mpl = maxPossibleLen(index);
	for(long s = 1 ;s<=2; s++)
	{
		string newkeyword = keyword;
		if(s==1) 
		{
	    	newkeyword = newkeyword.append("1");
			hashtoken = Utilities::encode(newkeyword, key);
		}
		else if(s==2)
		{
			newkeyword = keyword;
			newkeyword = newkeyword.append("2");
			hashtoken = Utilities::encode(newkeyword, key);
		}
		
		ciphers = server->search(index, token, hashtoken, keywordCnt, mpl);
		if(flag == 0)
		{
			for (auto item : ciphers) 
			{
		 		prf_type plaintext;
		 		Utilities::decode(item, plaintext, key);
		 		if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
				{
		 			finalRes.push_back(plaintext);
					flag++;
		 		}
			}
		}
		totalCommunication += ciphers.size()* sizeof(prf_type);
	}
	if(keywordCnt > mpl)
	{
		oneChoiceCiphers = one->search(index,token,(keywordCnt-mpl));
		cout <<"Searching ["<<keyword<<"] in One choice bins of index:"<<index<<endl;
		if(oneChoiceCiphers.size()>0)
		{
			for (auto item : oneChoiceCiphers) 
			{
				prf_type plaintext;
				Utilities::decode(item, plaintext, key);
				if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
				{
					finalRes.push_back(plaintext);
				}
			}
		}
		totalCommunication += cuckooCiphers.size()* sizeof(prf_type);
	}
	return finalRes;
}

vector<prf_type> TwoChoiceClient::getAllData(long index, unsigned char* key) 
{
	vector<prf_type> finalRes = vector<prf_type>();
	auto ciphers = server->getAllData(index);
	auto oneChoiceCiphers = one->getAllData(index);

	for (auto cipher : ciphers) 
	{
		prf_type plaintext;
		Utilities::decode(cipher.second, plaintext, key);
		finalRes.push_back(plaintext);
	}
	for (auto cipher : oneChoiceCiphers) 
	{
		prf_type plaintext;
		Utilities::decode(cipher.second, plaintext, key);
		finalRes.push_back(plaintext);
	}
	totalCommunication += (ciphers.size() +  oneChoiceCiphers.size()) * sizeof (prf_type);
	return finalRes;
}

void TwoChoiceClient::destry(long index) 
{
	server->clear(index);
	one->clear(index);
	exist[index] = false;
	totalCommunication += sizeof (long);
}
