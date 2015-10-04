#pragma once
#include "words_compressor.h"

#include <iostream>
#include <queue>
#include <clocale>
#include <random>

typedef unsigned long long ull;

class TextGenerator {
public:
    explicit TextGenerator(ull order, const std::string& locale, unsigned int seed)
        : markovChainOrder_(order)
        , modelIsReady_(false)
        , locale_(locale)
        , mersenneRandomGenerator_(seed)
    {}
    explicit TextGenerator(ull order, const std::string& locale)
        : TextGenerator(order, locale, std::random_device()())
    {}
    explicit TextGenerator(std::istream& model);

    void saveModel(std::ostream& outputStream) const;
    void fit(std::istream& inputStream = std::cin);
    void generate(ull requiredNumberOfWords, std::istream& inputStream = std::cin, std::ostream& outputStream =  std::cout);
    void showTransistions(std::ostream& outputStream = std::cout) const;
    ull getNextWord(const std::queue<ull>& buffer);

private:
    ull markovChainOrder_;
    bool modelIsReady_;
    WordsCompressor wordCompressor_;
    std::map<std::queue<ull>, std::map<ull, ull>> counter_;
    std::map<std::queue<ull>, ull> totals_;
    std::string locale_;
    std::mt19937_64 mersenneRandomGenerator_;
};
