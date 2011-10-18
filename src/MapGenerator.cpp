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
    createMountains();
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

void MapGenerator::createMountains() {
    auto settings(Settings::pointer());
    auto randomizer(Randomizer::pointer());

    auto width(settings->getValueOf<int>("width"));
    auto height(settings->getValueOf<int>("height"));
    auto maxHeight(settings->getValueOf<float>("maxHeight"));
    auto baseNiveau(settings->getValueOf<float>("baseNiveau"));

    Clamper clamper;
    clamper.clamp<float>(maxHeight, baseNiveau, 1.f);

    int mountainRange(randomizer->random(0, width/2));
    int currentPeekIndex(randomizer->random(0, width-1));

    std::cout<<currentPeekIndex<<std::endl;

    for (int y(0); y < height; ++y) {
        int rangeDiff(randomizer->random(-mountainRange/4, mountainRange/4));
        heightMap_[currentPeekIndex + y*width] = 255*maxHeight;
        int start(currentPeekIndex - (mountainRange + rangeDiff));
        int end(currentPeekIndex + (mountainRange - rangeDiff));
        clamper.clamp<int>(start, 0, width);
        clamper.clamp<int>(end, 0, width);
        for (int x(start); x < end; ++x) {
            if (x!=currentPeekIndex)
                heightMap_[x+y*width] = randomizer->random(baseNiveau, maxHeight)*255 ;
        }
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
