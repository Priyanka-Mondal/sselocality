#include "TwoChoiceStorage.h"
#include<string.h>

TwoChoiceStorage::TwoChoiceStorage(bool inMemory, long dataIndex, string fileAddressPrefix, bool profile) 
{
    this->inMemoryStorage = inMemory;
    this->fileAddressPrefix = fileAddressPrefix;
    this->dataIndex = dataIndex;
    this->profile = profile;
    memset(nullKey.data(), 0, AES_KEY_SIZE);
    for (long i = 0; i < dataIndex; i++) 
    {
        long curNumberOfBins = i > 3 ? ((long) ceil((float) pow(2, i) / ((log2(log2(pow(2,i))))*(log2(log2(log2(pow(2,i)))))*(log2(log2(log2(pow(2,i)))))))) : pow(2,i);
	curNumberOfBins = pow(2, (long)ceil(log2(curNumberOfBins))); 
     	long curSizeOfEachBin = i > 3 ? ceil(SO*(log2(log2(pow(2,i))))*(log2(log2(log2(pow(2,i)))))*(log2(log2(log2(pow(2,i)))))) : SO;
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
        printf("Level:%d number of Bins:%d size of bin:%d\n", i, curNumberOfBins, curSizeOfEachBin);
    }
}

bool TwoChoiceStorage::setup(bool overwrite) 
{
    for (long i = 0; i < dataIndex; i++) 
	{
        string filename = fileAddressPrefix + "MAP-" + to_string(i) + ".dat";
        filenames.push_back(filename);
        fstream testfile(filename.c_str(), std::ofstream::in);
        if (testfile.fail() || overwrite) 
		{
            testfile.close();
            fstream file(filename.c_str(), std::ofstream::out);
            if (file.fail()) 
                cerr << "Error: " << strerror(errno);
            long maxSize = numberOfBins[i] * sizeOfEachBin[i];
            for (long j = 0; j < maxSize; j++) 
    		{
                file.write((char*) nullKey.data(), AES_KEY_SIZE);
            }
            file.close();
        }
    }
}



void TwoChoiceStorage::insertAll(long index, vector<vector< prf_type > > ciphers) 
{
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
            unsigned char newRecord[AES_KEY_SIZE];
            memset(newRecord, 0, AES_KEY_SIZE);
            std::copy(pair.begin(), pair.end(), newRecord);
            file.write((char*) newRecord, AES_KEY_SIZE);
        }
    }
    file.close();
}

vector<prf_type> TwoChoiceStorage::getAllData(long index) 
{
    vector<prf_type > results;
    fstream file(filenames[index].c_str(), ios::binary | ios::in | ios::ate);
    if (file.fail()) {
        cerr << "Error in read: " << strerror(errno);
    }
    long size = file.tellg();
    file.seekg(0, ios::beg);
    char* keyValues = new char[size];
    file.read(keyValues, size);
    file.close();
    for (long i = 0; i < size / AES_KEY_SIZE; i++) 
    {
        prf_type tmp;
        std::copy(keyValues + i*AES_KEY_SIZE, keyValues + i * AES_KEY_SIZE + AES_KEY_SIZE, tmp.begin());
        if (tmp != nullKey) //dummy added to fillup bins 
    	   {
               results.push_back(tmp);
        }
    }
    delete keyValues;
    return results;
}

void TwoChoiceStorage::clear(long index) 
{
	fstream file(filenames[index].c_str(), std::ios::binary | std::ofstream::out);
	if (file.fail()) 
    	cerr << "Error: " << strerror(errno);
	long maxSize = numberOfBins[index] * sizeOfEachBin[index];
	for (long j = 0; j < maxSize; j++) 
	{
		file.write((char*) nullKey.data(), AES_KEY_SIZE);
	}
	file.close();
}

TwoChoiceStorage::~TwoChoiceStorage() {
}

vector<prf_type> TwoChoiceStorage::find(long index, prf_type mapKey, long cnt) 
{
    vector<prf_type> results;
    std::fstream file(filenames[index].c_str(), ios::binary | ios::in);
    if (file.fail()) 
        cerr << "Error in read: " << strerror(errno);
    unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
    if (cnt >= numberOfBins[index]) 
	{
        long fileLength = numberOfBins[index] * sizeOfEachBin[index] * AES_KEY_SIZE;
        char* keyValues = new char[fileLength];
        file.read(keyValues, fileLength);
        SeekG++;
        readBytes += fileLength;
        for (long i = 0; i < numberOfBins[index] * sizeOfEachBin[index]; i++) 
		{
            prf_type restmp;
            std::copy(keyValues+i*AES_KEY_SIZE, keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE,restmp.begin());
            if (restmp != nullKey) {
                results.push_back(restmp);
            }
        }
    } 
	else 
	{
		long superBins = ceil((float) numberOfBins[index]/cnt); 
        long pos = (unsigned long) (*((long*) hash)) % superBins; //numberOfBins[index];
        long readPos = pos *cnt* AES_KEY_SIZE * sizeOfEachBin[index];
        long fileLength = numberOfBins[index] * sizeOfEachBin[index] * AES_KEY_SIZE;
        long remainder = fileLength - readPos;
        long totalReadLength = cnt * AES_KEY_SIZE * sizeOfEachBin[index];
        long readLength = 0;
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
        for (long i = 0; i < readLength / AES_KEY_SIZE; i++) 
		{
            prf_type restmp;
            std::copy(keyValues + i * AES_KEY_SIZE , keyValues + i * AES_KEY_SIZE + AES_KEY_SIZE, restmp.begin());
            if (restmp != nullKey) 
                results.push_back(restmp);
            
        }
        if (totalReadLength > 0) 
		{
            readLength = totalReadLength;
            file.seekg(0, ios::beg);
            char* keyValues = new char[readLength];
            file.read(keyValues, readLength);
            readBytes += readLength;
            SeekG++;
            for (long i = 0; i < readLength / AES_KEY_SIZE; i++) 
			{
                prf_type restmp;
                std::copy(keyValues + i * AES_KEY_SIZE , keyValues + i * AES_KEY_SIZE + AES_KEY_SIZE, restmp.begin());
                if (restmp != nullKey) 
                    results.push_back(restmp);
            }
        }
    }
    file.close();
    return results;
}
