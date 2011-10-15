# include "include/MapGenerator.hpp"

# include "include/Settings.hpp"
# include "include/Randomizer.hpp"

# include <cmath>
# include <cfloat>
# include <iostream>

MapGenerator::MapGenerator():
    heightMap_(Settings::pointer()->getValueOf<int>("width")*Settings::pointer()->getValueOf<int>("height")),
    randomPointsHor_(),
    randomPointsVer_() {}

std::vector<unsigned char> const MapGenerator::generateHeightMap() {

    std::cout << "Generating height map..." << std::endl;

    createBaseNiveau();
    generateNoise();

    std::cout << "Generated height map." << std::endl;

    return heightMap_;
}

void MapGenerator::createBaseNiveau() {
    auto settings(Settings::pointer());

    auto baseNiveau(settings->getValueOf<float>("baseNiveau"));

    for (auto pixel(0); pixel < heightMap_.size(); ++pixel) {
        heightMap_[pixel] = static_cast<unsigned char>(baseNiveau*255);
    }

}

void MapGenerator::generateNoise() {
    auto randomizer(Randomizer::pointer());
    auto settings(Settings::pointer());

    auto noiseFactor(std::abs(settings->getValueOf<int>("noiseFactor")));

    for (auto i(0); i < heightMap_.size(); ++i) {
        int randomDiff(randomizer->random(-noiseFactor,noiseFactor));
        auto newVal(static_cast<int>(heightMap_[i] + randomDiff));
        if (newVal > 255)
            heightMap_[i] = 255;
        else if (newVal < 0)
            heightMap_[i] = 0;
        else heightMap_[i] = newVal;
    }
}
