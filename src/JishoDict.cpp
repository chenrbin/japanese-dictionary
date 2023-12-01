#include "JishoDict.h"

// Select map mode, build dictionary, and start timer
JishoDict::JishoDict(bool usingOrdered) {
    this->usingOrdered = usingOrdered;
    maxStringSize = 0;
    buildDictionary();
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
vector<DictionaryEntry> JishoDict::getEntry(const string& term) {
    // If simply returning a reference map[term], the map will add keys for terms with no matches
    if (usingOrdered)
        return ordered.count(term) ? ordered[term] : vector<DictionaryEntry>();
    else
        return unordered.count(term) ? unordered[term] : vector<DictionaryEntry>();
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
    if (!entries.empty())
        for (DictionaryEntry& entry: entries)
            entry.printEntry();
    else
        cout << term << " is missing\n";
}

// Scan a string of text for terms
void JishoDict::scanText(const string& query) {
    auto start = steady_clock::now();
    for (int i = 0; i < query.length(); i += 3) { // Start index
        for (unsigned int j = min(maxStringSize, int(query.size()) - i); j > 0; j -= 3) { // String length
            // Scan for hits
            //cout << query.substr(i, j) << endl;
            vector<DictionaryEntry> hit = getEntry(query.substr(i, j));
            if (!hit.empty()) {
                hit[0].printEntry(); // Print only one entry for now
                i += j - 3; // Move starting point to the end of the hit term
                break;
            }
        }
    }
    auto end = steady_clock::now();
    cout << "Search time: " << float(duration_cast<microseconds>(end - start).count()) / 1000 << " milliseconds\n";
}

const duration<long long int, ratio<1, 1000>>& JishoDict::getBuildTime() const {
    return buildTime;
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

