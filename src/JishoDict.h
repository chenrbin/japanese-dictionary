#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <iostream>
#include <set>
#include <algorithm>
#include "DictionaryEntry.h"
using namespace std;
using namespace chrono;
// Class for the dictionary object, which contains a map to the entries
class JishoDict {
    // Declaring both maps, and choosing one to use
    map<string, vector<DictionaryEntry>> ordered;
    unordered_map<string, vector<DictionaryEntry>> unordered;
    bool usingOrdered;

    // Map to find terms using kana reading
    map<string, set<string>> kanaOrdered;
    unordered_map<string, set<string>> kanaUnordered;

    // Timer
    time_point<steady_clock> start_time;
    duration<long long, ratio<1, 1000>> buildTime{};

    // Extra functionality used for searching algorithm
    int maxStringSize;
public:
    JishoDict(bool);
    void buildDictionary();
    void resetDictionary(bool);
    void readFile(const string&);
    void addSingleEntry(const string&, const string&, const string&);
    void printEntry(const string& term);
    vector<DictionaryEntry> getEntry(const string&);
    set<string> getTermsFromKana(const string&);
    vector<DictionaryEntry> operator[](const string&);
    void scanText(const string& query);
    int getBuildTime() const;
    bool getUsingOrdered() const;
    int getDictionarySize();
    int getKanaMapSize();
    int getMaxStringSize() const;
};


