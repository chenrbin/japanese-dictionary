#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <iostream>
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
    map<string, vector<string>> kanaMap;

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
    vector<DictionaryEntry> operator[](const string&);
    const duration<long long int, ratio<1, 1000>>& getBuildTime() const;
    bool getUsingOrdered() const;
    int getDictionarySize();
    int getMaxStringSize();
};


