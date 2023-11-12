#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <chrono>
#include "DictionaryEntry.h"
using namespace std;
int main(){
    auto start_time = std::chrono::steady_clock::now();
    cout << "Start timer\n";
    int entryCount = 0;
    map<string, DictionaryEntry> jisho;
    for (int i = 1; i <= 32; ++i) { // Iterates for each term bank file
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
            string part1, part2, part3, part4;

            if (!getline(file, part1, '[') || !getline(file, part2, '[') || !getline(file, part3, ']') ||
                !getline(file, part4, ']'))
                break;

            // Parse part2, which contains first five fields
            vector<string> fields1to5(5);
            stringstream ss(part2);
            for (string& field: fields1to5)
                getline(ss, field, ',');

            // Part3 is only definition field. Individual definitions will be parsed in the class

            // Parse part4, which contains last two fields
            string field7, field8;
            stringstream ss2(part4);
            getline(ss2, field7, ','); // Unused comma
            getline(ss2, field7, ',');
            getline(ss2, field8, ',');
            if (!jisho.count(fields1to5[0]))
                jisho[fields1to5[0]].addEntry(fields1to5, part2, field7, field8);
            else {
                //cout << fields1to5[0] << " | " << entryCount << " entry already exists. Do something later\n";
            }
            entryCount++;
        }

        //cout << entryCount << endl;

        // Read last bracket and confirm end of file
        getline(file, input, ']');
        if (getline(file, input))
            cout << "File not empty";
        file.close();
    }
    auto end_time = std::chrono::steady_clock::now();
    cout << "Jisho size is " << jisho.size() << endl;
    string target = "オールターナティブ・スクール";
    if (jisho.count(target))
        jisho[target].printEntry();
    else
        cout << target << " is missing\n";
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Elapsed time: " << elapsed_time.count() << " milliseconds" << std::endl;

    return 0;
}