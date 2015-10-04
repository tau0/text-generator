#include "text_generator.h"
#include "protos/cpp/model.pb.h"
#include <string>
#include <set>

TextGenerator::TextGenerator(std::istream& inputStream) {
    Protos::Model model;
    if (!model.ParseFromIstream(&inputStream)) {
        throw std::runtime_error("failed to read model");
    }

    locale_ = model.locale();
    markovChainOrder_ = model.order();

    std::vector<std::string> dictionary;
    for (const auto& word : model.words()) {
        dictionary.push_back(word);
    }
    wordCompressor_ = WordsCompressor(dictionary);

    for (const auto& transition : model.transitions()) {
        std::queue<ull> buffer;
        for (const auto& word : transition.state().words()) {
            buffer.push(word);
        }

        ull total = 0;
        for (const auto& destination : transition.destinations()) {
            counter_[buffer][destination.nextword()] = destination.count();
            total += destination.count();
        }
        totals_[buffer] = total;
    }

    modelIsReady_ = true;
}

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

        auto canonizedWord = WordsCompressor::canonizeWord(currentWord, std::locale(locale_));
        ull wordId = wordCompressor_.addWord(canonizedWord);

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
        auto canonizedWord = WordsCompressor::canonizeWord(currentWord, std::locale(locale_));
        ull wordId = wordCompressor_.getId(canonizedWord);
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
            state->add_words(buffer.front());
            buffer.pop();
        }

        for (const auto& destinationAndCount : transitions.second) {
            auto destination = transition->add_destinations();
            destination->set_nextword(destinationAndCount.first);
            destination->set_count(destinationAndCount.second);
        }
    }
    model.set_order(markovChainOrder_);
    model.set_locale(locale_);

    if (!model.SerializeToOstream(&outputStream)) {
        throw std::runtime_error("failed to write model");
    }
}
