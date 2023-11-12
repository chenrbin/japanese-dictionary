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
const vector<string>& DictionaryEntry::getDefinition() const {
    return definition;
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
void DictionaryEntry::addEntry(const vector<string>& fields1to5,
                               const string& definitions, const string& dictID, const string& unknownField8) {
    mainText = fields1to5[0].substr(1, fields1to5[0].size() - 2);

    yomikata = fields1to5[1].substr(1, fields1to5[1].size() - 2);
    //cout << yomikata << endl;
    speechTag = fields1to5[2].substr(1, fields1to5[2].size() - 2);
    field4 = fields1to5[3].substr(1, fields1to5[3].size() - 2);
    field5 = fields1to5[4];

    // Build definitions
    vector<string> defs;
    stringstream ss(definitions);
    string input;
    while (getline(ss, input, ','))
        defs.push_back(input.substr(1, input.size() - 2));

    this->dictID = dictID.substr(1, dictID.size() - 2);
    field8 = unknownField8.substr(1, unknownField8.size() - 2);


}
void DictionaryEntry::printEntry() {
    cout << "Term: " << mainText << endl;
    cout << "Reading: " << yomikata << endl;
    cout << "Definition: " << definition[0] << endl;

}
