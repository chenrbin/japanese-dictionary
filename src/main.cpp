#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "DictionaryEntry.h"
#include "JishoDict.h"

using namespace std;
using namespace chrono;

void printEntry(const string& target, map<string, vector<DictionaryEntry>>& jisho) {
    if (jisho.count(target))
        for (DictionaryEntry& entry: jisho[target])
            entry.printEntry();
    else
        cout << target << " is missing\n";
}

vector<vector<DictionaryEntry>> basicSearch(const string& query, JishoDict& jisho) {
    vector<vector<DictionaryEntry>> result;
    for (unsigned int i = query.length(); i > 0; i -= 3)
        if (!jisho.getEntry(query.substr(0, i)).empty())
            result.push_back(jisho.getEntry(query.substr(0, i)));
    return result;
}

void searchTest(const string& query, JishoDict& jisho) {
    for (const vector<DictionaryEntry>& match: basicSearch(query, jisho)) {
        for (const DictionaryEntry& entry: match) {
            cout << entry.getMainText() << endl;
            for (const string& def: entry.getDefinitions())
                cout << "\t" << def << endl;
        }
    }
}

void testBuildTime() {
    JishoDict jisho(false);
    vector<int> unorderedTimes;
    vector<int> orderedTimes;
    const int ITERATIONS = 5;
    for (int i = 0; i < ITERATIONS; ++i) {
        jisho.resetDictionary(false);
        unorderedTimes.push_back(jisho.getBuildTime());
        jisho.resetDictionary(true);
        orderedTimes.push_back(jisho.getBuildTime());
    }
    float unorderedAverage = 0, orderedAverage = 0;
    cout << "Unordered map (hash table) build times: ";
    for (int val: unorderedTimes) {
        unorderedAverage += val;
        cout << val << " ";
    }
    cout << endl;
    unorderedAverage /= ITERATIONS;
    cout << "Average: " << unorderedAverage << endl;
    cout << "Ordered map (binary tree) build times: ";
    for (int val: orderedTimes) {
        orderedAverage += val;
        cout << val << " ";
    }
    cout << endl;
    orderedAverage /= ITERATIONS;
    cout << "Average: " << orderedAverage << endl;
}

int main(int argc, char** argv) {
    JishoDict jisho(true);

    jisho.scanTextAndStoreResults(argv[1]); // only search parameter should be without a space
    string json = jisho.printResultsJson();
    ofstream result_json("./public/result_data.json");
    result_json << json << endl;
    result_json.close();

    return 0;
}
