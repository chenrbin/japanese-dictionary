#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "DictionaryEntry.h"
#include "JishoDict.h"

using namespace std;
using namespace chrono;

void printEntry(const string& target, map<string, vector<DictionaryEntry>>& jisho) {
    if (jisho.count(target))
        for (DictionaryEntry& entry: jisho[target])
            entry.printEntry();
    else
        cout << target << " is missing\n";
}

vector<vector<DictionaryEntry>> basicSearch(const string& query, JishoDict& jisho) {
    vector<vector<DictionaryEntry>> result;
    for (unsigned int i = query.length(); i > 0; i -= 3)
        if (!jisho.getEntry(query.substr(0, i)).empty())
            result.push_back(jisho.getEntry(query.substr(0, i)));
    return result;
}

void searchTest(const string& query, JishoDict& jisho) {
    for (const vector<DictionaryEntry>& match: basicSearch(query, jisho)) {
        for (const DictionaryEntry& entry: match) {
            cout << entry.getMainText() << endl;
            for (const string& def: entry.getDefinitions())
                cout << "\t" << def << endl;
        }
    }
}


int main(int argc, char** argv) {
    JishoDict jisho(true);

    // Test if dictionary is working
    // cout << "Jisho size is " << jisho.getDictionarySize() << endl;
    // cout << "Kana map size is " << jisho.getKanaMapSize() << endl;
    // for (const string& term: jisho.getTermsFromKana("たえる"))
    //     cout << term << " ";
    // cout << endl;
    // jisho.scanTextAndStoreResults("無敵の笑顔で荒らすメディア知りたいその秘密ミステリアス");
    // jisho.printResults();
    // jisho.scanTextAndStoreResults("「おれの方ほうが強つよい。」「いいや、ぼくの方ほうが強つよい。」"
    //                "北風きたかぜと太陽たいようの声こえが聞きこえます。二人ふたりはどちらの力ちか"
    //                "らが強つよいかでケンカをしているようです。「太陽たいようが毎日まいにち元げん気き"
    //                "だから、暑あつくてみんな困こまっているよ。おれが涼すずしい風かぜを吹ふくと、みんな"
    //                "嬉うれしそうだ。おれの方ほうがみんなの役やくに立たっているよ。」「でも、ぼくがいな"
    //                "いと、木きや野や菜さいは育そだたないよ。冬ふゆは北風きたかぜの吹ふく風かぜが冷つめ"
    //                "たくて、とても寒さむかった。みんな外そとに出でられなかったよね？最近さいきんは暖あ"
    //                "たたかいから、みんな喜よろこんでいるよ。」「いいや、あそこを見みて。太陽たいようが"
    //                "強つよく照てらすから、川かわの水みずがもうすぐ無なくなりそうだ。水みずがないと、み"
    //                "んな生活せいかつできないよ。」");
    jisho.scanTextAndStoreResults(argv[1]); // only search parameter should be without a space
    string json = jisho.printResultsJson();
    ofstream result_json("./public/result_data.json");
    result_json << json << endl;
    result_json.close();
    // for (const string& term: jisho.getTermsFromKana("おれ"))
    //     cout << term << " ";
    // const set<string>& term = jisho.getTermsFromKana("おれ");
    // cout << term.size();
    // cout << endl;
    return 0;
}
