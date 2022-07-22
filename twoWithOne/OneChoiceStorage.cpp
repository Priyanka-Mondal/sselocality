#include "OneChoiceStorage.h"
#include<string.h>

OneChoiceStorage::OneChoiceStorage(bool inMemory, long dataIndex, string fileAddressPrefix, bool profile) 
{
    this->inMemoryStorage = inMemory;
    this->fileAddressPrefix = fileAddressPrefix;
    this->dataIndex = dataIndex;
    this->profile = profile;
    memset(nullKey.data(), 0, AES_KEY_SIZE);
   	for (long j = 0; j <=dataIndex; j++) 
	{
        long curNumberOfBins = j > 1 ? 
			(long) ceil(((float) pow(2, j))/(float)(log2(pow(2, j))*log2(log2(pow(2, j))))) : 1;
        long curSizeOfEachBin = j > 1 ? 3*(log2(pow(2, j))*ceil(log2(log2(pow(2, j))))) : pow(2,j);
        numberOfBins.push_back(curNumberOfBins);
        sizeOfEachBin.push_back(curSizeOfEachBin);
    }

}

bool OneChoiceStorage::setup(bool overwrite) 
{
   for (long i = 0; i < dataIndex; i++) 
	{
       string filename = fileAddressPrefix + "ONEMAP-" + to_string(i) + ".dat";
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
               file.write((char*) nullKey.data(), AES_KEY_SIZE);
           }
           file.close();
       }
   }

}

void OneChoiceStorage::insertAll(long index, vector<vector< pair<prf_type, prf_type> > > ciphers) 
{
            fstream file(filenames[index].c_str(), ios::binary | ios::out);
            if (file.fail()) {
                cerr << "Error in insert: " << strerror(errno);
            }
            for (auto item : ciphers) {
                for (auto pair : item) {
                    unsigned char newRecord[KEY_VALUE_SIZE];
                    memset(newRecord, 0, KEY_VALUE_SIZE);
                    std::copy(pair.first.begin(), pair.first.end(), newRecord);
                    std::copy(pair.second.begin(), pair.second.end(), newRecord + AES_KEY_SIZE);
                    file.write((char*) newRecord, 2 * AES_KEY_SIZE);
                }
            }
            file.close();
}

vector<pair<prf_type, prf_type> > OneChoiceStorage::getAllData(long index) {
    if (inMemoryStorage) {
        vector<pair<prf_type, prf_type> > results;
        for (long i = 0; i < data[index].size(); i++) {
            if (data[index][i].first != nullKey) {
                results.push_back(data[index][i]);
            }
        }
        return results;
    } else {
        /*if (USE_XXL) {
            vector<pair<prf_type, prf_type> > results;
            for (long i = 0; i < diskData[index]->size(); i++) {
                if (diskData[index]->at(i).first != nullKey) {
                    results.push_back(diskData[index]->at(i));
                }
            }
            return results;
        } else {*/
            vector<pair<prf_type, prf_type> > results;
            fstream file(filenames[index].c_str(), ios::binary | ios::in | ios::ate);
            if (file.fail()) {
                cerr << "Error in read: " << strerror(errno);
            }
            long size = file.tellg();
            file.seekg(0, ios::beg);
            char* keyValues = new char[size];
            file.read(keyValues, size);
            file.close();

            for (long i = 0; i < size / KEY_VALUE_SIZE; i++) {
                prf_type tmp, restmp;
                std::copy(keyValues + i*KEY_VALUE_SIZE, keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, tmp.begin());
                std::copy(keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE + AES_KEY_SIZE, restmp.begin());
                if (tmp != nullKey) {
                    results.push_back(pair<prf_type, prf_type>(tmp, restmp));
                }
            }

            delete keyValues;

            return results;
        //}
    }
}

void OneChoiceStorage::clear(long index) {
    if (inMemoryStorage) {
        data[index].clear();
    } else {
        /*if (USE_XXL) {
            diskData[index]->clear();
        } else {*/
            fstream file(filenames[index].c_str(), std::ios::binary | std::ofstream::out);
            if (file.fail()) {
                cerr << "Error: " << strerror(errno);
            }
            long maxSize = numberOfBins[index] * sizeOfEachBin[index];
            for (long j = 0; j < maxSize; j++) {
                file.write((char*) nullKey.data(), AES_KEY_SIZE);
                file.write((char*) nullKey.data(), AES_KEY_SIZE);
            }
            file.close();
        //}
    }
}

OneChoiceStorage::~OneChoiceStorage() {
}

vector<prf_type> OneChoiceStorage::find(long index, prf_type mapKey, long cnt) 
{
        vector<prf_type> results;
        std::fstream file(filenames[index].c_str(), ios::binary | ios::in);
		//cout <<filenames[index].c_str()<<endl;
        if (file.fail()) 
            cerr << "Error in read: " << strerror(errno);
        
        unsigned char* hash = Utilities::sha256((char*) mapKey.data(), AES_KEY_SIZE);
        if (cnt >= numberOfBins[index]) 
		{
            //read everything
			//cout <<"full index"<<endl;
				cout <<"full index:"<<index<<endl;
            long fileLength = numberOfBins[index] * sizeOfEachBin[index] * KEY_VALUE_SIZE;
            char* keyValues = new char[fileLength];
            file.read(keyValues, fileLength);
            SeekG++;
            readBytes += fileLength;
            for (long i = 0; i < numberOfBins[index] * sizeOfEachBin[index]; i++) 
			{
                prf_type tmp, restmp;
                std::copy(keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, keyValues + i * KEY_VALUE_SIZE + (2 * AES_KEY_SIZE), restmp.begin());
                if (restmp != nullKey) 
				{
                    results.push_back(restmp);
                }
            }
			//cout <<"size:"<<results.size()<<endl;
        } 
		else 
		{
				cout <<"NOT full index:"<<index<<endl;
            long pos = (unsigned long) (*((long*) hash)) % numberOfBins[index];
            long readPos = pos * KEY_VALUE_SIZE * sizeOfEachBin[index];
            long fileLength = numberOfBins[index] * sizeOfEachBin[index] * KEY_VALUE_SIZE;
            long remainder = fileLength - readPos;
            long totalReadLength = cnt * KEY_VALUE_SIZE * sizeOfEachBin[index];
            long readLength = 0;
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
            for (long i = 0; i < readLength / KEY_VALUE_SIZE; i++) {
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
                for (long i = 0; i < readLength / KEY_VALUE_SIZE; i++) {
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
}
