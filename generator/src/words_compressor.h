#pragma once

#include <string>
#include <vector>
#include <map>
#include <locale>

typedef unsigned long long ull;

class WordsCompressor {
public:
    WordsCompressor() {};
    /// @brief Construct word compressor using custom dictionary.
    ///
    explicit WordsCompressor(const std::vector<std::string>& dictionary);

    /// @brief Add word to the vocabulary.
    ///
    ull addWord(const std::string&);

    /// @brief Get id of the word.
    ///
    ull getId(const std::string&) const;

    /// @brief Get word by id.
    ///
    std::string getWord(ull) const;

    /// @brief Get list of the words.
    ///
    std::vector<std::string> getDictionary() const;

    /// @brief Canonize word: to lower, remove punctuation.
    ///
    /// @param locale It's encoding of a text for correct lowering and removing punctuation.
    /// @pararm word  Word for canonization in the locale encoding.
    /// 
    /// @return Canonized word.
    ///
    static std::string canonizeWord(const std::string& word, const std::locale& locale);
private:
    std::vector<std::string> storage_;
    std::map<std::string, ull> mapper_;
};
