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

    auto settings(Settings::pointer());
    generateRandomPoints();
    interpolate();
    generateNoise();

    std::cout << "Generated height map." << std::endl;


    return heightMap_;
}

void MapGenerator::generateRandomPoints() {
    auto settings(Settings::pointer());
    auto stepXMin(settings->getValueOf<int>("stepSizeXMin"));
    auto stepXMax(settings->getValueOf<int>("stepSizeXMax"));
    auto stepYMin(settings->getValueOf<int>("stepSizeYMin"));
    auto stepYMax(settings->getValueOf<int>("stepSizeYMax"));
    auto maxHeightFrequency(settings->getValueOf<float>("maxHeightFrequency"));
    auto maxHeight(settings->getValueOf<float>("maxHeight"));
    auto minHeight(settings->getValueOf<float>("minHeight"));
    auto width(settings->getValueOf<int>("width"));
    auto height(settings->getValueOf<int>("height"));

    auto randomizer(Randomizer::pointer());

    for (int step(0); step < width; step += randomizer->random(stepXMin, stepXMax)) {
        float height(0.f);
        if (randomizer->random(0.f, 1.f) <= maxHeightFrequency)
            height = maxHeight;
        else height = randomizer->random(minHeight, maxHeight);

        randomPointsHor_.insert(std::make_pair(step,height));
    }

    randomPointsHor_.insert(std::make_pair(width-1,randomizer->random(minHeight, maxHeight)));



    for (int step(0); step < height; step += randomizer->random(stepYMin, stepYMax)) {
        float height(0.f);
        if (randomizer->random(0.f, 1.f) <= maxHeightFrequency)
            height = maxHeight;
        else height = randomizer->random(minHeight, maxHeight);

        randomPointsVer_.insert(std::make_pair(step,height));
    }

    randomPointsVer_.insert(std::make_pair(height-1,randomizer->random(minHeight, maxHeight)));

}

void MapGenerator::interpolate() {
    auto settings(Settings::pointer());

    auto width(settings->getValueOf<int>("width"));
    auto height(settings->getValueOf<int>("height"));

    ///Setting up a new vector for interpolation by filling it with the random values calculated already.

    std::vector<float> interpolatedPointsHor(width);
    std::vector<float> interpolatedPointsVer(height);

    for (auto point : randomPointsHor_)
        interpolatedPointsHor[point.first] = point.second;

    for (auto point : randomPointsVer_)
        interpolatedPointsVer[point.first] = point.second;

    ///Interpolate between the random values.

    for (int i(0); i < interpolatedPointsHor.size(); ++i)
        if (interpolatedPointsHor[i] == 0)
            interpolatedPointsHor[i] = linear(i, randomPointsHor_);

    for (int i(0); i < interpolatedPointsVer.size(); ++i)
        if (interpolatedPointsVer[i] == 0)
            interpolatedPointsVer[i] = linear(i, randomPointsVer_);

    ///Set the color values in the height map


    auto heightDiff(std::fabs(settings->getValueOf<float>("maxHeight")-settings->getValueOf<float>("minHeight")));
    auto relHeightDiff(255 / (heightDiff > 0.f ? heightDiff : 1.f));

    for (auto y(0); y <height; ++y) {
        for (auto x(0); x <width; ++x) {

            auto colorVal(((interpolatedPointsHor[x] + interpolatedPointsVer[y])*0.5f - settings->getValueOf<float>("minHeight")) * relHeightDiff);
            heightMap_[x+y*width] = colorVal;
        }
    }
}


float MapGenerator::linear(int x, std::map<int ,float> const& points) const {
    float result(0.f);
    auto lastPoint(points.begin());
    auto startPoint(points.begin());
    for (auto point(++startPoint); point != points.end(); ++point) {
        if (x > lastPoint->first && x < point->first) {
            result = lastPoint->second + ((point->second-lastPoint->second)/(point->first - lastPoint->first) * (x - lastPoint->first));
            break;
        }
        else lastPoint = point;
    }

    return result;
}

void MapGenerator::generateNoise() {
    auto randomizer(Randomizer::pointer());
    auto settings(Settings::pointer());

    auto noiseFactor(std::abs(settings->getValueOf<int>("noiseFactor")));

    for (auto i(0); i < heightMap_.size(); ++i) {
        int randomDiff(randomizer->random(-noiseFactor,noiseFactor));
        heightMap_[i]+= randomDiff;
    }
}
