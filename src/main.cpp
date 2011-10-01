# include "include/Randomizer.hpp"
# include "include/BMPwriter.hpp"
# include "include/MapGenerator.hpp"
# include "include/Settings.hpp"

int main() {
    Randomizer randomizer;
    randomizer.init();

    Settings settings;
    settings.load();

    BMPwriter writer;
    MapGenerator mapGen;
    writer.save("test.bmp", mapGen.generateHeightMap(), settings.getValueOf<int>("width"), settings.getValueOf<int>("height"));

    return 0;
}
