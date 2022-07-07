#include "TwoChoiceStorage.h"
#include<string.h>
#include<assert.h>

TwoChoiceStorage::TwoChoiceStorage(bool inMemory, int dataIndex, string fileAddressPrefix, bool profile) 
{
    this->inMemoryStorage = inMemory;
    this->fileAddressPrefix = fileAddressPrefix;
    this->dataIndex = dataIndex;
    this->profile = profile;
    memset(nullKey.data(), 0, AES_KEY_SIZE);
    for (int i = 0; i < dataIndex; i++) 
    {
        int curNumberOfBins = 
	     i>3 ? ((int)ceil((float) (pow(2,i)/((log2(log2(pow(2,i))))*(log2(log2(log2(pow(2,i))))))))) : pow(2,i);
	//in twochoice they assume #of Bins in power of 2
	curNumberOfBins = pow(2, (int)ceil(log2(curNumberOfBins))); 
        int curSizeOfEachBin = i > 3 ? ceil(((log2(log2(pow(2,i))))*(log2(log2(log2(pow(2,i)))))*3)) : 3;
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
	cout<<"Level:"<<i<<" number of bins:"<<curNumberOfBins<<" size of each bin:"<<curSizeOfEachBin<<endl;
    }
}

bool TwoChoiceStorage::setup(bool overwrite) 
{
    if (inMemoryStorage) 
    {
        for (int i = 0; i < dataIndex; i++) 
	{
            vector<pair<prf_type, prf_type> > curData;
            data.push_back(curData);
        }
    } 
    else 
    {
	cuckoo1filenames.resize(dataIndex);
	cuckoo2filenames.resize(dataIndex);
	stashfilenames.resize(dataIndex);
        for (int i = 0; i < dataIndex; i++) 
	{
            string filename = fileAddressPrefix + "MAP-" + to_string(i) + ".dat";
            filenames.push_back(filename);
            fstream testfile(filename.c_str(), std::ofstream::in);
            if (testfile.fail() || overwrite) 
	    {
                testfile.close();
                fstream file(filename.c_str(), std::ofstream::out);
                if (file.fail()) 
	        {
                    cerr << "Error: " << strerror(errno);
                }
                int maxSize = numberOfBins[i] * sizeOfEachBin[i];
                for (int j = 0; j < maxSize; j++) 
	        {
                    file.write((char*) nullKey.data(), AES_KEY_SIZE);
                    file.write((char*) nullKey.data(), AES_KEY_SIZE);
                }
                file.close();
            }
	    
	    cuckoo1filenames[i].resize(dataIndex);
	    cuckoo2filenames[i].resize(dataIndex);
	    stashfilenames[i].resize(dataIndex);
	    for(int k = 0; k< dataIndex; k++) // k < log2(pow(2, dataIndex))
	    {
                string cuckoo1 = fileAddressPrefix + "CUCKOO1-" + to_string(i) +"-"+to_string(k)+ ".dat";
                string cuckoo2 = fileAddressPrefix + "CUCKOO2-" + to_string(i) +"-"+to_string(k)+ ".dat";
            	string stashfilename = fileAddressPrefix + "STASH-" + to_string(i)+"-"+to_string(k) + ".dat";
	        cuckoo1filenames[i][k]=cuckoo1;
	        cuckoo2filenames[i][k]=cuckoo2;
		stashfilenames[i][k]=stashfilename;

                fstream c1file(cuckoo1.c_str(), std::ofstream::in);
                if (c1file.fail() || overwrite) 
	        {
                    c1file.close();
                    fstream c11file(cuckoo1.c_str(), std::ofstream::out);
                    if (c11file.fail()) 
	            {
                        cerr << "Error: " << strerror(errno);
                    }
                    int maxSize = 2* pow(2,i);//numberOfBins[i] * sizeOfEachBin[i];
                	for (int j = 0; j < maxSize; j++) 
	        	{
	                        c11file.write((char*) nullKey.data(), AES_KEY_SIZE);
                	}
                    c11file.close();
                }
                fstream c2file(cuckoo2.c_str(), std::ofstream::in);
                if (c2file.fail() || overwrite) 
	        {
                    c2file.close();
                    fstream c22file(cuckoo2.c_str(), std::ofstream::out);
                    if (c22file.fail()) 
	            {
                        cerr << "Error: " << strerror(errno);
                    }
                    int maxSize = 2*pow(2,i);//numberOfBins[i] * sizeOfEachBin[i]; 
                    for (int j = 0; j < maxSize; j++) 
	            {
                        c22file.write((char*) nullKey.data(), AES_KEY_SIZE);
                    }
                    c22file.close();
                }
                fstream tfile(stashfilename.c_str(), std::ofstream::in);
                if (tfile.fail() || overwrite) 
	        {
                    tfile.close();
                    fstream sfile(stashfilename.c_str(), std::ofstream::out);
                    if (sfile.fail()) 
	            {
                        cerr << "Error: " << strerror(errno);
                    }
                    int maxSize = 2*pow(2,i);//numberOfBins[i] * sizeOfEachBin[i]; 
                    for (int j = 0; j < maxSize; j++) 
	            {
                        sfile.write((char*) nullKey.data(), AES_KEY_SIZE);
                    }
                    sfile.close();
                }
	    }
        }	
    }
}


