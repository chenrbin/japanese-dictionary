#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "DictionaryEntry.h"
#include "JishoDict.h"

using namespace std;
using namespace chrono;


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
