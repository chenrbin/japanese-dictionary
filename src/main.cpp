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

vector<vector<DictionaryEntry>> basicSearch(const string& query, JishoDict& jisho)
{
    vector<vector<DictionaryEntry>> result;
    for (unsigned int i = query.length(); i > 0; i-=3)
        if (!jisho.getEntry(query.substr(0, i)).empty())
            result.push_back(jisho.getEntry(query.substr(0, i)));
    return result;
}

void searchTest(const string& query, JishoDict& jisho)
{
    for (const vector<DictionaryEntry>& match : basicSearch(query, jisho))
    {
        for (const DictionaryEntry& entry : match)
        {
            cout << entry.getMainText() << endl;
            for (const string& def : entry.getDefinitions())
                cout << "\t" << def << endl;
        }
    }
}


int main(){
    JishoDict jisho(true);

    // Test if dictionary is working
    cout << "Jisho size is " << jisho.getDictionarySize() << endl;
    cout << "Kana map size is " << jisho.getKanaMapSize() << endl;
    jisho.printEntry("キーウィ");
    for (const string& term : jisho.getTermsFromKana("たえる"))
        cout << term << " ";
    cout << endl;
    searchTest("堪える", jisho);
    cout << endl;
    return 0;
}