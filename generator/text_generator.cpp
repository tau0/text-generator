#include "text_generator.h"
#include <string>
#include <set>

void TextGenerator::fit(std::istream& inputStream) {
    std::string currentWord;
    std::queue<size_t> buffer;

    while (inputStream >> currentWord) {
        std::transform(
            currentWord.begin(),
            currentWord.end(),
            currentWord.begin(),
            ::tolower
        );

        auto filteredWord = WordsCompressor::canonizeWord(currentWord, locale_);
        size_t wordId = wordCompressor_.addWord(filteredWord);

        if (buffer.size() == markovChainOrder_) {
            ++counter_[std::make_pair(buffer, wordId)];
            buffer.pop();
        }

        buffer.push(wordId);
    }

}

void TextGenerator::showTransistions(std::ostream& outputStream) const {
    for (const auto& transition : counter_) {
        auto queue = transition.first.first;
        outputStream << '(' << wordCompressor_.getWord(queue.front());
        queue.pop();
        while (!queue.empty()) {
            outputStream << ", " << wordCompressor_.getWord(queue.front());
            queue.pop();
        }
        outputStream << ')'; 
        outputStream << " -> " << wordCompressor_.getWord(transition.first.second)
                     << " = " << transition.second << std::endl;
    }
}

