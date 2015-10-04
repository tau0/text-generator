#include "text_generator.h"

#include <random>
#include <iostream>
#include <string>
#include <clocale>
#include <fstream>

int main(int argc, char* argv[]) {
    std::cerr << argc << std::endl;
    std::cerr << argv[1] << std::endl;


    std::ifstream trainingText(argv[3]);

    std::ifstream modelInputStream("model.protobin");
    TextGenerator generator(modelInputStream);
    generator.showTransistions();

    generator.generate(std::atoi(argv[4]));

    return 0;
}
