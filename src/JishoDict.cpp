#include <cstring>
#include "JishoDict.h"

// Select map mode, build dictionary, and start timer
JishoDict::JishoDict(bool usingOrdered) {
    this->usingOrdered = usingOrdered;
    maxStringSize = 0;
    buildDictionary();
    buildConjugations();
    buildSimilarKana();
}

// Manually add conjugations to conjugation map
void JishoDict::buildConjugations() {
    // Stems
    vector<string> stems = {"あいえお", "たちてと", "らりれろ", "なにねの","ばびべぼ",
                            "まみめも", "さしせそ", "かきけこ", "がぎげご"};
    string vowelEndings = "うつるぬぶむすくぐ";
    for (int i = 0; i < stems.size(); i++) {
        for (int j = 0; j < 4; j++) {
            conjugation[stems[i].substr(j*3, 3)] = make_pair(vowelEndings.substr(i*3, 3), j);
        }
    }
    // Te forms
    conjugation["って"] = make_pair("うつる", 4);
    conjugation["んで"] = make_pair("ぬぶむ", 4);
    conjugation["して"] = make_pair("す", 4);
    conjugation["いて"] = make_pair("く", 4);
    conjugation["いで"] = make_pair("ぐ", 4);
    conjugation["て"] = make_pair("る", 7);
    // Ta forms
    conjugation["った"] = make_pair("うつる", 5);
    conjugation["んだ"] = make_pair("ぬぶむ", 5);
    conjugation["した"] = make_pair("す", 5);
    conjugation["いた"] = make_pair("く", 5);
    conjugation["いだ"] = make_pair("ぐ", 5);
    conjugation["た"] = make_pair("る", 8);
}

void JishoDict::buildSimilarKana() {
    vector<string> groups = {"かが", "きぎ", "くぐ", "けげ", "こご",
                             "さざ", "しじ", "すず", "せぜ", "そぞ",
                             "ただ", "ちぢ", "つづっ", "てで", "とど",
                             "はばぱ", "ひびぴ", "ふぶぷ", "へべぺ", "ほぼぽ",
                             "やゃ", "ゆゅ", "よょ"};
    for (string group : groups)
        for (int i = 0; i < group.size(); i+=3)
            for (int j = 0; j < group.size(); j+=3)
                if (i != j)
                    similarKana[group.substr(i,3)].push_back(group.substr(j,3));
}

// Read jmdict files and record time
void JishoDict::buildDictionary() {
    // Check if dictionary has already been built
    if ((usingOrdered && !ordered.empty()) || (!usingOrdered && !unordered.empty())) {
        cout << "Dictionary has already been built\n";
        return;
    }
    cout << "Building from JMDict. Start timer\n";
    start_time = steady_clock::now();
    for (int i = 1; i <= 32; ++i) { // Iterates for each term bank file
        // Open file
        readFile("../jmdict_english/term_bank_" + to_string(i) + ".json");
    }
    buildTime = duration_cast<milliseconds>(steady_clock::now() - start_time);
    build_duration_str = to_string(buildTime.count());
    cout << "Elapsed time: " << buildTime.count() << " milliseconds" << endl;
}

// Clear dictionary entries and reassign usingOrdered
void JishoDict::resetDictionary(bool newValue) {
    (usingOrdered) ? ordered.clear() : unordered.clear();
    (usingOrdered) ? kanaOrdered.clear() : kanaUnordered.clear();

    usingOrdered = newValue;
    buildDictionary();
}

