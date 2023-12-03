#include "DictionaryEntry.h"
const string& DictionaryEntry::getMainText() const {
    return mainText;
}
const string& DictionaryEntry::getYomikata() const {
    return yomikata;
}
const string& DictionaryEntry::getSpeechTag() const {
    return speechTag;
}
const string& DictionaryEntry::getField4() const {
    return field4;
}
const string& DictionaryEntry::getField5() const {
    return field5;
}
const vector<string>& DictionaryEntry::getDefinitions() const {
    return definitions;
}
const string& DictionaryEntry::getDictId() const {
    return dictID;
}
const string& DictionaryEntry::getField8() const {
    return field8;
}
DictionaryEntry::DictionaryEntry() = default;
// Constructs dictionary entry using the strings obtained from the term bank files. All
DictionaryEntry::DictionaryEntry(const vector<string>& fields1to5, const vector<string>& definitions,
                                 const string& dictID, const string& unknownField8) {
    mainText = fields1to5[0];
    yomikata = fields1to5[1];
    speechTag = fields1to5[2];
    field4 = fields1to5[3];
    field5 = fields1to5[4];
    this->definitions = definitions;
    this->dictID = dictID;
    field8 = unknownField8;
}
// Print Term, reading, and all definitions
void DictionaryEntry::printEntry() {
    cout << mainText;
    if (!yomikata.empty())
        cout << " - " << yomikata;
    cout << endl;
    for (string& def : definitions)
        cout << "\t" << def << endl;
}

// Simplified entry addition, using only the basic attributes
DictionaryEntry::DictionaryEntry(const string& term, const string& reading, const string& definition) {
    mainText = term;
    yomikata = reading;
    definitions.push_back(definition);

    // Initialize unused attributes
    speechTag = "", field4 = "", field5 = "", dictID = "", field8 = "";

}

