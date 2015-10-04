#pragma once
#include "words_compressor.h"

#include <iostream>
#include <queue>
#include <clocale>
#include <random>

typedef unsigned long long ull;

class TextGenerator {
public:
    /// @brief           Prepare object for fitting.
    ///
    /// @param order     Order of a desired markov chain.
    /// @pararm locale   Input data encoding. Like "ru_RU.CP1251"
    /// @param seed      Random seed for mersenne random generator.
    ///
    explicit TextGenerator(ull order, const std::string& locale, unsigned int seed)
        : markovChainOrder_(order)
        , modelIsReady_(false)
        , locale_(locale)
        , mersenneRandomGenerator_(seed)
    {}

    /// @brief           Prepare object for fitting.
    ///
    /// @param order     Order of a desired markov chain.
    /// @pararm locale   Input data encoding. Like "ru_RU.CP1251"
    ///
    explicit TextGenerator(ull order, const std::string& locale)
        : TextGenerator(order, locale, std::random_device()())
    {}

    /// @brief       Construct model from a binary file given to the modelStream.
    ///
    explicit TextGenerator(std::istream& modelStream);

    /// @brief       Save model to the outputStream
    ///
    void saveModel(std::ostream& outputStream) const;

    /// @brief       Fit model which should be read from file or fitted.
    ///
    void fit(std::istream& inputStream = std::cin);

    /// @brief       Generate sequence of numbers using markov chain with initial state 
    ///              given to inputStream.
    ///
    /// @param requiredNumberOfWords     Number of words to generate.
    ///
    void generate(
        ull requiredNumberOfWords,
        std::istream& inputStream = std::cin,
        std::ostream& outputStream =  std::cout
    );

    /// @brief       Output states and transitions in human readable way.
    ///
    void showTransistions(std::ostream& outputStream = std::cout) const;

private:
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
