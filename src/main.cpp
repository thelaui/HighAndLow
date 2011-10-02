# include "include/Randomizer.hpp"
# include "include/BMPwriter.hpp"
# include "include/MapGenerator.hpp"
# include "include/Settings.hpp"


# include <iostream>

void showErrorMessage();
void showHelpMessage();

int main(int argc, char* argv[]) {

    std::string fileName("settings.txt");
    std::string targetName("heightMap.bmp");

    if (argc > 2) {

        for (int argCount(1); argCount < argc; ++argCount) {
            std::string argVal(argv[argCount]);
            if (argVal == "--load" || argVal == "-l") {
                if ((argCount + 1) < argc) {
                    fileName = argv[argCount + 1];
                    ++argCount;
                }
                else showErrorMessage();
            }

            else if (argVal == "--out" || argVal == "-o") {
                if ((argCount + 1) < argc) {
                    targetName = argv[argCount + 1];
                    ++argCount;
                }
                else showErrorMessage();
            }

            else if (argVal == "--help" || argVal == "-h") {
                if ((argCount + 1) < argc) {
                    showHelpMessage();
                }
                else showErrorMessage();
            }
            else showErrorMessage();

        }
    }

    Randomizer randomizer;
    randomizer.init();

    Settings settings;
    settings.load(fileName);

    BMPwriter writer;
    MapGenerator mapGen;
    writer.save(targetName, mapGen.generateHeightMap(), settings.getValueOf<int>("width"), settings.getValueOf<int>("height"));


    while (true) {

        std::cout << "Type 'r' or 'repeat' to reload the settings and generate a new height map!" << std::endl;
        std::cout << "Type 'q' or 'quit' to escape HighAndLow!" << std::endl;


        std::string input;
        std::cin >> input;

        if (input == "r" || input == "repeat") {

            Settings settings;
            settings.load(fileName);

            BMPwriter writer;
            MapGenerator mapGen;
            writer.save(targetName, mapGen.generateHeightMap(), settings.getValueOf<int>("width"), settings.getValueOf<int>("height"));

        }

        else if (input == "q" || input == "quit")
            break;
    }

    return 0;
}

void showErrorMessage() {
    std::cerr << "Wrong usage of HighAndLow! Start with '--help' or '-h' to show available commands!"<<std::endl;
    std::exit(-1);
}

void showHelpMessage() {
    std::cout << "Usage: HighAndLow {[OPTION] [ARGUMENT]}*"<<std::endl;
    std::cout << "Options: "<<std::endl;
    std::cout << " -l | --load [FILENAME]             load settings from file"<<std::endl;
    std::cout << " -o | --out [FILENAME]              set target file to save the height map in"<<std::endl;
    std::cout << " -h | --help [FILENAME]             show this help message"<<std::endl;
    std::exit(0);
}
