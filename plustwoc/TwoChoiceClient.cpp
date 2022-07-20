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
	this->profile = profile;
	server = new TwoChoiceServer(numOfDataSets, inMemory, overwrite, profile);
    one = new OneChoiceServer(numOfDataSets, inMemory, overwrite, profile);
	cout <<"===================RUNNING SDa+TwoChoicePLUSPLUS (long) ====================="<<endl;
	memset(nullKey.data(), 0, AES_KEY_SIZE);
	for (long i = 0; i < numOfDataSets; i++) //why not <=
	{
		exist.push_back(false);
		long curNumberOfBins = i > 3 ? ((long) ceil((float) pow(2, i) / (log2(log2(log2(pow(2,i))))))) : pow(2,i);
		curNumberOfBins = pow(2, (long)ceil(log2(curNumberOfBins)));
	 //long curSizeOfEachBin = i > 3 ? 3*ceil((log2(log2(pow(2,i)))*(log2(log2(log2(pow(2,i))))))) : 3;
	 	long curSizeOfEachBin = i > 3 ? 3*ceil((log2(log2(log2(pow(2,i)))))) : 3;
		numberOfBins.push_back(curNumberOfBins);
		sizeOfEachBin.push_back(curSizeOfEachBin);
	}
	
   	for (long j = 0; j <numOfDataSets; j++) 
	{
        long curNumberOfBins = j > 1 ? 
			(long) ceil(((float) pow(2, j))/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        long curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : pow(2,j);
        nB.push_back(curNumberOfBins);
        sEB.push_back(curSizeOfEachBin);
		long is = curNumberOfBins*curSizeOfEachBin;
	//	indexSize.push_back(is);
    }
}

long countTotal(map<long, long> fullness, long bin, long size)
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
	long N = pow(2, index);
	if(N == 1) return 1;
	long l = (float)(log2(N)*log2(N)*log2(N));
	long max = ceil((float)N/(float)l);
	//assert(max <3); // this never fails, which implies mpl is never >3,
	if(max>numberOfBins[index])
		max = numberOfBins[index];
	max = pow(2, (long)floor((float)log2(max)));
	//max = numberOfBins[index];
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

void TwoChoiceClient::writeToCuckooStash(vector<prf_type> fileids, long cnt, 
		long index, long tableNum, unsigned char* key)
{
	long entrySize = pow(2, tableNum);
	vector<prf_type> ctCiphers;
	for (auto c : fileids) 
	{
		prf_type value;
		value = Utilities::encode(c.data(), key);
		ctCiphers.push_back(value);
	}
	if(fileids.size()<entrySize)
	{
		prf_type dummy;
		memset(dummy.data(), 0, AES_KEY_SIZE);
		for(long i = fileids.size(); i<entrySize ; i++)
		ctCiphers.push_back(dummy);
	}
	server->insertCuckooStash(index, tableNum, ctCiphers);
}

