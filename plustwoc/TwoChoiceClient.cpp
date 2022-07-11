#include "TwoChoiceClient.h"
#include<vector>
#include<algorithm>

TwoChoiceClient::~TwoChoiceClient() 
{
	delete server;
}

//3. change mpl value

TwoChoiceClient::TwoChoiceClient(int numOfDataSets, bool inMemory, bool overwrite, bool profile) 
{
	cout <<"===================RUNNING SDa+TwoChoicePLUSPLUS====================="<<endl;

	this->profile = profile;
	server = new TwoChoiceServer(numOfDataSets, inMemory, overwrite, profile);
	memset(nullKey.data(), 0, AES_KEY_SIZE);
	for (int i = 0; i < numOfDataSets; i++) //why not <=
	{
		exist.push_back(false);
		int curNumberOfBins = i > 3 ? ((int) ceil((float) pow(2, i) / (log2(log2(log2(pow(2,i))))))) : pow(2,i);
		curNumberOfBins = pow(2, (int)ceil(log2(curNumberOfBins)));
	 //int curSizeOfEachBin = i > 3 ? 3*ceil((log2(log2(pow(2,i)))*(log2(log2(log2(pow(2,i))))))) : 3;
	 	int curSizeOfEachBin = i > 3 ? 3*ceil((log2(log2(log2(pow(2,i)))))) : 3;
		numberOfBins.push_back(curNumberOfBins);
		sizeOfEachBin.push_back(curSizeOfEachBin);
	}
}

int countTotal(map<int,int> fullness, int bin,int size)
{
	int full = 0;
	for(int i = 0; i< size; i++)
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

int maxPossibleLen(int N, int bins, int index)
{
	int max;
	if(N<4)
		max = bins;
	else
	{
		float p = (float) ((float)1/log2(log2(N)));
		float m = (float) (1-p);
		max = (float) floor(pow(N,m));
	}
	int maxmax= pow(2, (int)ceil(log2(max)));
	int minmin= pow(2, (int)floor(log2(max)));
	if(maxmax<= bins)
		max = maxmax;
	else
		max = minmin;
	return max;
}


void TwoChoiceClient::writeToStash(int pss, int mpl, vector<prf_type> fileids, 
		unsigned char* key, vector<prf_type> &stashCiphers)
{
	for (unsigned int i = mpl; i < pss; i++) 
	{
		prf_type value;
		value = Utilities::encode(fileids[i].data(), key);
		stashCiphers.push_back(value);
	}
}


void TwoChoiceClient::writeToCuckooStash(vector<prf_type> fileids, int cnt, 
		int index, int tableNum, unsigned char* key)
{
	int entrySize = pow(2, tableNum);
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
		for(int i = fileids.size(); i<entrySize ; i++)
		ctCiphers.push_back(dummy);
	}
	server->insertCuckooStash(index, tableNum, ctCiphers);
}

void TwoChoiceClient::place(string keyword, vector<prf_type> fileids, int cuckooID, 
		int cnt, int index, int tableNum, unsigned char* key)
{
	if(cnt == (pow(2,index-tableNum))+1) // check this condition
	{
		cout <<"Cuckoo overflow: write in cuckooStash:"<<" index:"<<index<<" tableNum:"<<tableNum<<endl;
		writeToCuckooStash(fileids, cnt, index, tableNum, key);
		return;
	}
	int entrySize = pow(2, tableNum);
	int entryNum = pow(2,(index-tableNum));

	string temp = keyword;
	unsigned char cntstr[AES_KEY_SIZE];
	if(cuckooID == 0)
		temp = temp.append("0");
	else
		temp = temp.append("1");

	prf_type K = Utilities::encode(temp, key);
	memset(cntstr, 0, AES_KEY_SIZE);
	*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
	prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
	unsigned char* h = Utilities::sha256((char*) (mapKey.data()), AES_KEY_SIZE);
	int hash = (unsigned int) (*((int*) h)) % entryNum; 
	
	prf_type encKeyw = Utilities::encode(keyword, key);
	pair<prf_type ,vector<prf_type>> dis = server->insertCuckooHT(index, tableNum, cuckooID, hash,
									encKeyw,fileids);
	if(dis.first != nullKey)
	{
		string keyw = Utilities::decode(dis.first, key);
		place(keyw, dis.second, cnt+1, ((cuckooID+1)%2), index, tableNum, key);
	}
}
void TwoChoiceClient::writeToCuckooHT(int index, int size, string keyword, 
				vector<prf_type> fileids, unsigned char* key)
{
	assert(fileids.size() > 0);
	int tableNum = (int)ceil((float) log2(size));

	vector<prf_type> ctCiphers;
	for(int i = 0; i<fileids.size(); i++)
	{
		prf_type fid = Utilities::encode(fileids[i].data(), key);
		ctCiphers.push_back(fid);
	}
	ctCiphers.resize(size);
	if(fileids.size()<size)
	{
		prf_type dummy;
		memset(dummy.data(), 0, AES_KEY_SIZE);
		for(int i = fileids.size(); i<size ; i++)
		ctCiphers.push_back(dummy);
	}
	place(keyword, ctCiphers, 0, 0, index, tableNum, key);
}


