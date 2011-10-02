# include "include/MapGenerator.hpp"

# include "include/Settings.hpp"
# include "include/Randomizer.hpp"

# include <cmath>
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

    std::cout << "Generated height map." << std::endl;


    return heightMap_;
}

void MapGenerator::generateRandomPoints() {
    auto settings(Settings::pointer());
    auto stepX(settings->getValueOf<int>("stepSizeX"));
    auto stepY(settings->getValueOf<int>("stepSizeY"));
    auto maxHeightFrequency(settings->getValueOf<float>("maxHeightFrequency"));
    auto maxHeight(settings->getValueOf<float>("maxHeight"));
    auto minHeight(settings->getValueOf<float>("minHeight"));
    auto width(settings->getValueOf<int>("width"));
    auto height(settings->getValueOf<int>("height"));

    auto randomizer(Randomizer::pointer());

    for (int step(0); step < width; step += stepX) {
        if (randomizer->random(0.f, 1.f) <= maxHeightFrequency)
            randomPointsHor_.insert(std::make_pair(step,maxHeight));
        else randomPointsHor_.insert(std::make_pair(step,randomizer->random(minHeight, maxHeight)));
    }

    for (int step(0); step < height; step += stepY) {
        if (randomizer->random(0.f, 1.f) <= maxHeightFrequency)
            randomPointsVer_[step] = maxHeight;
        else randomPointsVer_[step] = randomizer->random(minHeight, maxHeight);
    }

}

void MapGenerator::interpolate() {
    auto settings(Settings::pointer());

    auto width(settings->getValueOf<int>("width"));
    auto height(settings->getValueOf<int>("height"));

    ///Setting up a new vector for interpolation by filling it with the random values calculated already.

    std::vector<float> interpolatedPointsHor(width);

    for (auto point : randomPointsHor_)
        interpolatedPointsHor[point.first] = point.second;

    ///Interpolate between the random values.

    for (int i(0); i < interpolatedPointsHor.size(); ++i)
        if (interpolatedPointsHor[i] == 0)
            interpolatedPointsHor[i] = lagrange(i, randomPointsHor_);

    ///Set the color values in the height map

    auto heightDiff(std::fabs(settings->getValueOf<float>("maxHeight")-settings->getValueOf<float>("minHeight")));
    auto relHeightDiff(255 / (heightDiff > 0.f ? heightDiff : 1.f));

    for (auto y(0); y <height; ++y) {
        for (auto x(0); x <width; ++x) {

            auto colorVal((interpolatedPointsHor[x] - settings->getValueOf<float>("minHeight")) * relHeightDiff);
            heightMap_[x+y*width] = colorVal;
        }
    }
}

float MapGenerator::lagrange(int x, std::map<int ,float> & points) const {
    float result(0.f);

    int count(0);
    for (auto point : points) {
        result += point.second * lagrangeBasis(x, count, points);
        ++count;
    }
    return result;
}

float MapGenerator::lagrangeBasis(int x, int x_i, std::map<int, float> const& points) const {
    float result(1.f);

    for (int j(0); j < points.size(); ++j) {
        if (x_i != j) {
            result *= (x-j)/(x_i-j);
        }
    }

    return result;
}

