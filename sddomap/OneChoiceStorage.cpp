#include "OneChoiceStorage.h"
#include<assert.h>
#include<string.h>

OneChoiceStorage::OneChoiceStorage(bool inMemory, int dataIndex, string fileAddressPrefix, bool profile) 
{
    this->inMemoryStorage = inMemory;
    this->fileAddressPrefix = fileAddressPrefix;
    this->dataIndex = dataIndex;
    this->profile = profile;
    memset(nullKey.data(), 0, AES_KEY_SIZE);
	b = ceil((float)log2(B));
	int prev = 0;
    int	cprev = 0;
    for (int i = 0; i <= dataIndex; i++) 
	{
		int j = i + b;
        int curNumberOfBins = j > 1 ? 
			(int) ceil(((float) pow(2, j))/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        int curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : pow(2,j);
		if(curSizeOfEachBin*curNumberOfBins <= 2*prev*cprev)
		{
			curNumberOfBins = ceil((float)(2*prev*cprev+1)/(float)curSizeOfEachBin);
		}
		cprev = curSizeOfEachBin;
		prev = curNumberOfBins;
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
		int is = curNumberOfBins*curSizeOfEachBin;
        printf("%d StLevel:%d number of Bins:%d size of bin:%d is:%d\n",j, i, curNumberOfBins, curSizeOfEachBin, is);
    }

}

bool OneChoiceStorage::setup(bool overwrite) 
{
    if (inMemoryStorage) 
	{
        for (int i = 0; i <= dataIndex; i++) 
		{
            vector<pair<prf_type, prf_type> > curData;
            data.push_back(curData);
        }
    } 
	else
	{
		filenames.resize(dataIndex+1);
        for (int i = 0; i <= dataIndex; i++) 
		{
			filenames[i].resize(3);
			for(int j=0; j<3; j++)
			{
           		string filename = fileAddressPrefix + "SDD-" + to_string(i)+"-"+to_string(j) + ".dat";
           		filenames[i][j]=filename;
           		fstream testfile(filename.c_str(), std::ofstream::in);
           		if (testfile.fail() || overwrite) 
		   		{
           		    testfile.close();
           		    fstream file(filename.c_str(), std::ofstream::out);
           		    if (file.fail()) 
           		        cerr << "Error: " << strerror(errno);
           		    int maxSize = numberOfBins[i] * sizeOfEachBin[i];
           		    for (int k = 0; k < maxSize; k++) 
		   			{
           		        file.write((char*) nullKey.data(), AES_KEY_SIZE);
           		        //file.write((char*) nullKey.data(), AES_KEY_SIZE);
           		    }
           		    file.close();
           		}
        	}
		}
    }

}

void OneChoiceStorage::insertAll(int index, int instance, vector<prf_type> ciphers) 
{
    if (inMemoryStorage) 
	{
        //for (auto item : ciphers) {
          //  data[index].insert(data[index].end(), item.begin(), item.end());
        //}
    } 
	else 
	{
            fstream file(filenames[index][instance].c_str(), ios::binary | ios::out);
            if (file.fail()) 
                cerr << "Error in insert: " <<strerror(errno)<<endl;
            for (auto ci : ciphers) 
			{
                //for (auto pair : item) {
                    unsigned char newRecord[AES_KEY_SIZE];
                    memset(newRecord, 0, AES_KEY_SIZE);
                    std::copy(ci.begin(), ci.end(), newRecord);
                    file.write((char*) newRecord, AES_KEY_SIZE);
    int bina = *(int*) (&(ci.data()[AES_KEY_SIZE - 10]));
    int id = *(int*) (&(ci.data()[AES_KEY_SIZE - 5]));
					//if(ci != nullKey)
					//cout <<"wrote ["<<ci.data()<<"|"<<id<<"|"<<bina<<"] in "<<filenames[index][instance].c_str()<<endl;
                //}
            }
            file.close();
    }
}


vector<prf_type> OneChoiceStorage::getAllData(int index, int instance) 
{
    if (inMemoryStorage) 
	{
		
        vector<prf_type> results;
        /*for (int i = 0; i < data[index].size(); i++) 
		{
            if (data[index][i].first != nullKey) 
                results.push_back(data[index][i]);
        }*/
        return results;
    } 
	else 
	{
            vector<prf_type> results;
            fstream file(filenames[index][instance].c_str(), ios::binary | ios::in | ios::ate);
            if (file.fail()) 
                cerr << "Error in read: " << strerror(errno);
            int size = file.tellg();
            file.seekg(0, ios::beg);
            char* keyValues = new char[size];
            file.read(keyValues, size);
            file.close();

            for (int i = 0; i < size / AES_KEY_SIZE; i++) 
			{
                prf_type tmp;
                std::copy(keyValues+i*AES_KEY_SIZE, keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE, tmp.begin());
                //if (tmp != nullKey) 
				//{
                    results.push_back(tmp);
                //}
            }
            delete keyValues;
            return results;
    }
}

vector<prf_type> OneChoiceStorage::getElements(int index, int instance, int start, int numOfEl)
{
	vector<prf_type> results;
    fstream file(filenames[index][instance].c_str(), ios::binary | ios::in | ios::ate);
    if (file.fail()) 
        cerr << "Error in getElements: " << strerror(errno);
    int seek = AES_KEY_SIZE*start;
    file.seekg(seek, ios::beg);
	SeekG++;
	int readLength = (numOfEl)*AES_KEY_SIZE;
	int size = numberOfBins[index]*sizeOfEachBin[index]*AES_KEY_SIZE;
	int remainder = size - seek;
	//cout <<"index:"<<index<<endl;
	//cout <<"start:"<<start<<" end:"<<end<<" seek:"<< seek<<endl;
	//cout<<"rem:"<<remainder<<" size:"<<size<< "read len:"<<readLength<<endl;
	if(readLength > remainder)
		readLength = remainder;
	if(remainder <0)
		readLength = 0;
    char* keyValues = new char[readLength];
    file.read(keyValues, readLength);
    file.close();
////////////////////////

//    file.seekg(seek, ios::beg);
//	readLength = size;
//    char* keyValues = new char[size];
//    file.read(keyValues, readLength);
//    file.close();

////////////////////////
    for (int i = 0; i < readLength/AES_KEY_SIZE; i++) 
	{
        prf_type restmp;
        std::copy(keyValues+i*AES_KEY_SIZE, keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE, restmp.begin());
        //if (restmp != nullKey) {
            results.push_back(restmp);
        //}
    }
	return results;
}

void OneChoiceStorage::clear(int index, int instance) 
{
    if (inMemoryStorage) 
	{
        data[index].clear();
    } 
	else 
	{
        fstream file(filenames[index][instance].c_str(), std::ios::binary | std::ofstream::out);
        if (file.fail()) 
            cerr << "Error: " << strerror(errno);
        int maxSize = numberOfBins[index] * sizeOfEachBin[index];
        for (int j = 0; j < maxSize; j++) 
		{
            file.write((char*) nullKey.data(), AES_KEY_SIZE);
            //file.write((char*) nullKey.data(), AES_KEY_SIZE);
        }
        file.close();
    }
}

OneChoiceStorage::~OneChoiceStorage() {}

vector<prf_type> OneChoiceStorage::searchBin(int index, int instance, int bin) 
{
    vector<prf_type> results;
    std::fstream file(filenames[index][instance].c_str(), ios::binary | ios::in);
    if (file.fail()) 
        cerr << "Error in read: " << strerror(errno);
    int readPos = bin * AES_KEY_SIZE * sizeOfEachBin[index];
    int fileLength = numberOfBins[index] * sizeOfEachBin[index] * AES_KEY_SIZE;
    int remainder = fileLength - readPos;
    //int totalReadLength = cnt * KEY_VALUE_SIZE * sizeOfEachBin[index];
    int totalReadLength = AES_KEY_SIZE * sizeOfEachBin[index];
    int readLength = 0;
    if (totalReadLength > remainder) 
	{
        readLength = remainder;
    } 
	else 
	{
        readLength = totalReadLength;
    }
	if(remainder<0)
		readLength = 0;
    file.seekg(readPos, ios::beg);
    SeekG++;
    char* keyValues = new char[readLength];
    file.read(keyValues, readLength);
    readBytes += readLength;
	cout <<"index:"<<index<<" bin:"<<bin<<" sizeOfBin:"<<sizeOfEachBin[index]<<endl;
	//cout <<"readPos:"<<readPos<<" readLen:"<<readLength<<" totalsize:"<< AES_KEY_SIZE*sizeOfEachBin[index]*numberOfBins[index]<<endl;
	cout<<"rem:"<<remainder<<" read len:"<<readLength<<endl;
	assert(readLength>=AES_KEY_SIZE);
    for (int i = 0; i < readLength / AES_KEY_SIZE; i++) 
	{
        prf_type restmp;
        std::copy(keyValues+i*AES_KEY_SIZE,keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE,restmp.begin());
        if (restmp != nullKey) 
		{
            results.push_back(restmp);
        }
    }
    file.close();
    return results;
}

/*
vector<prf_type> OneChoiceStorage::find(int index, prf_type mapKey, int cnt) {
    if (inMemoryStorage) {
        vector<prf_type> results;

        unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
        if (cnt >= numberOfBins[index]) {
            for (int i = 0; i < numberOfBins[index] * sizeOfEachBin[index]; i++) {
                if (data[index][i].first != nullKey) {
                    results.push_back(data[index][i].second);
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
            vector<prf_type> results;

            std::fstream file(filenames[index].c_str(), ios::binary | ios::in);
            if (file.fail()) {
                //cerr << "Error in read: " << strerror(errno);
            }
            unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
            if (cnt >= numberOfBins[index]) {
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
            } else {
                int pos = (unsigned int) (*((int*) hash)) % numberOfBins[index];
                int readPos = pos * KEY_VALUE_SIZE * sizeOfEachBin[index];
                int fileLength = numberOfBins[index] * sizeOfEachBin[index] * KEY_VALUE_SIZE;
                int remainder = fileLength - readPos;
                int totalReadLength = cnt * KEY_VALUE_SIZE * sizeOfEachBin[index];
                int readLength = 0;
                if (totalReadLength > remainder) {
                    readLength = remainder;
                    totalReadLength -= remainder;
                } else {
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
    }
}
*/
