#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <chrono>
#include "DictionaryEntry.h"
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
    // Timer to track time efficiency
    auto start_time = steady_clock::now();
    cout << "Start timer\n";

    // Counter for dictionary entry
    int entryCount = 0;

    map<string, vector<DictionaryEntry>> jisho;
    for (int i = 1; i <= 32; ++i) { // Iterates for each term bank file
        // Open file
        ifstream file("../jmdict_english/term_bank_" + to_string(i) + ".json");
        if (!file.is_open()) {
            cout << "File could not be opened\n";
            return -1;
        }

        string input;
        // Each file is wrapped by brackets. These are unused.
        getline(file, input, '[');

        // Every entry contains two [ and two ]. Use these as delimiters
        while (true) {
            string startingBracket, termField, definitionField, endingField;

            if (!getline(file, startingBracket, '[') || !getline(file, termField, '[') || !getline(file, definitionField, ']') ||
                !getline(file, endingField, ']'))
                break;

            // Parse termField, which contains first five fields
            vector<string> fields1to5(5);

            stringstream ss(termField);
            for (string& field: fields1to5)
                getline(ss, field, ',');
            // First four fields are enclosed by quotes. Delete those
            for (int j = 0; j < 4; ++j)
                fields1to5[j] = fields1to5[j].substr(1, fields1to5[j].size() - 2);

            // Parse endingField, which contains last two fields
            string field7, field8;
            stringstream ss2(endingField);
            getline(ss2, field7, ','); // Unused comma
            getline(ss2, field7, ',');
            getline(ss2, field8, ',');

            jisho[fields1to5[0]].push_back(DictionaryEntry(fields1to5, definitionField, field7, field8));
            entryCount++;
        }

        cout << entryCount << endl;

        // Read last bracket and confirm end of file
        getline(file, input, ']');
        if (getline(file, input))
            cout << "File not empty";
        file.close();
    }
    cout << "Jisho size is " << jisho.size() << endl;
    printEntry("キーウィ", jisho);

    // Print elapsed time
    auto end_time = steady_clock::now();
    auto elapsed_time = duration_cast<milliseconds>(end_time - start_time);
    cout << "Elapsed time: " << elapsed_time.count() << " milliseconds" << endl;

    // Temp
    searchTest("適当", jisho);

    return 0;
}