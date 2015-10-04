#include "text_generator.h"

#include <random>
#include <iostream>
#include <string>
#include <clocale>
#include <fstream>
#include <cstring>

void showHelp() {
    std::cout << "please read the README.md file about program arguments" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showHelp();
        return 0;
    }

    std::string mode(argv[1]);
    if (mode == "fit") {
        if (argc != 5) {
            showHelp();
            return 0;
        }

        ull order;
        try {
            order = std::stoi(argv[2]);
        } catch  (...) {
            std::cout << "incorrect number for the order" << std::endl;
            return 0;
        }

        std::string locale(argv[3]);
        std::ofstream modelOutputFile(argv[4]);

        try {
            TextGenerator generator(order, locale);
            generator.fit();
            generator.saveModel(modelOutputFile);
        } catch (std::runtime_error& error) {
            std::cout << error.what() << std::endl;
        }
    } else if (mode == "generate") {
        if (argc != 4) {
            showHelp();
            return 0;
        }

        std::ifstream modelInputStream(argv[3]);

        ull wordsToGenerate;
        try {
            wordsToGenerate = std::stoi(argv[2]);
        } catch  (...) {
            std::cout << "incorrect number of words to generate" << std::endl;
            return 0;
        }

        TextGenerator generator(modelInputStream);
        generator.generate(wordsToGenerate);
    } else if (mode == "show") {
        if (argc != 3) {
            showHelp();
            return 0;
        }

        std::ifstream modelInputStream(argv[2]);
        TextGenerator generator(modelInputStream);
        generator.showTransistions();
    } else {
        showHelp();
        return 0;
    }

    return 0;
}
