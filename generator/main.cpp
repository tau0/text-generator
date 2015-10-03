#include "text_generator.h"

#include <iostream>
#include <string>
#include <clocale>

int main(int argc, char* argv[]) {
    std::cerr << argc << std::endl;
    std::cerr << argv[1] << std::endl;

    size_t order = std::stoi(argv[1]);
    const auto locale = argv[2];

    TextGenerator generator(order, locale);
    generator.fit(std::cin);
    generator.showTransistions();

    return 0;
}
