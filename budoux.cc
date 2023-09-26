#include "budoux.h"

#include <fstream>

#include "nlohmann/json.hpp"
#include "utf8string.h"

namespace budoux {

namespace {

nlohmann::json json;

int getScore(const char *featureKey, utf8string sequence) {
    std::string s   = sequence.value();
    try {
        return json.at(featureKey).at(s).get<int>();
    }
    catch (std::exception e) {
        // 何もしなくてよい
    }
    return 0;
}

}

bool loadModel(std::string& path) {
    std::ifstream ifs(path + "model.json");
    if ( ! ifs) {
        return false;
    }
    ifs >> json;
    return true;
}

std::vector<std::string> parse(std::string& sentence, int max_width) {
    std::vector<utf8string> splitted;
    std::vector<std::string> result;
    if (sentence.empty()) {
        return result;
    }
    int total_score  = 0;
    for (auto& [_, value] : json.items()) {
        for (auto& [_, value] : value.items()) {
            total_score += value.get<int>();
        }
    }
    auto utf8str    = utf8string::parse(sentence);
    if (utf8str.size() == 0) {
        return result;
    }
    splitted.push_back(utf8str.substr(0, 1));
    for (int i = 1; i < utf8str.size(); i++) {
        int score   = -total_score;
        if (i - 3 >= 0) {
            score   += 2 * getScore("UW1", utf8str.substr(i - 3, 1));
        }
        if (i - 2 >= 0) {
            score   += 2 * getScore("UW2", utf8str.substr(i - 2, 1));
        }
        // i = 1で始まっているのでif (i - 1 >= 0)は必ずtrueになるのでいらない
        score   += 2 * getScore("UW3", utf8str.substr(i - 1, 1));
        score   += 2 * getScore("UW4", utf8str.substr(i, 1));
        if (i + 1 < utf8str.size()) {
            score   += 2 * getScore("UW5", utf8str.substr(i + 1, 1));
        }
        if (i + 2 < utf8str.size()) {
            score   += 2 * getScore("UW6", utf8str.substr(i + 2, 1));
        }
        if (i >= 2) {
            score   += 2 * getScore("BW1", utf8str.substr(i - 2, 2));
        }
        // i = 1で始まっているのでif (i - 1 >= 0)は必ずtrueになるのでいらない
        score   += 2 * getScore("BW2", utf8str.substr(i - 1, 2));
        if (i + 1 < utf8str.size()) {
            score   += 2 * getScore("BW3", utf8str.substr(i, 2));
        }
        if (i - 3 >= 0) {
            score   += 2 * getScore("TW1", utf8str.substr(i - 3, 3));
        }
        if (i - 2 >= 0) {
            score   += 2 * getScore("TW2", utf8str.substr(i - 2, 3));
        }
        // i = 1で始まっているのでif (i - 1 >= 0)は必ずtrueになるのでいらない
        if (i + 1 < utf8str.size()) {
            score   += 2 * getScore("TW3", utf8str.substr(i - 1, 3));
        }
        if (i + 2 < utf8str.size()) {
            score   += 2 * getScore("TW4", utf8str.substr(i, 3));
        }
        if (score > 0) {
            splitted.push_back(utf8string::parse(""));
        }
        splitted[splitted.size() - 1]   += utf8str[i];
    }
    if (max_width <= 0) {
        for (auto& e : splitted) {
            result.push_back(e.value());
        }
    }
    else {
        int width   = 0;
        for (auto& e : splitted) {
            if (result.empty() || width > max_width) {
                result.push_back(e.value());
                width   = e.width();
            }
            else if (width + e.width() > max_width) {
                result.push_back(e.value());
                width   = e.width();
            }
            else {
                result[result.size() - 1]   += e.value();
                width   += e.width();
            }
        }
    }
    return result;
}

}