void TwoChoiceClient::place(string keyword, vector<prf_type> fileids, long cuckooID, 
		long cnt, long index, long tableNum, unsigned char* key)
{
	if(cnt == (pow(2,index-tableNum))+1) // check this condition
	{
		cout <<"Cuckoo overflow: write in cuckooStash:"<<" index:"<<index<<" tableNum:"<<tableNum<<endl;
		writeToCuckooStash(fileids, cnt, index, tableNum, key);
		return;
	}
	long entrySize = pow(2, tableNum);
	long entryNum = pow(2,(index-tableNum));

	string temp = keyword;
	unsigned char cntstr[AES_KEY_SIZE];
	if(cuckooID == 0)
		temp = temp.append("0");
	else
		temp = temp.append("1");

	prf_type K = Utilities::encode(temp, key);
	memset(cntstr, 0, AES_KEY_SIZE);
	*(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
	prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
	unsigned char* h = Utilities::sha256((char*) (mapKey.data()), AES_KEY_SIZE);
	long hash = (unsigned long) (*((long*) h)) % entryNum; 
	
	prf_type encKeyw = Utilities::encode(keyword, key);
	pair<prf_type ,vector<prf_type>> dis = server->insertCuckooHT(index, tableNum, cuckooID, hash,
									encKeyw,fileids);
	if(dis.first != nullKey)
	{
		string keyw = Utilities::decode(dis.first, key);
		place(keyw, dis.second, cnt+1, ((cuckooID+1)%2), index, tableNum, key);
	}
}
void TwoChoiceClient::writeToCuckooHT(long index, long size, string keyword, 
				vector<prf_type> fileids, unsigned char* key)
{
	assert(fileids.size() > 0);
	long tableNum = (long)ceil((float) log2(size));

	vector<prf_type> ctCiphers;
	for(long i = 0; i<fileids.size(); i++)
	{
		prf_type fid = Utilities::encode(fileids[i].data(), key);
		ctCiphers.push_back(fid);
	}
	ctCiphers.resize(size);
	if(fileids.size()<size)
	{
		prf_type dummy;
		memset(dummy.data(), 0, AES_KEY_SIZE);
		for(long i = fileids.size(); i<size ; i++)
		ctCiphers.push_back(dummy);
	}
	place(keyword, ctCiphers, 0, 0, index, tableNum, key);
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
	map<long, long> fullness;
	for(int b = 0; b<numberOfBins[index];b++)
		fullness[b]=0;

	vector<pair<string,vector<prf_type>>> sorted = sort(pairs);
	long mpl = maxPossibleLen(index);

	for (auto pair : sorted) 
	{
		long pss = pair.second.size();
		long newsize = pss;
		long flagOneChoice = 0;
		if(pss > mpl)
		{
			flagOneChoice = 1;
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
       		for (unsigned long i = 0; i < pair.second.size(); i++) 
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
		else
		{
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
					//if(fullness.find(cipherIndex) == fullness.end())
					//	fullness[cipherIndex] = 1;
					//else
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
					//if(fullness.find(cipherIndex) == fullness.end()) //although this check is not reqd
					//	fullness[cipherIndex] = 1;
					//else
						fullness[cipherIndex] = fullness[cipherIndex]+1;
				   	cipherIndex++;
				}
			 }
			 else
				 writeToCuckooHT(index, newsize, pair.first, pair.second, key);
		}
		prf_type K = Utilities::encode(pair.first, key);
		unsigned char cntstr[AES_KEY_SIZE];
		memset(cntstr, 0, AES_KEY_SIZE);
		*(long*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
		prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
		prf_type valueTmp, totalTmp;
		if(flagOneChoice == 0)
			*(long*) (&(valueTmp[0])) = newsize;
		else if(flagOneChoice == 1) 
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
		ciphers = server->search(index, token, hashtoken, keywordCnt, numberOfBins[index]);
		if(flag == 0)
		{
			for (auto item : ciphers) 
			{
		 		prf_type plalongext;
		 		Utilities::decode(item, plalongext, key);
		 		if (strcmp((char*) plalongext.data(), keyword.data()) == 0) 
				{
		 			finalRes.push_back(plalongext);
					flag++;
		 		}
			}
		}
		totalCommunication += ciphers.size()* sizeof(prf_type);
	}
	if(keywordCnt > maxPossibleLen(index))
	{
		oneChoiceCiphers = one->search(index,token,keywordCnt);
		cout <<"Searching ["<<keyword<<"] in One choice bins of index:"<<index<<endl;
		if(oneChoiceCiphers.size()>0)
		{
			for (auto item : oneChoiceCiphers) 
			{
				prf_type plalongext;
				Utilities::decode(item, plalongext, key);
				if (strcmp((char*) plalongext.data(), keyword.data()) == 0) 
				{
						finalRes.push_back(plalongext);
				}
			}
		}
		totalCommunication += cuckooCiphers.size()* sizeof(prf_type);
	}

	long tableNum = (long)ceil(log2(keywordCnt));
	if(keywordCnt>0)
	{
		string newkeyword = keyword;
		newkeyword = newkeyword.append("0");
		prf_type hashtoken1 = Utilities::encode(newkeyword, key);
		newkeyword = keyword;
		newkeyword = newkeyword.append("1");
		prf_type hashtoken2 = Utilities::encode(newkeyword, key);
		cuckooCiphers = server->cuckooSearch(index, tableNum, hashtoken1, hashtoken2);//search HT+cuckoostash
		if(cuckooCiphers.size()>0)
		{
			for (auto item : cuckooCiphers) 
			{
				prf_type plalongext;
				Utilities::decode(item, plalongext, key);
				if (strcmp((char*) plalongext.data(), keyword.data()) == 0) 
				{
						finalRes.push_back(plalongext);
				}
				cout <<"cuckoo data:"<<plalongext.data()<<endl;
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
	vector<prf_type> cuckooCiphers = server->getCuckooHT(index); // also fetches the cuckoo stash
	//vector<prf_type> stashCiphers = server->getStash(index);

	for (auto cipher : ciphers) 
	{
		prf_type plalongext;
		Utilities::decode(cipher.second, plalongext, key);
		finalRes.push_back(plalongext);
	}
	for (auto cipher : oneChoiceCiphers) 
	{
		prf_type plalongext;
		Utilities::decode(cipher.second, plalongext, key);
		finalRes.push_back(plalongext);
	}
	/*if(stashCiphers.size()>0)
	{
		//cout <<"size of stash ciphers:"<<stashCiphers.size()<<endl;
		for(auto b : stashCiphers)
		{
	   		prf_type plalongext;
	   		Utilities::decode(b, plalongext, key);
		//cout <<"{"<<plalongext.data()<<"}"<<endl;
		finalRes.push_back(plalongext);
		}
	}*/
	if(cuckooCiphers.size()>0)
	{
		cout <<"getAllData:size of cuckoo ciphers:"<<cuckooCiphers.size()<<endl;
		for(auto b : cuckooCiphers)
		{
	   		prf_type plalongext;
	   		Utilities::decode(b, plalongext, key);
		finalRes.push_back(plalongext);
		}
	}
	totalCommunication += (ciphers.size() +  oneChoiceCiphers.size()+ cuckooCiphers.size())* sizeof (prf_type);
	return finalRes;
}

void TwoChoiceClient::destry(long index) 
{
	server->clear(index);
	one->clear(index);
	exist[index] = false;
	totalCommunication += sizeof (long);
}
