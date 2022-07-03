#include "TwoChoiceClient.h"
#include<vector>
#include<algorithm>

TwoChoiceClient::~TwoChoiceClient() 
{
    delete server;
}

TwoChoiceClient::TwoChoiceClient(int numOfDataSets, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
    cout <<"===================RUNNING SDa+TwoChoice====================="<<endl;
    server = new TwoChoiceServer(numOfDataSets, inMemory, overwrite, profile);
    for (int i = 0; i < numOfDataSets; i++) //why not <=
    {
        exist.push_back(false);
        int curNumberOfBins = i > 3 ? ((int) ceil((float) pow(2, i) / (log2(log2(pow(2,i)))*(log2(log2(log2(pow(2,i)))))))) : pow(2,i);
	//in twochoice they assume #of Bins in power of 2
	curNumberOfBins = pow(2, (int)ceil(log2(curNumberOfBins)));
     int curSizeOfEachBin = i > 3 ? ceil((log2(log2(pow(2,i)))*(log2(log2(log2(pow(2,i)))))*3)) : 3;
	//cout <<"cur num of bins:"<< curNumberOfBins<<"/"<<curSizeOfEachBin<<endl;
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
	//if(a.first=="") cout <<"a is empty"<<endl;
	//if(b.first=="") cout <<"b is empty"<<endl;
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
//cout<<"index:"<<index <<" bins:"<<bins<<" N:"<< N<<" max:"<< max<<"("<<maxmax<<","<<minmin<<"):"<<endl;
	return max;
}


void TwoChoiceClient::writeToStash(int pss, int mpl, int index, string keyword, vector<prf_type> fileids, unsigned char* key,vector<prf_type> &stashCiphers )
{
        prf_type K = Utilities::encode(keyword, key);
	for (unsigned int i = mpl; i < pss; i++) 
	{
            prf_type mapKey, mapValue;
            unsigned char cntstr[AES_KEY_SIZE];
            memset(cntstr, 0, AES_KEY_SIZE);
            *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
            mapKey = Utilities::generatePRF(cntstr, K.data());
            mapValue = Utilities::encode(fileids[i].data(), key);
            stashCiphers.push_back(mapValue);
	}
}


void TwoChoiceClient::truncateToMpl(int pss, int mpl, int index, string keyword, vector<prf_type> fileids, unsigned char* key)
{
    if(keyword != "") //not required
    {
	cout <<"ADDING TO STASH++"<<endl;
	map<string,vector<prf_type>> indexstash;
	if(stash.count(index)>0)
		indexstash = stash[index];
	vector<prf_type> trunc = vector < prf_type>();
	for(int k=mpl; k<pss; k++)
	{
            //prf_type mapValue;
            //mapValue = Utilities::encode(fileids[k].data(), key);
	    //trunc.push_back(mapValue);
	    //cout <<"fileids[k]:"<<fileids[k].data()<<endl;
		trunc.push_back(fileids[k]);
	}
	if(indexstash.count(keyword)==0)
		 indexstash[keyword] = vector < prf_type>();
	indexstash[keyword]= trunc;
	stash[index]=indexstash;
    }
}

void TwoChoiceClient::setup(int index, map<string, vector<prf_type> > pairs, unsigned char* key) 
{
    exist[index] = true;
    vector<vector<pair<prf_type, prf_type> > > ciphers;
    vector<prf_type> stashCiphers;

    for (int i = 0; i < numberOfBins[index]; i++) 
    {
        ciphers.push_back(vector<pair<prf_type,prf_type>>());
    }
    map<prf_type, prf_type> keywordCntCiphers;
    map<prf_type, prf_type> stashCntCiphers;
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
		truncateToMpl(pss,mpl,index,pair.first,pair.second,key); // stores extra elems in stash
		writeToStash(pss,mpl,index,pair.first,pair.second,key, stashCiphers);
		pss = mpl;
		newsize = mpl;
	}
	if(newsize > mpl)
		newsize = mpl;

        prf_type K = Utilities::encode(pair.first, key);
	prf_type mapKey, mapValue;
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        mapKey = Utilities::generatePRF(cntstr, K.data());
        prf_type valueTmp, totalTmp;
        *(int*) (&(valueTmp[0])) = newsize;//pair.second.size(); //already resized to nearPow2
        mapValue = Utilities::encode(valueTmp.data(), K.data());
        keywordCntCiphers[mapKey] = mapValue; 
       
        if(pair.second.size()>newsize)
	{	
		*(int*) (&(totalTmp[0])) = pair.second.size(); 
        	mapValue = Utilities::encode(totalTmp.data(), K.data());
        	stashCntCiphers[mapKey] = mapValue; 
	}



/////////////////////////////////////////////////////////////////////////	
	auto count = counters[index];				       //	
	count[pair.first] = make_pair(newsize,pair.second.size());     //
	counters[index]=count;                                         //
/////////////////////////////////////////////////////////////////////////



        string temp = pair.first;
	temp = temp.append("1");
        prf_type K1 = Utilities::encode(temp, key);
        prf_type mapKey1;
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        mapKey1 = Utilities::generatePRF(cntstr, K1.data());
        unsigned char* hash1 = Utilities::sha256((char*) (mapKey1.data()), AES_KEY_SIZE);

	temp = pair.first;
	temp = temp.append("2");
        prf_type K2 = Utilities::encode(temp, key);
        prf_type mapKey2;
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
        mapKey2 = Utilities::generatePRF(cntstr, K2.data());
        unsigned char* hash2 = Utilities::sha256((char*) (mapKey2.data()), AES_KEY_SIZE);

        int superBins = ceil((float) numberOfBins[index]/newsize); 
        int pos1 = (unsigned int) (*((int*) hash1)) % superBins;
        int pos2 = (unsigned int) (*((int*) hash2)) % superBins; 

	int totalItems1 = countTotal(fullness, pos1*newsize, newsize);
	int totalItems2 = countTotal(fullness, pos2*newsize, newsize);
	//assert(totalItems1 == totalItems2);
        int cipherIndex ;
	if(totalItems1>totalItems2)
		cipherIndex = pos2*newsize;
	else
		cipherIndex = pos1*newsize;
        
	for (unsigned int i = 0; i < pss; i++) 
	{
            prf_type mapKey, mapValue;
            unsigned char cntstr[AES_KEY_SIZE];
            memset(cntstr, 0, AES_KEY_SIZE);
            *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = i;
            mapKey = Utilities::generatePRF(cntstr, K.data());
            mapValue = Utilities::encode(pair.second[i].data(), key);

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
    prf_type dummy;
    memset(dummy.data(), 0, AES_KEY_SIZE);
    auto dummypair = pair<prf_type, prf_type>(dummy, dummy);
    for (int i = 0; i < numberOfBins[index]; i++)  //filling up each bin to maximum capacity
    {
        int curSize = ciphers[i].size();
        for (int j = curSize; j < sizeOfEachBin[index]; j++) 
	{
            ciphers[i].push_back(dummypair);
        }
    }

    prf_type randomKey;
    for (int i = 0; i < AES_KEY_SIZE; i++) 
        randomKey[i] = rand();

    for (int i = keywordCntCiphers.size(); i < pow(2, index); i++)  // what is this loop doing ?
    {
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 9])) = rand();
        prf_type mapKey = Utilities::generatePRF(cntstr, randomKey.data());
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = rand();
        prf_type mapValue = Utilities::generatePRF(cntstr, randomKey.data());
        keywordCntCiphers[mapKey] = mapValue;
    }
    for (int i = stashCntCiphers.size(); i < pow(2, index); i++)  // what is this loop doing ?
    {
        unsigned char cntstr[AES_KEY_SIZE];
        memset(cntstr, 0, AES_KEY_SIZE);
        *(int*) (&(cntstr[AES_KEY_SIZE - 9])) = rand();
        prf_type mapKey = Utilities::generatePRF(cntstr, randomKey.data());
        *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = rand();
        prf_type mapValue = Utilities::generatePRF(cntstr, randomKey.data());
        stashCntCiphers[mapKey] = mapValue;
    }
    //    totalCommunication += ciphers.size() * sizeof (prf_type)*2;
    
    server->storeCiphers(index, ciphers, stashCiphers, keywordCntCiphers,stashCntCiphers);
}

