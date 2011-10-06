# include "include/Randomizer.hpp"

# include <cstdlib>
# include <ctime>
# include <iostream>

void Randomizer::init() const {
    std::srand(std::time(NULL));
}

float Randomizer::random(float begin, float end) const {
    return static_cast<float>(std::rand()) / RAND_MAX * (end - begin) + begin;
}

int Randomizer::random(int begin, int end) const {
    return std::rand() % (end - begin + 1) + begin;
}
