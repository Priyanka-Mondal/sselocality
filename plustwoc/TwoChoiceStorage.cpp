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
        int curNumberOfBins = i > 3 ? ((int) ceil((float) pow(2, i)/(log2(log2(log2(pow(2,i))))))) 
		                    : pow(2,i);
	curNumberOfBins = pow(2, (int)ceil(log2(curNumberOfBins))); 
        int curSizeOfEachBin = i > 3 ? 3*ceil((log2(log2(log2(pow(2,i)))))) : 3;
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
	cuckoofilenames.resize(dataIndex);
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
            string st = fileAddressPrefix + "STASH-" + to_string(i) + ".dat";
            stashes.push_back(st);
            fstream stest(st.c_str(), std::ofstream::in);
            if (stest.fail() || overwrite) 
	    {
                stest.close();
                fstream sfile(st.c_str(), std::ofstream::out);
                if (sfile.fail()) 
	        {
                    cerr << "Error: " << strerror(errno);
                }
                int maxSize = pow(2,i);//numberOfBins[i] * sizeOfEachBin[i];
                for (int j = 0; j < maxSize; j++) 
	        {
                    sfile.write((char*) nullKey.data(), AES_KEY_SIZE);
                }
                sfile.close();
            }
	    cuckoofilenames[i].resize(dataIndex);
	    stashfilenames[i].resize(dataIndex);
	    for(int k = 0; k< dataIndex; k++) // k < log2(pow(2, dataIndex))
	    {
	         cuckoofilenames[i][k].resize(2);
	    	 for(int c = 0; c<2; c++)
	   	 {
	   	     string cuckoo = fileAddressPrefix+"CUCKOO-"+to_string(i)+"-"
	   	     		+to_string(k) +"-"+to_string(c)+ ".dat";
	   	     cuckoofilenames[i][k][c]=cuckoo;
           	     fstream cfile(cuckoo.c_str(), std::ofstream::in);
           	     if (cfile.fail() || overwrite) 
	   	     {
           	         cfile.close();
           	         fstream c1file(cuckoo.c_str(), std::ofstream::out);
           	         if (c1file.fail()) 
           	             cerr << "Error: " << strerror(errno);
           	         int maxSize = 2* pow(2,i);//numberOfBins[i] * sizeOfEachBin[i];
           	     	 for (int j = 0; j < maxSize; j++) 
	   	             c1file.write((char*) nullKey.data(), AES_KEY_SIZE);
           	         c1file.close();
           	     }
	   	 }
           	 string stashfilename = fileAddressPrefix + "CUCKOOSTASH-" 
		            +to_string(i)+"-"+to_string(k) + ".dat";
	   	 stashfilenames[i][k]=stashfilename;
           	 fstream tfile(stashfilename.c_str(), std::ofstream::in);
           	 if (tfile.fail() || overwrite) 
	   	 {
           	     tfile.close();
           	     fstream sfile(stashfilename.c_str(), std::ofstream::out);
           	     if (sfile.fail()) 
           	         cerr << "Error: " << strerror(errno);
           	     int maxSize = 2*pow(2,i);//numberOfBins[i] * sizeOfEachBin[i]; 
           	     for (int j = 0; j < maxSize; j++) 
           	         sfile.write((char*) nullKey.data(), AES_KEY_SIZE);
           	     sfile.close();
           	 }
	    }
        }	
    }
}