pair<prf_type, vector<prf_type>> TwoChoiceStorage::insertCuckooHT(int index,int tableNum,int hash,int cuckooID, prf_type keyword, vector<prf_type> fileids)
{
	fstream cuckoo;
	if(cuckooID == 0)
      	    cuckoo.open(cuckoo1filenames[index][tableNum].c_str(), ios::binary | ios::out | ios::in);
      	  //cuckoo.open(cuckoo1filenames[index][tableNum].c_str(), ios::binary | ios::out|ios::in |ios::ate);
	else if(cuckooID == 1)
            cuckoo.open(cuckoo2filenames[index][tableNum].c_str(), ios::binary | ios::out | ios::in);

	vector<prf_type> results;
	prf_type keyw;
        if (cuckoo.fail()) 
	{
	    cout <<"ERROR IN FILE READ"<<endl;
            cerr << "Error in read: " << strerror(errno);
        }

	int entrySize = pow(2, tableNum);
        int readPos = hash *(entrySize+1)* AES_KEY_SIZE;
        cuckoo.seekg(readPos, ios::beg);
        SeekG++;
	
	char* oldKey = new char[AES_KEY_SIZE];
	cuckoo.read(oldKey,AES_KEY_SIZE);
	//cout <<"rp:"<<readPos<<" hash:"<<hash<<" entrySize:"<<entrySize<<endl;
	//cout <<"index:"<<index<<endl;
        copy(oldKey+0, oldKey+KEY_VALUE_SIZE, keyw.begin());
	delete oldKey;
        readBytes +=AES_KEY_SIZE ;

	results.resize(entrySize);
        int readLength = entrySize*AES_KEY_SIZE;
	//cout <<"["<<keyw.data()<<"]"<<" " <<"["<<nullKey.data()<<"]"<<endl;
        cuckoo.seekg(readPos+AES_KEY_SIZE, ios::beg);
	if(keyw != nullKey)
	{
            char* keyValues = new char[readLength];
            cuckoo.read(keyValues, readLength);
            readBytes += readLength;
            for (int i = 0; i < readLength /AES_KEY_SIZE ; i++) 
	    {
                prf_type tmp;
                std::copy(keyValues + i*AES_KEY_SIZE, keyValues + i * AES_KEY_SIZE, tmp.begin());
                //if (tmp != nullKey)
	        //{
                   results.push_back(tmp);
                //}
            }
	}
	else
	{
	    //cuckoo.clear();
            //cout << "you can insert in CT"<<endl;
            int readPos = hash *(entrySize+1)* AES_KEY_SIZE;
            cuckoo.seekg(readPos, ios::beg);
            SeekG++;
            unsigned char newRecord[KEY_VALUE_SIZE];
            memset(newRecord, 0, KEY_VALUE_SIZE);
            std::copy(keyword.begin(), keyword.end(), newRecord);
            cuckoo.write((char*) newRecord, AES_KEY_SIZE);
	    for(auto c : fileids)
	    {
                memset(newRecord, 0, KEY_VALUE_SIZE);
                std::copy(c.begin(), c.end(), newRecord);
                cuckoo.write((char*) newRecord, AES_KEY_SIZE);
	    }
	}
	cuckoo.close();
	auto result = make_pair(keyw , results);
	return result;
}

