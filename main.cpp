#include "twochoice/Amortized.h"
#include "twochoice/Utilities.h"
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
    vector<TC<int> > testCases;
    vector<string> testKeywords;
    uint keywordLength = 7;
    bool inMemory, overwrite;
    string filename = "config.txt";
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
        for (int j = 0; j < testCases.size(); j++) 
	{
            for (int z = 0; z < testCases[j].Qs.size(); z++) 
	    {
                file << testCases[j].testKeywords[z] << endl;
            }
        }
        file.close();
    } 
    else 
    {
        fstream file("/tmp/search.txt", std::ofstream::in);
        if (file.fail()) 
	{
            cerr << "Error in read: " << strerror(errno);
        }
        for (int i = 0; i < testCases.size(); i++) 
	{
            for (int z = 0; z < testCases[i].Qs.size(); z++) 
	    {
                string tmp;
                getline(file, tmp);
                testKeywords.push_back(tmp);
		cout <<"["<<tmp<<"]"<<endl;
            }
        }
        file.close();
        //Utilities::generateTestCases(testCases, keywordLength, time(NULL), testKeywords);
    Utilities::generateTwoChoiceTestCases(testCases, keywordLength, time(NULL),overwrite, testKeywords);
    }
    
    Amortized client(testCases[0].N, inMemory, overwrite); // only one test is performed for now
    client.endSetup(testCases[0].N);
    cout << "Start of Static, size of test suits:" << testCases.size()<< endl;
    cout <<"----------------------------------------------"<<endl;
    
    for (uint i = 0; i < testCases.size(); i++) 
    {
	cout <<" SIZE of testcases:"<<testCases[i].Qs.size()<<endl;
        int cnt = 0;
        double time = 0;
        if (overwrite) 
	{
	    int key = 0;
            for (auto cur = testCases[i].filePairs.begin(); cur != testCases[i].filePairs.end(); cur++) 
	    {
		key++;
		//cout <<" Total keywords:"<<key<<"/"<<testCases[i].filePairs.size()<<endl;
		int j;
                for (j = 0; j < cur->second.size(); j++) 
		{
                    client.update(OP::INS, cur->first, cur->second[j], true);
                    cnt++;
                    if (cnt % 1000 == 0) 
		    {
                        cout << "Initial Insertion:" << cnt << "/" << to_string(testCases[i].N) << endl;
                    }
                }
	        //cout <<" SIZE of file ids:"<<j<<"/"<<cur->second.size()<<endl;
            }
            cnt = 0;
            for (uint j = 0; j < testCases[i].Qs.size(); j++) 
	    {
                auto item = testCases[i].filePairs[j].second;
                vector<int> delPoses;
		if(testCases[i].delNumber[j] < item.size())
		{
                    for (uint k = 0; k < testCases[i].delNumber[j]; k++) 
	            {
                        int randPos = (rand() % (item.size() - 1)) + 1;
                        if (find(delPoses.begin(), delPoses.end(), randPos) != delPoses.end()) 
                            k--;
	                else 
                            delPoses.push_back(randPos);
                    }
		}
		else
		{
			for(uint k= 0 ;k<item.size();k++)
				delPoses.push_back(k);
		}
                for (uint k = 0; k < testCases[i].delNumber[j]; k++) 
		{
                    Utilities::startTimer(500);
                    client.update(OP::DEL, testCases[i].testKeywords[j], item[delPoses[k]], true);
		    //cout <<"deleted:"<<testCases[i].testKeywords[j]<<" fileid:"<<delPoses[k]<<endl;
                    time = Utilities::stopTimer(500);
                }
            }
        }
        //        client.endSetup();
	
        for (uint j = 0; j < testCases[i].Qs.size(); j++) 
	{
            cout << "------------------------------------------------------------------------------" << endl;
            cout << "Result of Operations for DB Size " << testCases[i].N << endl;
            cout << "Search for Keyword With [" << testCases[i].Qs[j] << "] Results and [" << testCases[i].delNumber[j] << "] Deletions:" << endl;
                Utilities::startTimer(500);
                vector<int> res = client.search(testCases[i].testKeywords[j]);
                time = Utilities::stopTimer(500);
                cout<<"Search Computation Time(micro):"<<time<<" for:"<<testCases[i].testKeywords[j]<<endl;
                cout << "Search Communication Size (Bytes):" << client.getTotalSearchCommSize() << endl;
                cout << "Number of return item:[" << res.size()<<"]" << endl;
        }
        cout << "**************************************************************************" << endl;

    }
    return 0;
}
