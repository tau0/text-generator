#include "text_generator.h"

#include <random>
#include <iostream>
#include <string>
#include <clocale>
#include <fstream>

int main(int argc, char* argv[]) {
    std::cerr << argc << std::endl;
    std::cerr << argv[1] << std::endl;

    size_t order = std::stoi(argv[1]);
    const auto locale = argv[2];

    std::ifstream trainingText(argv[3]);

    TextGenerator generator(order, locale);
    generator.fit(trainingText);
    generator.showTransistions();

    generator.generate(std::atoi(argv[4]));
    std::ofstream modelOutputStream("model.protobin");
    generator.saveModel(modelOutputStream);

    return 0;
}
