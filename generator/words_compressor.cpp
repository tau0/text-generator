#include "words_compressor.h"
#include <stdexcept>
#include <algorithm>

size_t WordsCompressor::addWord(const std::string& word) {
    if (mapper_.find(word) != mapper_.end()) {
        return mapper_[word];
    }

    size_t id = 0;
    storage_.push_back(word);
    id = storage_.size() - 1;
    mapper_.insert(std::make_pair(word, id));
    return id;
}

std::string WordsCompressor::getWord(size_t id) const {
    if (id >= storage_.size()) {
        throw std::out_of_range("no word with this id");
    }

    return storage_[id];
}

std::string WordsCompressor::canonizeWord(const std::string& word, const std::locale& locale) {
    std::locale::global(locale);
    std::string filtered;

    std::remove_copy_if(word.begin(), word.end(),
        std::back_inserter(filtered),
        std::ptr_fun<int, int>(&std::ispunct)
    );

    std::string lowered;
    for (const auto& letter : filtered) {
        lowered.push_back(std::tolower(letter, locale));
    }
    return lowered;
}
