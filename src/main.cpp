#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "DictionaryEntry.h"
#include "JishoDict.h"
using namespace std;
using namespace chrono;

void printEntry(const string& target, map<string, vector<DictionaryEntry>>& jisho){
    if (jisho.count(target))
        for (DictionaryEntry& entry : jisho[target])
            entry.printEntry();
    else
        cout << target << " is missing\n";
}

vector<pair<string,vector<DictionaryEntry>*>> basicSearch(const string& query, map<string, vector<DictionaryEntry>>& jisho)
{
    vector<pair<string,vector<DictionaryEntry>*>> result;
    for (unsigned int i = query.length(); i > 0; i-=3)
    {
        if (jisho.count(query.substr(0, i)) > 0)
            result.push_back(make_pair(query.substr(0, i), &jisho[query.substr(0, i)]));
    }
    return result;
}

void searchTest(const string& query, map<string, vector<DictionaryEntry>>& jisho)
{
    for (pair<string,vector<DictionaryEntry>*> match : basicSearch(query, jisho))
    {
        for (int i = 0; i < match.second->size(); i++)
        {
            DictionaryEntry entry = match.second->at(i);
            cout << match.first << "\n";
            for (const string& def : entry.getDefinitions())
                cout << "\t" << def << "\n";
        }
    }
}


int main(){
    JishoDict jisho(false);

    // Test if dictionary is working
    cout << "Jisho size is " << jisho.getDictionarySize() << endl;
    jisho.printEntry("キーウィ");
    string str = "キーウィ";
    cout << str.size() << endl;

    cout << endl;
    return 0;
}