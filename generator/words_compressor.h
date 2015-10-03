#pragma once

#include <string>
#include <vector>
#include <map>
#include <locale>

class WordsCompressor {
public:
    size_t addWord(const std::string&);
    std::string getWord(size_t) const;
    static std::string canonizeWord(const std::string& word, const std::locale& locale);
private:
    std::vector<std::string> storage_;
    std::map<std::string, size_t> mapper_;
};