vector<prf_type> TwoChoiceClient::search(int index, string keyword, unsigned char* key) 
{
    double searchPreparation = 0, searchDecryption = 0;
    int flag = 0;
    if (profile) 
        Utilities::startTimer(65);
    vector<prf_type> finalRes;
    auto count = counters[index][keyword];
    int keywordCnt = 0;
    int total = count.second;
    prf_type hashtoken;
    prf_type token = Utilities::encode(keyword, key);
    vector<prf_type> ciphers;
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
        ciphers = server->search(index, token, hashtoken, keywordCnt);
        stashCiphers = server->stashSearch(index, token, keywordCnt);
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
       	}
    }
    return finalRes;
}

/*
vector<prf_type> TwoChoiceClient::newsearch(int index, string keyword, unsigned char* key) 
{
    double searchPreparation = 0, searchDecryption = 0;
    int flag = 0;
    if (profile) 
        Utilities::startTimer(65);

    vector<prf_type> finalRes;
    auto count = counters[index][keyword];
    int keywordCnt = count.first;
    int total = count.second;
    prf_type hashtoken;
    prf_type token = Utilities::encode(keyword, key);
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
	int po =0;	
	if(keywordCnt != 0)
	{
        	vector<prf_type> ciphers = server->newsearch(index, hashtoken, keywordCnt,po);
        	int cnt = 0;
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
	}
    }
    if(total>keywordCnt && stash.count(index)>0)
    {
	map<string, vector<prf_type>> srch = stash[index];
	if(srch.count(keyword)>0)
	{

		cout <<"Searching from stash.. Aborting!!:"<<srch[keyword].size()<<endl;
		vector<prf_type> fileids = srch[keyword];
		for(auto a : fileids)
		{

			prf_type plaintext;
			//Utilities::decode(a, plaintext, key);
			//somemore decode ?;
			finalRes.push_back(plaintext);
		}
	//cout <<"size of finalRes after adding from:"<<finalRes.size()<<endl<<endl;
	}
    }
    	return finalRes;
}
*/

vector<prf_type> TwoChoiceClient::getAllData(int index, unsigned char* key) 
{
    vector<prf_type> finalRes = vector<prf_type>();
    auto ciphers = server->getAllData(index);
    for (auto cipher : ciphers) 
    {
        prf_type plaintext;
        Utilities::decode(cipher.second, plaintext, key);
        finalRes.push_back(plaintext);
    }
    if(stash.count(index)>0)
    {
	    cout <<"getAllData: Adding from stash"<<endl;
    	map<string, vector<prf_type>> temp = stash[index];
    	for(auto a : temp)
    	{
	    assert(a.first != "");
	    for(auto b : a.second)
	    {
        	//prf_type plaintext;
        	//Utilities::decode(b, plaintext, key);
        	//finalRes.push_back(plaintext);
		//cout <<"b.data():["<<b.data()<<"]"<<endl;
		finalRes.push_back(b);
	    }
    	}
    }
    totalCommunication += ciphers.size() * sizeof (prf_type);
    return finalRes;
}

void TwoChoiceClient::destry(int index) 
{
    server->clear(index);
    exist[index] = false;
    stash.erase(index);
    counters.erase(index);
    totalCommunication += sizeof (int);
}
