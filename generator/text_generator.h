#pragma once
#include "words_compressor.h"

#include <iostream>
#include <queue>
#include <clocale>

class TextGenerator {
public:
    explicit TextGenerator(size_t order, const char* const locale)
        : markovChainOrder_(order)
        , locale_(locale)
    {}

    void fit(std::istream& inputStream = std::cin);
    void generate();
    void showTransistions(std::ostream& outputStream = std::cout) const;

private:
    size_t markovChainOrder_;
    WordsCompressor wordCompressor_;
    std::map<std::pair<std::queue<size_t>, size_t>, size_t> counter_;
    std::locale locale_;

};
