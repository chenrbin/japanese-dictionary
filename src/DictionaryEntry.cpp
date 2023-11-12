//
// Created by geniu on 11/12/2023.
//

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
DictionaryEntry::DictionaryEntry() {
    // This is empty for now
}
DictionaryEntry::DictionaryEntry(const vector<string>& fields1to5,
                               const string& defs, const string& dictID, const string& unknownField8) {
    mainText = fields1to5[0];
    yomikata = fields1to5[1];
    speechTag = fields1to5[2];
    field4 = fields1to5[3];
    field5 = fields1to5[4];

    // Build definitions
    stringstream ss(defs);
    string input;
    while (getline(ss, input, ',')){
        definitions.push_back(input.substr(1, input.size() - 2));
    }

    this->dictID = dictID.substr(1, dictID.size() - 2);
    field8 = unknownField8.substr(1, unknownField8.size() - 2);
}
void DictionaryEntry::printEntry() {
    cout << "Term: " << mainText << endl;
    cout << "Reading: " << yomikata << endl;
    for (string& def : definitions)
        cout << "Definition: " << def << endl;
}
