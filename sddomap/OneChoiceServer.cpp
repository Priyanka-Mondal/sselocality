#include "OneChoiceServer.h"
#include <string.h>

OneChoiceServer::OneChoiceServer(int dataIndex, vector<OMAP*> omaps, bool inMemory, bool overwrite, bool profile) 
{
    this->profile = profile;
    storage = new OneChoiceStorage(inMemory, dataIndex, "/tmp/", profile);
    storage->setup(overwrite);
    keyworkCounters = new Storage(inMemory, dataIndex, "/tmp/keyword-", profile);
    keyworkCounters->setup(overwrite);
	for(int n=0; n<=dataIndex; n++)
    	NEW.push_back(vector<prf_type>());
}

OneChoiceServer::~OneChoiceServer() {}


/*
void OneChoiceServer::storeCiphers(int dataIndex, int instance, vector<vector<pair<prf_type, prf_type> > > ciphers, map<prf_type, prf_type> keywordCounters) {
    storage->insertAll(dataIndex, instance, ciphers);
    keyworkCounters->insert(dataIndex, keywordCounters);
}
*/
vector<prf_type> OneChoiceServer::search(int dataIndex, int instance, int bin) 
{
    vector<prf_type> result;
	/*
    keyworkCounters->seekgCount = 0;
    storage->readBytes = 0;
    double keywordCounterTime = 0, serverSearchTime = 0;
    if (profile) {
        Utilities::startTimer(35);
    }
    prf_type curToken = token;
    unsigned char cntstr[AES_KEY_SIZE];
    memset(cntstr, 0, AES_KEY_SIZE);
    *(int*) (&(cntstr[AES_KEY_SIZE - 5])) = -1;
    prf_type keywordMapKey = Utilities::generatePRF(cntstr, curToken.data());
    bool found = false;
    prf_type res = keyworkCounters->find(dataIndex, keywordMapKey, found);
    if (profile) 
	{
        keywordCounterTime = Utilities::stopTimer(35);
        cout<<"keyword counter Search Time:"<<keywordCounterTime<<endl;
	   	cout<<"number of SeekG:"<<keyworkCounters->seekgCount<<endl;
	   	cout<<"number of read bytes:"<<keyworkCounters->KEY_VALUE_SIZE * keyworkCounters->seekgCount<<endl;
        Utilities::startTimer(45);
    }
    if (found) 
	{
        prf_type plaintext;
        Utilities::decode(res, plaintext, curToken.data());
        keywordCnt = *(int*) (&(plaintext[0]));
        result = storage->find(dataIndex, keywordMapKey, keywordCnt);
        if (profile) 
		{
            serverSearchTime = Utilities::stopTimer(45);
            cout<<"server Search Time:"<<serverSearchTime <<endl;
			cout<<"number of SeekG:"<<storage->SeekG<<" number of read bytes:"<<storage->readBytes<<endl;
        }
    }
	*/
    result = storage->searchBin(dataIndex, instance, bin);
    return result;
}

vector<prf_type> OneChoiceServer::getAllData(int dataIndex, int instance) 
{
    return storage->getAllData(dataIndex, instance);
}

void OneChoiceServer::clear(int index, int instance) 
{
    storage->clear(index, instance);
    keyworkCounters->clear(index);
}

void OneChoiceServer::move(int index, int toInstance, int fromInstance)
{
	vector<prf_type> data;
   	data = storage->getAllData(index, fromInstance);
	storage->insertAll(index, toInstance, data);
}

void OneChoiceServer::copy(int index, int toInstance)
{
	//cout <<"NEW[index].size():"<<NEW[index].size()<<endl;
	storage->insertAll(index, toInstance, NEW[index]);
	NEW[index].resize(0);
}

void OneChoiceServer::append(int index, prf_type keyVal)
{
	NEW[index].push_back(keyVal);
}

void OneChoiceServer::destroy(int index, int instance)
{
	clear(index,instance);
}

void OneChoiceServer::resize(int index, int size)
{
	NEW[index].resize(size);
}

vector<prf_type> OneChoiceServer::getElements(int index, int instance, int start, int end)
{
	return storage->getElements(index, instance, start, end);
}
vector< prf_type> OneChoiceServer::getNEW(int index)
{
	return NEW[index];
}
void OneChoiceServer::putNEW(int index, vector<prf_type> sorted)
{
	NEW[index].resize(0);
	NEW[index].resize(sorted.size());
	for(auto n : sorted)
	{
		NEW[index].push_back(n);
	}
}


void OneChoiceServer::bitonicSort(int index)
{
	//do the actual sort here
	//but get the elements from the server
}