pair<prf_type, vector<prf_type>> TwoChoiceStorage::insertCuckooHT(int index,int tableNum,int cuckooID, 
		int hash, prf_type keyword, vector<prf_type> fileids)
{
	fstream cuckoo(cuckoofilenames[index][tableNum][cuckooID].c_str(), 
				ios::binary | ios::out | ios::in);
	cout <<"insertCuckooHT:"<<cuckoofilenames[index][tableNum][cuckooID].c_str()<<endl;
        if (cuckoo.fail()) 
            cerr << "Error in cuckoo hash table read: " << strerror(errno);

	vector<prf_type> results;
	int entrySize = pow(2, tableNum);
    int readPos = hash *(entrySize+1)* AES_KEY_SIZE;
    cuckoo.seekg(readPos, ios::beg);
    SeekG++;
	
	char* oldKey = new char[AES_KEY_SIZE];
	cuckoo.read(oldKey, AES_KEY_SIZE);
	prf_type keyw;
        copy(oldKey+0, oldKey+AES_KEY_SIZE, keyw.begin());
	delete oldKey;
        readBytes +=AES_KEY_SIZE ;

	results.resize(entrySize);
        int readLength = entrySize*AES_KEY_SIZE;
	//cout <<"["<<keyw.data()<<"]"<<" " <<"["<<nullKey.data()<<"]"<<endl;
        cuckoo.seekg(readPos+AES_KEY_SIZE, ios::beg); //cuckoo.seekg(AES_KEY_SIZE, ios::cur);
	if(keyw != nullKey)
	{
            char* keyValues = new char[readLength];
            cuckoo.read(keyValues, readLength);
            readBytes += readLength;
            for (int i = 0; i < entrySize ; i++) 
	    {
                prf_type tmp;
                std::copy(keyValues+i*AES_KEY_SIZE, keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE, tmp.begin());
                //if (tmp != nullKey)
	        //{
                   results.push_back(tmp);
                //}
            }
	}
	else
	{
	    cuckoo.clear();
            int readPos = hash *(entrySize+1)* AES_KEY_SIZE;
            cuckoo.seekg(readPos, ios::beg);
            SeekG++;
            unsigned char newRecord[AES_KEY_SIZE];
            memset(newRecord, 0, AES_KEY_SIZE);
            std::copy(keyword.begin(), keyword.end(), newRecord);
            cuckoo.write((char*) newRecord, AES_KEY_SIZE);
	    for(auto c : fileids)
	    {
                memset(newRecord, 0, AES_KEY_SIZE);
                std::copy(c.begin(), c.end(), newRecord);
                cuckoo.write((char*) newRecord, AES_KEY_SIZE);
	    }
	}
	cuckoo.close();
	auto result = make_pair(keyw , results);
	return result;
}


void TwoChoiceStorage::insertCuckooStash(int index, int tableNum, vector<prf_type> ctCiphers)
{
      fstream file(stashfilenames[index][tableNum].c_str(), ios::binary | ios::out | ios::ate);
      if (file.fail()) 
          cerr << "(Error in Cuckoo Stash insert: " << strerror(errno)<<")"<<endl;
      for (auto item : ctCiphers) 
      {
          unsigned char newRecord[AES_KEY_SIZE];
          memset(newRecord, 0, AES_KEY_SIZE);
          std::copy(item.begin(), item.end(), newRecord);
          file.write((char*) newRecord, AES_KEY_SIZE);
      }
      file.close();
}
void TwoChoiceStorage::insertStash(int index, vector<prf_type> ciphers) 
{
      fstream file(stashes[index].c_str(), ios::binary | ios::out);
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
      fstream file(stashes[index].c_str(), ios::binary | ios::in | ios::ate);
      if (file.fail()) 
          cerr << "Error in read: " << strerror(errno);
      int size = file.tellg();
      file.seekg(0, ios::beg);
      SeekG++;
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


vector<prf_type> TwoChoiceStorage::getCuckooHT(int index) 
{
      vector<prf_type> results;
      for(int tn = 0; tn < index; tn++)
      {
         for(int c = 0; c<2; c++ )
	 {
	    fstream cuckoo(cuckoofilenames[index][tn][c].c_str(), ios::binary | ios::in | ios::ate);
      	    if (cuckoo.fail()) 
           	cerr << "Error in read: " << strerror(errno);
	    int entryNum = pow(2, (index-tn));
	    int entrySize = pow(2, tn);
      	    int size = cuckoo.tellg();
            cuckoo.seekg(0, ios::beg);
       	    SeekG++;
            char* keyValues = new char[size];
      	    cuckoo.read(keyValues, size);
	    for(int e = 0; e< entryNum; e++)
	    {
		for(int es = 0; es<=entrySize; es++) // one extra entry for the keyword
		{
		    prf_type entry;
          	    //std::copy(keyValues+(e+1)*es*AES_KEY_SIZE, 
		    //		keyValues+(e+1)*es*AES_KEY_SIZE+AES_KEY_SIZE, entry.begin());
          	    std::copy(keyValues+e*es*AES_KEY_SIZE, 
				keyValues+e*es*AES_KEY_SIZE+AES_KEY_SIZE, entry.begin());
		    if(es !=0 && entry != nullKey)
		    {
			results.push_back(entry);
		    } 
		    if(es == 0 && entry!=nullKey)
		    	cout <<"cuckoo key(encrypted):["<<entry.data()<<"]"<<endl;
		}
	    }
      	    delete keyValues;
            cuckoo.close();
	 }
         fstream file(stashfilenames[index][tn].c_str(), ios::binary | ios::in | ios::ate);
         if (file.fail()) 
             cerr << "Error in read: " << strerror(errno);
         int size = file.tellg();
         file.seekg(0, ios::beg);
	 SeekG++;
         char* keyValues = new char[size];
         file.read(keyValues, size);
         file.close();
         for (int i = 0; i < size/AES_KEY_SIZE ; i++) 
         {
             prf_type tmp;
             std::copy(keyValues+i*AES_KEY_SIZE, keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE, tmp.begin());
             if (tmp != nullKey) 
                 results.push_back(tmp);
         }
         delete keyValues;
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
    }
}

void TwoChoiceStorage::clear(int index) 
{
    if (inMemoryStorage) 
        data[index].clear();
    else 
    {
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
       fstream sfile(stashes[index].c_str(), std::ios::binary | std::ofstream::out);
       if (sfile.fail()) 
           cerr << "Error: " << strerror(errno);
       maxSize = pow(2,index);
       for (int j = 0; j < maxSize; j++) 
       {
           sfile.write((char*) nullKey.data(), AES_KEY_SIZE);
       }
       sfile.close();
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
           for(int c=0; c<2 ; c++)
           {
               fstream sfile(cuckoofilenames[index][k][c].c_str(), 
           		    std::ios::binary | std::ofstream::out);
               if (sfile.fail()) 
                   cerr << "Error: " << strerror(errno);
               int maxSize = 2*pow(2,index);
               for (int j = 0; j < maxSize; j++) 
                   sfile.write((char*) nullKey.data(), AES_KEY_SIZE);
               sfile.close();
	    }
        }
    }
}

