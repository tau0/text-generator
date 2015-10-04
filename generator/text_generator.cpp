#include "text_generator.h"
#include "protos/cpp/model.pb.h"
#include <string>
#include <set>

void TextGenerator::fit(std::istream& inputStream) {
    std::string currentWord;
    std::queue<ull> buffer;

    while (inputStream >> currentWord) {
        std::transform(
            currentWord.begin(),
            currentWord.end(),
            currentWord.begin(),
            ::tolower
        );

        auto filteredWord = WordsCompressor::canonizeWord(currentWord, locale_);
        ull wordId = wordCompressor_.addWord(filteredWord);

        if (buffer.size() == markovChainOrder_) {
            ++counter_[buffer][wordId];
            ++totals_[buffer];
            buffer.pop();
        }

        buffer.push(wordId);
    }

    modelIsReady_ = true;
}

void TextGenerator::generate(ull requiredNumberOfWords, std::istream& inputStream, std::ostream& outputStream) {
    if (!modelIsReady_) {
        throw std::invalid_argument("the model isn't loaded or fitted");
    }
    std::queue<ull> buffer;

    std::string currentWord;
    ull numberOfWords = 0;
    while (inputStream >> currentWord) {
        auto filteredWord = WordsCompressor::canonizeWord(currentWord, locale_);
        ull wordId = wordCompressor_.getId(filteredWord);
        buffer.push(wordId);
        ++numberOfWords;
        if (numberOfWords > markovChainOrder_) {
            throw std::invalid_argument("too many word in the input");
        }
    }
    if (numberOfWords != markovChainOrder_) {
        throw std::invalid_argument("incorrect number of words in the input");
    }

    for (ull i = 0; i < requiredNumberOfWords; ++i) {
        if (totals_.find(buffer) == totals_.end()) {
            break;
        }
        ull probableNextWord = getNextWord(buffer);
        outputStream << wordCompressor_.getWord(buffer.front()) << ' ';
        buffer.pop();
        buffer.push(probableNextWord);
    }

    while (!buffer.empty()) {
        outputStream << wordCompressor_.getWord(buffer.front()) << ' ';
        buffer.pop();
    }
}

ull TextGenerator::getNextWord(const std::queue<ull>& buffer) {
    auto totalsIt = totals_.find(buffer);
    if (totalsIt == totals_.end()) {
        throw std::invalid_argument("there isn't such state");
    }
    std::uniform_int_distribution<ull> getRandomInt(1, totalsIt->second);
    ull randomValue = getRandomInt(mersenneRandomGenerator_); 
    ull partialSum = 0;

    auto availableTransitions = counter_[buffer];

    // TODO: Can be optimized using lower_bound with some different data layout.
    for (const auto& nextWord : availableTransitions) {
        partialSum += nextWord.second;
        if (randomValue <= partialSum) {
            return nextWord.first;
        }
    }

    throw std::logic_error("shouldn't happen. data model probably corrupted");
}


void TextGenerator::showTransistions(std::ostream& outputStream) const {
    for (const auto& transition : counter_) {
        for (const auto& destination : transition.second) {
            auto queue = transition.first;
            outputStream << '(' << wordCompressor_.getWord(queue.front());
            queue.pop();
            while (!queue.empty()) {
                outputStream << ", " << wordCompressor_.getWord(queue.front());
                queue.pop();
            }
            outputStream << ')'; 
            outputStream << " -> " << wordCompressor_.getWord(destination.first)
                         << " = " << destination.second << std::endl;
        }
    }
}

void TextGenerator::saveModel(std::ostream& outputStream) const {
    Protos::Model model;
    auto dictionary = wordCompressor_.getDictionary();
    for (const auto& el : dictionary) {
        model.add_words(el);
    }
        
    for (const auto& transitions: counter_) {
        auto transition = model.add_transitions();
        auto state = transition->mutable_state();
        auto buffer = transitions.first;
        while (!buffer.empty()) {
            state->add_word(buffer.front());
            buffer.pop();
        }

        for (const auto& destinationAndCount : transitions.second) {
            auto destination = transition->add_destinations();
            destination->set_nextword(destinationAndCount.first);
            destination->set_count(destinationAndCount.second);
        }
    }

    model.SerializeToOstream(&outputStream);

}