void TwoChoiceClient::setup(int index, map<string, vector<prf_type> > pairs, unsigned char* key) 
{
	exist[index] = true;
	vector<vector<pair<prf_type, prf_type>>> ciphers;
	for (int i = 0; i < numberOfBins[index]; i++) 
		ciphers.push_back(vector<pair<prf_type,prf_type>>());
	
	vector<prf_type> stashCiphers; //for horizontal overflow
	map<prf_type, prf_type> keywordCntCiphers;
	map<int, int> fullness;

	vector<pair<string,vector<prf_type>>> sorted = sort(pairs);
	int mpl = maxPossibleLen((pow(2,index)),numberOfBins[index], index);
	mpl = numberOfBins[index]; // for now

	for (auto pair : sorted) 
	{
		int pss = pair.second.size();
		int newsize = pow(2, (int)ceil(log2(pss)));
		if(pss > mpl)
		{
			writeToStash(pss, mpl, pair.second, key, stashCiphers);
			pss = mpl;
			newsize = mpl;
		}
		if(newsize > mpl)
			newsize = mpl;

		prf_type K = Utilities::encode(pair.first, key);
		unsigned char cntstr[AES_KEY_SIZE];
		memset(cntstr, 0, AES_KEY_SIZE);
		*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
		prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
		prf_type valueTmp, totalTmp;
		*(int*) (&(valueTmp[0])) = newsize;
		//*(int*) (&(valueTmp[0])) = pair.second.size(); 
		prf_type mapValue = Utilities::encode(valueTmp.data(), K.data());
		keywordCntCiphers[mapKey] = mapValue; 
	   
		string temp = pair.first;
		temp = temp.append("1");
		prf_type K1 = Utilities::encode(temp, key);
		memset(cntstr, 0, AES_KEY_SIZE);
		*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
		prf_type mapKey1 = Utilities::generatePRF(cntstr, K1.data());
		unsigned char* hash1 = Utilities::sha256((char*) (mapKey1.data()), AES_KEY_SIZE);

		temp = pair.first;
		temp = temp.append("2");
		prf_type K2 = Utilities::encode(temp, key);
		memset(cntstr, 0, AES_KEY_SIZE);
		*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
		prf_type mapKey2 = Utilities::generatePRF(cntstr, K2.data());
		unsigned char* hash2 = Utilities::sha256((char*) (mapKey2.data()), AES_KEY_SIZE);

		int superBins = ceil((float) numberOfBins[index]/newsize); 
		int pos1 = (unsigned int) (*((int*) hash1)) % superBins;
		int pos2 = (unsigned int) (*((int*) hash2)) % superBins; 

		int totalItems1 = countTotal(fullness, pos1*newsize, newsize);
		int totalItems2 = countTotal(fullness, pos2*newsize, newsize);
			int cipherIndex ;
		if(totalItems1>totalItems2)
			cipherIndex = pos2*newsize;
		else
			cipherIndex = pos1*newsize;
		if(fullness[cipherIndex]<sizeOfEachBin[index])
		{
		   	for (unsigned int i = 0; i < pss; i++) 
		   	{
				unsigned char cntstr[AES_KEY_SIZE];
				memset(cntstr, 0, AES_KEY_SIZE);
				*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
				prf_type mapKey = Utilities::generatePRF(cntstr, K.data());
				prf_type mapValue = Utilities::encode(pair.second[i].data(), key);
				auto p = std::pair<prf_type, prf_type>(mapKey, mapValue);
				ciphers[cipherIndex].push_back(p);
				if(fullness.find(cipherIndex) == fullness.end())
					fullness[cipherIndex] = 1;
				else
						fullness[cipherIndex] = fullness[cipherIndex]+1;
				cipherIndex++;
			}
		   	for(int i = pss; i<newsize; i++)
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
			 writeToCuckooHT(index, newsize, pair.first, pair.second, key);
	}
	prf_type dummy;
	memset(dummy.data(), 0, AES_KEY_SIZE);
	auto dummypair = pair<prf_type, prf_type>(dummy, dummy);
	for (int i = 0; i < numberOfBins[index]; i++)  //filling up each bin to maximum capacity
	{
		int curSize = ciphers[i].size();
		for (int j = curSize; j < sizeOfEachBin[index]; j++) 
			ciphers[i].push_back(dummypair);
	}
	prf_type randomKey;
	for (int i = 0; i < AES_KEY_SIZE; i++) 
		randomKey[i] = rand();

	for (int i = keywordCntCiphers.size(); i < pow(2, index); i++) 
	{
		unsigned char cntstr[AES_KEY_SIZE];
		memset(cntstr, 0, AES_KEY_SIZE);
		*(int*) (&(cntstr[AES_KEY_SIZE - 9])) = rand();
		prf_type mapKey = Utilities::generatePRF(cntstr, randomKey.data());
		*(int*) (&(cntstr[AES_KEY_SIZE - 5])) = rand();
		prf_type mapValue = Utilities::generatePRF(cntstr, randomKey.data());
		keywordCntCiphers[mapKey] = mapValue;
	}
	totalCommunication += ciphers.size() * sizeof (prf_type)*2 + stashCiphers.size() * sizeof(prf_type);
	server->storeCiphers(index, ciphers, stashCiphers, keywordCntCiphers);
}

vector<prf_type> TwoChoiceClient::search(int index, string keyword, unsigned char* key) 
{
	double searchPreparation = 0, searchDecryption = 0;
	int flag = 0;
	if (profile) 
		Utilities::startTimer(65);
	vector<prf_type> finalRes;
	int keywordCnt = 0;
	prf_type hashtoken;
	prf_type token = Utilities::encode(keyword, key);
	vector<prf_type> ciphers;
	vector<prf_type> cuckooCiphers;
	vector<prf_type> stashCiphers;
	for(int s = 1 ;s<=2; s++)
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
		stashCiphers = server->getStash(index);
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
		if(flag !=0)//found in one superBin will imply NOT found in the other
		{
			for (auto item : stashCiphers) 
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
	int tableNum = (int)ceil(log2(keywordCnt));
	
	if(keywordCnt>0)
	{
		string newkeyword = keyword;
		newkeyword = newkeyword.append("0");
		prf_type hashtoken1 = Utilities::encode(newkeyword, key);
		newkeyword = keyword;
		newkeyword = newkeyword.append("1");
		prf_type hashtoken2 = Utilities::encode(newkeyword, key);
		cuckooCiphers = server->cuckooSearch(index, tableNum, hashtoken1, hashtoken2); // also searche suckoo stash
		if(cuckooCiphers.size()>0)
		{
			for (auto item : cuckooCiphers) 
			{
				prf_type plaintext;
				Utilities::decode(item, plaintext, key);
				if (strcmp((char*) plaintext.data(), keyword.data()) == 0) 
				{
						finalRes.push_back(plaintext);
				}
				cout <<"cuckoo data:"<<plaintext.data()<<endl;
			}
		}
		totalCommunication += cuckooCiphers.size()* sizeof(prf_type);
	}
	return finalRes;
}

vector<prf_type> TwoChoiceClient::getAllData(int index, unsigned char* key) 
{
	vector<prf_type> finalRes = vector<prf_type>();
	auto ciphers = server->getAllData(index);
	vector<prf_type> stashCiphers = server->getStash(index);
	vector<prf_type> cuckooCiphers = server->getCuckooHT(index); // also fetches the cuckoo stash

	for (auto cipher : ciphers) 
	{
		prf_type plaintext;
		Utilities::decode(cipher.second, plaintext, key);
		finalRes.push_back(plaintext);
	}
	if(stashCiphers.size()>0)
	{
		//cout <<"size of stash ciphers:"<<stashCiphers.size()<<endl;
		for(auto b : stashCiphers)
		{
	   		prf_type plaintext;
	   		Utilities::decode(b, plaintext, key);
		//cout <<"{"<<plaintext.data()<<"}"<<endl;
		finalRes.push_back(plaintext);
		}
	}
	if(cuckooCiphers.size()>0)
	{
		cout <<"getAllData:size of cuckoo ciphers:"<<cuckooCiphers.size()<<endl;
		for(auto b : cuckooCiphers)
		{
	   		prf_type plaintext;
	   		Utilities::decode(b, plaintext, key);
		finalRes.push_back(plaintext);
		}
	}
	totalCommunication += (ciphers.size() +  stashCiphers.size()+ cuckooCiphers.size())* sizeof (prf_type);
	return finalRes;
}

void TwoChoiceClient::destry(int index) 
{
	server->clear(index);
	exist[index] = false;
	totalCommunication += sizeof (int);
}
