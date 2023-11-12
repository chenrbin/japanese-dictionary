#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;
class DictionaryEntry {
    string mainText; // The main form of the entry, usually in kanji
    string yomikata; // The reading of the term in hiragana, if the mainText is not in kana
    string speechTag; // The classification of the term
    string field4; // Unknown. Usually empty
    string field5; // Unknown. Seems to be related to frequency. Not enclosed by quotes
    vector<string> definition; // English translation. Can have multiple definitions
    string dictID; // Assigned dictionary ID. Can be duplicate
    string field8; // Unknown. Usually empty
public:
    // Constructor is not used to initialize variables in order to deal with duplicates later
    DictionaryEntry();
    const string& getMainText() const;
    const string& getYomikata() const;
    const string& getSpeechTag() const;
    const string& getField4() const;
    const string& getField5() const;
    const vector<string>& getDefinition() const;
    const string& getDictId() const;
    const string& getField8() const;
    void addEntry(const vector<string>& fields1to5, const string& definitions, const string& dictID,
                  const string& unknownField8);
    void printEntry();
};


