#pragma once

#include <string>
#include <vector>
#include <map>
#include <locale>

typedef unsigned long long ull;

class WordsCompressor {
public:
    ull addWord(const std::string&);
    ull getId(const std::string&) const;
    std::string getWord(ull) const;
    std::vector<std::string> getDictionary() const;
    static std::string canonizeWord(const std::string& word, const std::locale& locale);
private:
    std::vector<std::string> storage_;
    std::map<std::string, ull> mapper_;
};
