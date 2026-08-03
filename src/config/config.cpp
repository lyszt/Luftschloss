#include "./config.h"
#include <algorithm>
#include <unordered_map>
#include <utility>

BotConfig* botConfig = nullptr;

std::string BotConfig::getToken() {
    return this->bot->token;
}

std::vector<std::string> BotConfig::getTopKFrequent(const std::vector<std::string>& items, int k) {
    std::unordered_map<std::string, int> frequencyMap;
    for (const std::string& item : items) {
        frequencyMap[item]++;
    }

    std::vector<std::pair<std::string, int>> freqVec(frequencyMap.begin(), frequencyMap.end());
    std::sort(freqVec.begin(), freqVec.end(), [](const auto& a, const auto& b) {
        return a.second != b.second ? a.second > b.second : a.first < b.first;
    });

    std::vector<std::string> result;
    for (size_t i = 0; i < static_cast<size_t>(std::max(k, 0)) && i < freqVec.size(); ++i) {
        result.push_back(freqVec[i].first);
    }

    return result;
}