/*
void TwoChoiceStorage::insertStash(int index, vector<prf_type> ciphers) 
{
      fstream file(stashfilenames[index].c_str(), ios::binary | ios::out);
      if (file.fail()) 
      {
          cout <<"StashXX:"<<index<<endl;
          cerr << "(Error in Stash insert: " << strerror(errno)<<")"<<endl;
      }
      for (auto item : ciphers) 
      {
          unsigned char newRecord[AES_KEY_SIZE];
          memset(newRecord, 0, AES_KEY_SIZE);
          std::copy(item.begin(), item.end(), newRecord);
          file.write((char*) newRecord, AES_KEY_SIZE);
      }
      file.close();
}

vector<prf_type> TwoChoiceStorage::getStash(int index) 
{
      vector<prf_type> results;
      fstream file(stashfilenames[index].c_str(), ios::binary | ios::in | ios::ate);
      if (file.fail()) 
          cerr << "Error in read: " << strerror(errno);
      int size = file.tellg();
      file.seekg(0, ios::beg);
      char* keyValues = new char[size];
      file.read(keyValues, size);
      file.close();

      for (int i = 0; i < size/AES_KEY_SIZE ; i++) 
      {
          prf_type tmp;
          std::copy(keyValues+i*AES_KEY_SIZE, keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE, tmp.begin());
          if (tmp != nullKey) 
          {
              results.push_back(tmp);
          }
      }
      delete keyValues;
      return results;
}
*/

vector<prf_type> TwoChoiceStorage::getCuckooHT(int index) 
{
      vector<prf_type> results;
      for(int c = 0; c<2; c++ )
      {
	 for(int tn = 0; tn < index; tn++)
	 {
	    fstream cuckoo;
	    if(c == 0)
      	        cuckoo.open(cuckoo1filenames[index][tn].c_str(), ios::binary | ios::in | ios::ate);
	    else
      	        cuckoo.open(cuckoo2filenames[index][tn].c_str(), ios::binary | ios::in | ios::ate);
      	    if (cuckoo.fail()) 
           	cerr << "Error in read: " << strerror(errno);
	    int entryNum = pow(2, (index-tn));
	    int entrySize = pow(2, tn);
      	    int size = cuckoo.tellg();
            cuckoo.seekg(0, ios::beg);
            char* keyValues = new char[size];
      	    cuckoo.read(keyValues, size);
	    for(int e = 0; e< entryNum; e++)
	    {
		for(int es = 0; es<=entrySize; es++)
		{
		    prf_type entry;
          	    std::copy(keyValues+(e+1)*es*AES_KEY_SIZE, 
				keyValues+(e+1)*es*AES_KEY_SIZE+AES_KEY_SIZE, entry.begin());
		    if(es !=0 && entry != nullKey)
		    {
			results.push_back(entry);
		    } 
		    if(es == 0 && entry!=nullKey)
		    	cout <<"=================================["<<entry.data()<<"]"<<endl;
		}
	    }
      	    delete keyValues;
            cuckoo.close();
	 }
      }
      return results;
}



void TwoChoiceStorage::insertAll(int index, vector<vector< pair<prf_type, prf_type> > > ciphers) 
{
    if (inMemoryStorage) 
    {
        for (auto item : ciphers) 
	{
            data[index].insert(data[index].end(), item.begin(), item.end());
        }
    } 
    else 
    {
	    /*
        if (USE_XXL) {
            for (auto item : ciphers) {
                for (auto pair : item) {
                    diskData[index]->push_back(pair);
                }

            }
        } else {*/
            fstream file(filenames[index].c_str(), ios::binary | ios::out);
            if (file.fail()) 
	    {
		cout <<"XX:"<<index<<endl;
                cerr << "(Error in insert: " << strerror(errno)<<")"<<endl;
            }
            for (auto item : ciphers) 
	    {
                for (auto pair : item) 
		{
                    unsigned char newRecord[KEY_VALUE_SIZE];
                    memset(newRecord, 0, KEY_VALUE_SIZE);
                    std::copy(pair.first.begin(), pair.first.end(), newRecord);
                    std::copy(pair.second.begin(), pair.second.end(), newRecord + AES_KEY_SIZE);
                    file.write((char*) newRecord, 2 * AES_KEY_SIZE);
                }
            }
            file.close();
        //}
    }
}

