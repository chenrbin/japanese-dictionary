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
    string build_duration_str;
    string search_duration_str;

    // Extra functionality used for searching algorithm
    int maxStringSize;

    vector<DictionaryEntry> searchResults;

    // Map from elementary verb conjugations to base forms
    map<string, pair<string,string>> conjugation;

    // Map for common Japanese spelling errors
    map<string, vector<string>> similarKana;
public:
    JishoDict(bool);
    void buildDictionary();
    void buildConjugations();
    void buildSimilarKana();
    bool isKanaOnly(const string& term);
    void resetDictionary(bool);
    void readFile(const string&);
    void addSingleEntry(const string&, const string&, const string&);
    void printEntry(const string& term);
    void printResults();
    string printResultsJson();
    vector<DictionaryEntry>* getEntry(const string&);
    vector<pair<vector<DictionaryEntry>*,string>> getDictionaryForm(const string& term);
    vector<vector<DictionaryEntry>*> getSpellCorrectedEntries(const string& term);
    set<string> getTermsFromKana(const string&);
    void printDictionaryForms(const string&);
    vector<DictionaryEntry> operator[](const string&);
    void scanText(const string& query);
    void scanTextAndStoreResults(const string& query);
    int getBuildTime() const;
    bool getUsingOrdered() const;
    int getDictionarySize();
    int getKanaMapSize();
    int getMaxStringSize() const;
};


