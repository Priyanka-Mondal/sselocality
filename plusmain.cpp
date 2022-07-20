#include "plustwoc/Amortized.h"
#include "plustwoc/Utilities.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char** argv) 
{
    // create a disk_config structure.
    // add disk to config
    //stxxl::config * cfg = stxxl::config::get_instance();
    //cfg->add_disk(stxxl::disk_config("disk=/tmp/stxxl.tmp, 10 GiB, syscall unlink"));
    vector<TC<long> > testCases;
    vector<string> testKeywords;
    ulong keywordLength = 7;
    bool inMemory, overwrite;
    string filename = "configs/config.txt";
    if (argc == 2) 
    {
        filename = string(argv[1]);
    }
    //    Utilities::readConfigFile(testCases, usehdd, cleaningMode);       //for testing crimes dataset
    Utilities::readConfigFile(argc, argv, filename, testCases, inMemory,overwrite);
    
    if (overwrite) 
    {
        Utilities::generateTwoChoiceTestCases(testCases, keywordLength, time(NULL),overwrite, testKeywords);
        fstream file("/tmp/search.txt", std::ofstream::out);
        if (file.fail()) 
            cerr << "Error: " << strerror(errno);
        for (long j = 0; j < testCases.size(); j++) 
	{
            for (long z = 0; z < testCases[j].Qs.size(); z++) 
                file << testCases[j].testKeywords[z] << endl;
        }
        file.close();
    } 
    else 
    {
        fstream file("/tmp/search.txt", std::ofstream::in);
        if (file.fail()) 
            cerr << "Error in read: " << strerror(errno);
        for (long i = 0; i < testCases.size(); i++) 
	{
            for (long z = 0; z < testCases[i].Qs.size(); z++) 
	    {
                string tmp;
                getline(file, tmp);
		testCases[i].testKeywords.push_back(tmp);
                testKeywords.push_back(tmp);
		cout <<"Reading test keyword: ["<<tmp<<"]"<<endl;
            }
        }
        file.close();
    }
    
    Amortized client(testCases[0].N, inMemory, overwrite); // only one test is performed for now
    client.endSetup(testCases[0].N);
    cout << "Start of Static, size of test suits:" << testCases.size()<< endl;
    cout <<"*************************************************************************"<<endl;
    
    for (ulong i = 0; i < testCases.size(); i++) 
    {
	cout <<"SIZE of testcases:"<<testCases[i].Qs.size()<<endl;
        long cnt = 0;
        double time = 0;
        if (overwrite) 
	{
	    long key = 0;
            for (auto cur = testCases[i].filePairs.begin(); cur != testCases[i].filePairs.end(); cur++) 
	    {
		key++;
		//cout <<" Total keywords:"<<key<<"/"<<testCases[i].filePairs.size()<<endl;
		long j;
                for (j = 0; j < cur->second.size(); j++) 
		{
                    client.update(OP::INS, cur->first, cur->second[j], true);
					//cout <<"cur->sec:"<<cur->second[j]<<endl;
                    cnt++;
                    if (cnt % 10 == 0) 
		    {
                      cout << "Initial Insertions:" << cnt << "/" << to_string(testCases[i].N) << endl;
                    }
                }
	        //cout <<" SIZE of file ids:"<<j<<"/"<<cur->second.size()<<endl;
            }
	    
            cnt = 0;
            for (unsigned long j = 0; j < testCases[i].Qs.size(); j++) 
	    {
                auto item = testCases[i].filePairs[j].second;
                vector<long> delPoses;
		if(testCases[i].delNumber[j] < item.size())
		{
                    for (ulong k = 0; k < testCases[i].delNumber[j]; k++) 
	            {
                        long randPos = (rand() % (item.size() - 1)) + 1;
                        if (find(delPoses.begin(), delPoses.end(), randPos) != delPoses.end()) 
                            k--;
	                else 
                            delPoses.push_back(randPos);
                    }
		}
		else
		{
			for(ulong k= 0 ;k<item.size();k++)
				delPoses.push_back(k);
		}
                for (ulong k = 0; k < testCases[i].delNumber[j]; k++) 
		{
                    Utilities::startTimer(500);
                    client.update(OP::DEL, testCases[i].testKeywords[j], item[delPoses[k]], true);
		    //cout <<"deleted:"<<testCases[i].testKeywords[j]<<" fileid:"<<delPoses[k]<<endl;
                    time = Utilities::stopTimer(500);
                }
            }
        }
        //        client.endSetup();






        for (ulong j = 0; j < testCases[i].Qs.size(); j++) 
	{
            cout << "------------------------------------------------------------------------------" << endl;
            cout << "Result of Operations for DB Size " << testCases[i].N << endl;
            cout << "Search for Keyword With [" << testCases[i].Qs[j] << "] Results and [" << testCases[i].delNumber[j] << "] Deletions:" <<testCases[i].testKeywords[j]<< endl;
                Utilities::startTimer(500);
                vector<long> res = client.search(testCases[i].testKeywords[j]);
                time = Utilities::stopTimer(500);
                //cout<<"Search Computation Time(micro):"<<time<<" for:"<<testCases[i].testKeywords[j]<<endl;
                //cout << "Search Communication Size (Bytes):" << client.getTotalSearchCommSize() << endl;
                cout << "Number of return item:[" << res.size()<<"]" << endl;
		assert(testCases[i].Qs[j]-testCases[i].delNumber[j] == res.size());
        }
        cout << "*********************************************************************************" << endl;

    }
    return 0;
}