// Read a term bank file in the preset format. Can be used to add terms to the dictionary.
void JishoDict::readFile(const string& filename) {
    // Open file
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File could not be opened\n";
        return;
    }

    string input;
    // Each file is wrapped by brackets. These are unused.
    getline(file, input, '[');

    // Every entry contains two [ and two ]. Use these as delimiters
    while (true) {
        string startingBracket, termField, definitionField, endingField;
        // Check if at end of file
        if (!getline(file, startingBracket, '[') || !getline(file, termField, '[')
            || !getline(file, definitionField, ']') || !getline(file, endingField, ']'))
            break;

        // Parse termField, which contains first five fields
        vector<string> fields1to5(5);
        stringstream ss(termField);
        for (string& field: fields1to5) // [0] contains term, [1] contains reading
            getline(ss, field, ',');

        // First four fields are enclosed by quotes. Delete those
        for (int j = 0; j < 4; ++j)
            fields1to5[j] = fields1to5[j].substr(1, fields1to5[j].size() - 2);
        // Parse definitionField and create a vector of definition strings
        vector<string> definitions;
        ss = stringstream(definitionField);
        string def;
        while (getline(ss, def, '"'))
            if (!def.empty() && def != ",")
                definitions.push_back(def);
        // Parse endingField, which contains last two fields
        string field7, field8;
        ss = stringstream(endingField);
        getline(ss, field7, ','); // Unused comma
        getline(ss, field7, ',');
        getline(ss, field8, ',');

        // Remove surrounding quotes
        field7 = field7.substr(0, field7.size());
        field8 = field8.substr(1, field8.size() - 2);

        // Create entry
        if (usingOrdered)
            ordered[fields1to5[0]].push_back(DictionaryEntry(fields1to5, definitions, field7, field8));
        else
            unordered[fields1to5[0]].push_back(DictionaryEntry(fields1to5, definitions, field7, field8));

        // Update the size of the longest string
        if (fields1to5[0].size() > maxStringSize)
            maxStringSize = fields1to5[0].size();
        // Update kanaOrdered
        if (!fields1to5[1].empty()) {
            if (usingOrdered)
                kanaOrdered[fields1to5[1]].emplace(fields1to5[0]);
            else
                kanaUnordered[fields1to5[1]].emplace(fields1to5[0]);
        }
    }

    // Read last bracket and confirm end of file
    getline(file, input, ']');
    if (getline(file, input)) {
        cout << "File format error\n";
    }
    file.close();
}

// Simplified entry addition for manual input, using only the basic attributes
void JishoDict::addSingleEntry(const string& term, const string& reading, const string& definition) {
    if (usingOrdered)
        ordered[term].push_back(DictionaryEntry(term, reading, definition));
    else
        unordered[term].push_back(DictionaryEntry(term, reading, definition));
    // Update the size of the longest string
    if (term.size() > maxStringSize)
        maxStringSize = term.size();
    // Update kana map
    if (!reading.empty())
        (usingOrdered) ? kanaOrdered[reading].emplace(term) : kanaUnordered[reading].emplace(term);
}

// Search a term in the dictionary and return a copy of vector of entries.
// Returns an empty vector if there is no match.
vector<DictionaryEntry>* JishoDict::getEntry(const string& term) {
    // If simply returning a reference map[term], the map will add keys for terms with no matches
    if (usingOrdered)
        return ordered.count(term) ? &ordered[term] : nullptr;
    else
        return unordered.count(term) ? &unordered[term] : nullptr;
}

vector<pair<vector<DictionaryEntry>*,int>> JishoDict::getDictionaryForm(const string& term) {
    vector<pair<vector<DictionaryEntry>*,int>> result;
    for (int i = 3; i <= term.length(); i += 3) { // Ichiban stem test
        if (getEntry(term.substr(0, i).append("る"))) {
            vector<DictionaryEntry>* dictionaryForm = getEntry(term.substr(0, i).append("る"));
            if (dictionaryForm->at(1).getVerbType() == "v1")
                result.push_back(make_pair(dictionaryForm, 6));
        }
    }
    for (int len = 3; len <= 6; len+=3) { // len = 3 for 1 char conjugations, 6 for 2 char conjugations
        if (term.length() < len + 3) // Insufficient length to be a verb with given conjugation
            return result;
        for (int i = 3; i <= term.length(); i+=3) {
            if (conjugation.count(term.substr(i, len))) {
                pair<string, int> conj = conjugation[term.substr(i, len)];
                for (int j = 0; j <= conj.first.length(); j+=3)
                    if (getEntry(term.substr(0, i) + conj.first.substr(j, 3))) {
                        vector<DictionaryEntry>* dictionaryForm = getEntry(term.substr(0, i) + conj.first.substr(j, 3));
                        if (dictionaryForm->at(1).getVerbType() == "v5" && conj.second < 6
                            || dictionaryForm->at(1).getVerbType() == "v1" && conj.second > 6)
                            result.push_back(make_pair(dictionaryForm, conj.second));
                    }
            }
        }
    }
    return result;
}

bool JishoDict::isKanaOnly(const string& term) {
    for (int i = 0; i < term.length(); i+=3)
        if (strcmp(term.substr(i, 3).c_str(), "一") >= 0) // Naive implementation; code points for kana are all less than kanji
            return false;
    return true;
}

vector<vector<DictionaryEntry>*> JishoDict::getSpellCorrectedEntries(const string& term) {
    vector<vector<DictionaryEntry>*> result;
    for (int i = 0; i < term.length(); i+=3) {
        if (similarKana.count(term.substr(i,3))) {
            for (const string& correction : similarKana[term.substr(i,3)]) {
                string newTerm = term.substr(0,i) + correction + term.substr(i+3);
                if (getEntry(newTerm))
                    result.push_back(getEntry(newTerm));
            }
        }
    }
    return result;
}