TwoChoiceStorage::~TwoChoiceStorage() {
}

vector <prf_type> TwoChoiceStorage::cuckooSearch(int index, int tableNum, int h[2])
{
   vector<prf_type> results;
   //results.resize(0);
   for(int c=0; c<2; c++)
   { 
        std::fstream cuckoo(cuckoofilenames[index][tableNum][c].c_str(), ios::binary | ios::in | ios::ate);
        if (cuckoo.fail()) 
            cerr << "Error in read: " << strerror(errno);
        int entrySize = pow(2, tableNum);
        int readPos = h[c]*(entrySize+1)*AES_KEY_SIZE;
        readPos = readPos + AES_KEY_SIZE;
        int readLength = entrySize*AES_KEY_SIZE;
        cuckoo.seekg(readPos,ios::beg);
        SeekG++;
        char* keyValues = new char[readLength];
        cuckoo.read(keyValues, readLength);
        readBytes += readLength;
        
        for (int i = 0; i < entrySize; i++) 
        {
            prf_type tmp;
            std::copy(keyValues+i*AES_KEY_SIZE, keyValues+i*AES_KEY_SIZE+AES_KEY_SIZE, tmp.begin());
            if (tmp != nullKey) 
            {
                results.push_back(tmp);
            	cout <<"cuckoo result SIZE:"<<results.size()<<endl;
            }
        }
        cuckoo.close();
   }
   fstream file(stashfilenames[index][tableNum].c_str(), ios::binary | ios::in | ios::ate);
   if (file.fail()) 
       cerr << "Error in read: " << strerror(errno);
   int size = file.tellg();
   file.seekg(0, ios::beg);
   SeekG++;
   char* keyValues = new char[size];
   file.read(keyValues, size);
   file.close();
   readBytes += size;
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
vector<prf_type> TwoChoiceStorage::find(int index, prf_type mapKey, int cnt) 
{
     vector<prf_type> results;
     std::fstream file(filenames[index].c_str(), ios::binary | ios::in);
     if (file.fail()) 
         cerr << "Error in read: " << strerror(errno);
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
         for (int i = 0; i < readLength / KEY_VALUE_SIZE; i++) 
         {
             prf_type tmp, restmp;
             std::copy(keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, 
         		    keyValues + i * KEY_VALUE_SIZE + (2 * AES_KEY_SIZE), restmp.begin());
             if (restmp != nullKey) 
             {
                 results.push_back(restmp);
             }
         }
         if (totalReadLength > 0) 
         {
             readLength = totalReadLength;
             file.seekg(0, ios::beg);
             char* keyValues = new char[readLength];
             file.read(keyValues, readLength);
             readBytes += readLength;
             SeekG++;
             for (int i = 0; i < readLength / KEY_VALUE_SIZE; i++) 
             {
                 prf_type tmp, restmp;
                 std::copy(keyValues + i * KEY_VALUE_SIZE + AES_KEY_SIZE, 
         		keyValues + i * KEY_VALUE_SIZE + (2 * AES_KEY_SIZE), restmp.begin());
                 if (restmp != nullKey) 
         	{
                     results.push_back(restmp);
                 }
             }
         }
     }
     file.close();
     return results;
}


