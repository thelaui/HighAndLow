# include "include/MapGenerator.hpp"

# include "include/Settings.hpp"
# include "include/Randomizer.hpp"
# include "include/Clamper.hpp"

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

    Clamper clamper;
    clamper.clamp<float>(baseNiveau, 0.f, 1.f);

    for (auto pixel(0); pixel < heightMap_.size(); ++pixel) {
        heightMap_[pixel] = static_cast<unsigned char>(baseNiveau*255);
    }
}

void MapGenerator::generateNoise() {
    auto randomizer(Randomizer::pointer());
    auto settings(Settings::pointer());
    Clamper clamper;

    auto noiseFactor(std::abs(settings->getValueOf<float>("noiseFactor")));
    clamper.clamp<float>(noiseFactor, 0.f, 1.f);

    for (auto i(0); i < heightMap_.size(); ++i) {
        int randomDiff(randomizer->random(-noiseFactor*255,noiseFactor*255));
        auto newVal(static_cast<int>(heightMap_[i] + randomDiff));
        clamper.clamp<int>(newVal, 0, 255);
        heightMap_[i] = newVal;
    }
}