// Returns a list of terms that match the given kana reading
set<string> JishoDict::getTermsFromKana(const string& reading) {
    if (usingOrdered)
        return kanaOrdered.count(reading) ? kanaOrdered[reading] : set<string>();
    else
        return kanaUnordered.count(reading) ? kanaUnordered[reading] : set<string>();
}

vector<DictionaryEntry> JishoDict::operator[](const string& term) {
    if (usingOrdered)
        return ordered.count(term) ? ordered[term] : vector<DictionaryEntry>();
    else
        return unordered.count(term) ? unordered[term] : vector<DictionaryEntry>();
}

// Print all entries to a term
void JishoDict::printEntry(const string& term) {
    auto entries = getEntry(term);
    if (!entries->empty())
        for (DictionaryEntry& entry : *entries)
            entry.printEntry();
    else
        cout << term << " is missing\n";
}

void JishoDict::printResults() {
    if (searchResults.empty()) {
        cout << "No results." << endl;
        return;
    }
    for (auto result: searchResults) {
        result.printEntry();
    }
}

string JishoDict::printResultsJson() {
    string json = "{";
    json += "\"dict_build_time\": \"" + build_duration_str + "\",";
    json += "\"dict_search_time\": \"" + search_duration_str + "\",";
    json += "\"results\":[";
    for (int i = 0; i < searchResults.size(); ++i) {
        DictionaryEntry result = searchResults[i];
        string term = "\"term\":\"" + result.getMainText() + "\",";
        string yomikata = "\"yomikata\":\"" + result.getYomikata() + "\",";
        string definitions = "\"definitions\":[";
        vector<string> defVect = result.getDefinitions();
        for (int j = 0; j < defVect.size(); ++j) {
            string def = defVect[j];
            definitions += "\"" + def + "\"";
            if (j != defVect.size() - 1) {
                definitions += ",";
            }
        }
        definitions += "]";
        json += "{" + term + yomikata + definitions + "}";
        if (i != searchResults.size() - 1) { 
            json += ",";
        }
    };
    json += "]}";
    return json;
}

// Scan a string of text for terms
void JishoDict::scanText(const string& query) {
    auto start = steady_clock::now();
    for (int i = 0; i < query.length(); i += 1) { // Start index
        for (unsigned int j = min(maxStringSize, int(query.size()) - i); j > 0; j -= 1) { // String length
            // Scan for hits
            vector<DictionaryEntry>* hit = getEntry(query.substr(i, j));
            if (!hit->empty()) {
                hit->at(0).printEntry(); // Print only one entry for now
                i += j - 1; // Move starting point to the end of the hit term
                break;
            }
        }
    }
    auto end = steady_clock::now();
    cout << "Search time: " << float(duration_cast<microseconds>(end - start).count()) / 1000 << " milliseconds\n";
}

void JishoDict::scanTextAndStoreResults(const string& query) {
    searchResults.clear();
    auto start = steady_clock::now();
    for (int i = 0; i < query.length(); i += 1) { // Start index
        for (unsigned int j = min(maxStringSize, int(query.size()) - i); j > 0; j -= 1) { // String length
            // Scan for hits
            string curr_query = query.substr(i, j);
            vector<DictionaryEntry>* hit = getEntry(curr_query);
            if (!hit->empty()) {
                searchResults.push_back(hit->at(0));
                i += j - 1; // Move starting point to the end of the hit term
                break;
            }
        }
    }
    auto end = steady_clock::now();
    
    search_duration_str = to_string(float(duration_cast<microseconds>(end - start).count()) / 1000);
    cout << "Search time: " << float(duration_cast<microseconds>(end - start).count()) / 1000 << " milliseconds\n";
}

int JishoDict::getBuildTime() const {
    return buildTime.count();
}

// Return true if dictionary is using an ordered map
bool JishoDict::getUsingOrdered() const {
    return usingOrdered;
}

// Returns the number of terms in the dictionary
int JishoDict::getDictionarySize() {
    return (usingOrdered) ? ordered.size() : unordered.size();
}

// Returns the number of keys in the kana to kanji map
int JishoDict::getKanaMapSize() {
    return (usingOrdered) ? kanaOrdered.size() : kanaUnordered.size();
}

// Return the size of the longest string
int JishoDict::getMaxStringSize() const {
    return maxStringSize;
}
void JishoDict::printDictionaryForms(const string& term) {
    for (pair<vector<DictionaryEntry>*,int> i : getDictionaryForm("term"))
        for (int j = 0; j < i.first->size(); j++)
        {
            cout << i.second << endl;
            i.first->at(j).printEntry();
        }
}