vector<pair<prf_type, prf_type> > TwoChoiceStorage::getAllData(int index) 
{
    if (inMemoryStorage) 
    {
        vector<pair<prf_type, prf_type> > results;
        for (int i = 0; i < data[index].size(); i++) 
	{
            if (data[index][i].first != nullKey) 
                results.push_back(data[index][i]);
        }
        return results;
    } 
    else 
    {
            vector<pair<prf_type, prf_type> > results;
            fstream file(filenames[index].c_str(), ios::binary | ios::in | ios::ate);
            if (file.fail()) {
                cerr << "Error in read: " << strerror(errno);
            }
            int size = file.tellg();
            file.seekg(0, ios::beg);
            char* keyValues = new char[size];
            file.read(keyValues, size);
            file.close();

            for (int i = 0; i < size / KEY_VALUE_SIZE; i++) 
	    {
                prf_type tmp, restmp;
                std::copy(keyValues + i*KEY_VALUE_SIZE, keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, tmp.begin());
                std::copy(keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE + AES_KEY_SIZE, restmp.begin());
                if (tmp != nullKey) //dummy added to fillup bins 
		{
                    results.push_back(pair<prf_type, prf_type>(tmp, restmp));
                }
            }
            delete keyValues;
            return results;
        //}
    }
}

void TwoChoiceStorage::clear(int index) 
{
    if (inMemoryStorage) 
    {
        data[index].clear();
    } 
    else {
            fstream file(filenames[index].c_str(), std::ios::binary | std::ofstream::out);
            if (file.fail()) 
                cerr << "Error: " << strerror(errno);
            int maxSize = numberOfBins[index] * sizeOfEachBin[index];
            for (int j = 0; j < maxSize; j++) 
	    {
                file.write((char*) nullKey.data(), AES_KEY_SIZE);
                file.write((char*) nullKey.data(), AES_KEY_SIZE);
            }
            file.close();

 	    for(int k=0; k<index; k++)
	    {
                fstream sfile(stashfilenames[index][k].c_str(), std::ios::binary | std::ofstream::out);
                if (sfile.fail()) 
                    cerr << "Error: " << strerror(errno);
            	int maxSize = 2*pow(2,index);
                for (int j = 0; j < maxSize; j++) 
                    sfile.write((char*) nullKey.data(), AES_KEY_SIZE);
                sfile.close();
	    }
 	    for(int k=0; k<index; k++)
	    {
                fstream sfile(cuckoo1filenames[index][k].c_str(), std::ios::binary | std::ofstream::out);
                if (sfile.fail()) 
                    cerr << "Error: " << strerror(errno);
            	int maxSize = 2*pow(2,index);
                for (int j = 0; j < maxSize; j++) 
                    sfile.write((char*) nullKey.data(), AES_KEY_SIZE);
                sfile.close();
	    }
 	    for(int k=0; k<index; k++)
	    {
                fstream sfile(cuckoo2filenames[index][k].c_str(), std::ios::binary | std::ofstream::out);
                if (sfile.fail()) 
                    cerr << "Error: " << strerror(errno);
            	int maxSize = 2*pow(2,index);
                for (int j = 0; j < maxSize; j++) 
                    sfile.write((char*) nullKey.data(), AES_KEY_SIZE);
                sfile.close();
	    }
    }
}

TwoChoiceStorage::~TwoChoiceStorage() {
}

vector <prf_type> TwoChoiceStorage::cuckooSearch(int index, int tableNum, int h1, int h2)
{
    vector<prf_type> results;
    
    std::fstream cuckoo1(cuckoo1filenames[index][tableNum].c_str(), ios::binary | ios::in | ios::ate);
    if (cuckoo1.fail()) 
    {
        cerr << "Error in read: " << strerror(errno);
    }
    
    int size = cuckoo1.tellg();
    int entrySize = pow(2, tableNum);
    int readPos = h1*entrySize*AES_KEY_SIZE;
    readPos = readPos+ AES_KEY_SIZE;
    int readLength = entrySize*AES_KEY_SIZE;
    cuckoo1.seekg(readPos,ios::beg);
    SeekG++;
    char* keyValues = new char[readLength];
    cuckoo1.read(keyValues, readLength);
    readBytes += readLength;
    
    
    for (int i = 0; i < readLength / AES_KEY_SIZE; i++) 
    {
        prf_type tmp;
	assert(readPos+i*AES_KEY_SIZE <size);
        std::copy(keyValues+i*AES_KEY_SIZE, keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE, tmp.begin());
        if (tmp != nullKey) 
	{
            results.push_back(tmp);
		cout <<"SIZE2:"<<results.size()<<endl;
        }
    }
    cuckoo1.close();
    
    delete keyValues;
    
    std::fstream cuckoo2(cuckoo2filenames[index][tableNum].c_str(), ios::binary | ios::in | ios:: ate);
    if (cuckoo2.fail()) 
    {
	    cout <<"FAIL FAIL FAIL:"<<endl;
        cerr << "Error in read: " << strerror(errno);
    }

    size = cuckoo2.tellg();
    readPos = h2*entrySize*AES_KEY_SIZE;
    readPos = readPos+ AES_KEY_SIZE;
    readLength = entrySize*AES_KEY_SIZE;
    cuckoo2.seekg(readPos,ios::beg);
    SeekG++;
    char* keyValue = new char[readLength];
    cuckoo2.read(keyValue, readLength);
    readBytes += readLength;
    for (int i = 0; i < readLength / AES_KEY_SIZE; i++) 
    {
        prf_type tmp;
	assert(readPos+i*AES_KEY_SIZE <size);
        std::copy(keyValue + i * AES_KEY_SIZE, keyValue + i*AES_KEY_SIZE+AES_KEY_SIZE, tmp.begin());
        if (tmp != nullKey) 
	{
		cout <<"SIZE2:"<<results.size()<<endl;
            results.push_back(tmp);
        }
    }
    delete keyValue;
    cuckoo2.close();
    
    cout <<"results size:"<<results.size()<<endl;
    return results;
}
vector<prf_type> TwoChoiceStorage::find(int index, prf_type mapKey, int cnt) 
{
	/*
    if (inMemoryStorage) 
    {
        vector<prf_type> results;

        unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
        if (cnt >= numberOfBins[index]) {
            for (int i = 0; i < numberOfBins[index] * sizeOfEachBin[index]; i++) 
	    {
                if (data[index][i].first != nullKey) 
		{
                    results.push_back(data[index][i].second);
                }
            }
        } 
	else 
	{
            int pos = (unsigned int) (*((int*) hash)) % numberOfBins[index];
            int readPos = pos * sizeOfEachBin[index];
            int fileLength = numberOfBins[index] * sizeOfEachBin[index];
            int remainder = fileLength - readPos;
            int totalReadLength = cnt * sizeOfEachBin[index];
            int readLength = 0;
            if (totalReadLength > remainder) 
	    {
                readLength = remainder;
                totalReadLength -= remainder;
            } else {
                readLength = totalReadLength;
                totalReadLength = 0;
            }
            for (int i = 0; i < readLength; i++) {
                if (data[index][i + readPos].first != nullKey) {
                    results.push_back(data[index][i + readPos].second);
                }
            }
            if (totalReadLength > 0) {
                readLength = totalReadLength;
                for (int i = 0; i < readLength; i++) {
                    if (data[index][i].first != nullKey) {
                        results.push_back(data[index][i].second);
                    }
                }
            }
        }
        return results;
    } else {
        /*if (USE_XXL) {
            vector<prf_type> results;

            unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
            if (cnt >= numberOfBins[index]) {
                for (int i = 0; i < numberOfBins[index] * sizeOfEachBin[index]; i++) {
                    if (diskData[index]->at(i).first != nullKey) {
                        results.push_back(diskData[index]->at(i).second);
                    }
                }
            } else {
                int pos = (unsigned int) (*((int*) hash)) % numberOfBins[index];
                int readPos = pos * sizeOfEachBin[index];
                int fileLength = numberOfBins[index] * sizeOfEachBin[index];
                int remainder = fileLength - readPos;
                int totalReadLength = cnt * sizeOfEachBin[index];
                int readLength = 0;
                if (totalReadLength > remainder) {
                    readLength = remainder;
                    totalReadLength -= remainder;
                } else {
                    readLength = totalReadLength;
                    totalReadLength = 0;
                }
                for (int i = 0; i < readLength; i++) {
                    if (diskData[index]->at(i + readPos).first != nullKey) {
                        results.push_back(diskData[index]->at(i + readPos).second);
                    }
                }
                if (totalReadLength > 0) {
                    readLength = totalReadLength;
                    for (int i = 0; i < readLength; i++) {
                        if (diskData[index]->at(i).first != nullKey) {
                            results.push_back(diskData[index]->at(i).second);
                        }
                    }
                }
            }
            return results;
        } else {*/
            vector<prf_type> results;

            std::fstream file(filenames[index].c_str(), ios::binary | ios::in);
            if (file.fail()) 
	    {
                //cerr << "Error in read: " << strerror(errno);
            }
            unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
            if (cnt >= numberOfBins[index]) 
	    {
                //read everything
                int fileLength = numberOfBins[index] * sizeOfEachBin[index] * KEY_VALUE_SIZE;
                char* keyValues = new char[fileLength];
                file.read(keyValues, fileLength);
                SeekG++;
                readBytes += fileLength;
                for (int i = 0; i < numberOfBins[index] * sizeOfEachBin[index]; i++) {
                    prf_type tmp, restmp;
                    //                    std::copy(keyValues + i*KEY_VALUE_SIZE, keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, tmp.begin());
                    std::copy(keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, keyValues + i * KEY_VALUE_SIZE + (2 * AES_KEY_SIZE), restmp.begin());
                    if (restmp != nullKey) {
                        results.push_back(restmp);
                    }
                }
            } 
	    else 
	    {
        	int superBins = ceil((float) numberOfBins[index]/cnt); 
                int pos = (unsigned int) (*((int*) hash)) % superBins; //numberOfBins[index];
                int readPos = pos *cnt* KEY_VALUE_SIZE * sizeOfEachBin[index];
                int fileLength = numberOfBins[index] * sizeOfEachBin[index] * KEY_VALUE_SIZE;
                int remainder = fileLength - readPos;
                int totalReadLength = cnt * KEY_VALUE_SIZE * sizeOfEachBin[index];
                int readLength = 0;
                if (totalReadLength > remainder) 
		{
                    readLength = remainder;
                    totalReadLength -= remainder;
                } 
		else 
		{
                    readLength = totalReadLength;
                    totalReadLength = 0;
                }
                file.seekg(readPos, ios::beg);
                SeekG++;
                char* keyValues = new char[readLength];
                file.read(keyValues, readLength);
                readBytes += readLength;
                for (int i = 0; i < readLength / KEY_VALUE_SIZE; i++) {
                    prf_type tmp, restmp;
                    //                    std::copy(keyValues + i*KEY_VALUE_SIZE, keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, tmp.begin());
                    std::copy(keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, keyValues + i * KEY_VALUE_SIZE + (2 * AES_KEY_SIZE), restmp.begin());
                    if (restmp != nullKey) {
                        results.push_back(restmp);
                    }
                }
                if (totalReadLength > 0) {
                    readLength = totalReadLength;
                    file.seekg(0, ios::beg);
                    char* keyValues = new char[readLength];
                    file.read(keyValues, readLength);
                    readBytes += readLength;
                    SeekG++;
                    for (int i = 0; i < readLength / KEY_VALUE_SIZE; i++) {
                        prf_type tmp, restmp;
                        //                        std::copy(keyValues + i*KEY_VALUE_SIZE, keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, tmp.begin());
                        std::copy(keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, keyValues + i * KEY_VALUE_SIZE + (2 * AES_KEY_SIZE), restmp.begin());
                        if (restmp != nullKey) {
                            results.push_back(restmp);
                        }
                    }
                }
            }
            file.close();
            return results;
        //}
    //}
}


