# include "include/Settings.hpp"

# include <iostream>
# include <fstream>
# include <sstream>

Settings::Settings():
    settings_() {}

void Settings::load(std::string const& fileName) {
    loadDefault();
    loadFromFile(fileName);
}

template <typename T>
void Settings::setValueOf(char const* varName, T const& varVal) {
    setValueOf<T>(std::string(varName));
}

template <typename T>
void Settings::setValueOf(std::string const& varName, T const& varVal) {
    settings_[varName] = GenericSetting(&varVal);
}


//////////// GenericSetting //////////////
Settings::GenericSetting::GenericSetting():
    type(ST_NONE),
    intVal(0),
    boolVal(false),
    floatVal(0.f),
    stringVal(" ") {}



Settings::GenericSetting::GenericSetting(void* val):
    type(ST_NONE),
    intVal(0),
    boolVal(false),
    floatVal(0.f),
    stringVal(" ") {

    auto intCast(static_cast<int*>(val));
    auto boolCast(static_cast<bool*>(val));
    auto floatCast(static_cast<float*>(val));
    auto stringCast(static_cast<std::string*>(val));

    if (intCast) {
        intVal = *intCast;
        type = ST_INT;
    }

    else if (boolCast) {
        boolVal = *boolCast;
        type = ST_BOOL;
    }

    else if (floatCast) {
        floatVal = *floatCast;
        type = ST_FLOAT;
    }

    else if (stringCast) {
        stringVal = (*stringCast);
        type = ST_STRING;
    }
}

std::pair<void const*, Settings::SettingsType> Settings::GenericSetting::getValue() const {

    switch (type) {
        case ST_INT:
            return std::make_pair(&intVal, type);
        case ST_BOOL:
            return std::make_pair(&boolVal, type);
        case ST_FLOAT:
            return std::make_pair(&floatVal, type);
        case ST_STRING:
            return std::make_pair(&stringVal, type);
        default: return std::make_pair(nullptr, ST_NONE);
    }
}

void Settings::loadDefault() {
    std::cout << "Loading default settings as fallback..." << std::endl;

    int  intVal(80);
    settings_["width"]=GenericSetting(&intVal);

    intVal = 80;
    settings_["height"]=GenericSetting(&intVal);

    float floatVal(1.f);
    settings_["baseNiveau"]=GenericSetting(&floatVal);

    intVal = 40;
    settings_["noiseFactor"]=GenericSetting(&intVal);

    std::cout << "Default settings loaded." << std::endl;

}

void Settings::loadFromFile(std::string const& fileName) {
    std::cout << "Loading settings from " << fileName <<"..." << std::endl;


    std::fstream file(fileName);

    if (!file)
        std::cout<<"Unable to open "<<fileName<<"!"<<std::endl;

    else {
        std::string line;
        while (std::getline(file, line)) {

            std::stringstream lineStream(line);

            ///Ignore comments.

            if (line.find("//") == std::string::npos) {

                std::string varName;
                lineStream >> varName;

                std::string varType;
                lineStream >> varType;

                if (varType == "int" ) {
                    int intVal;
                    lineStream >> intVal;
                    settings_[varName] = GenericSetting(&intVal);
                }

                else if (varType == "bool" ) {
                    bool boolVal;
                    std::string tmp;
                    lineStream >> tmp;
                    tmp == "false" ? boolVal = false : boolVal = true;
                    settings_[varName] = GenericSetting(&boolVal);
                }

                else if (varType == "float" ) {
                    float floatVal;
                    lineStream >> floatVal;
                    settings_[varName] = GenericSetting(&floatVal);
                }

                else if (varType == "string" ) {
                    std::string stringVal;
                    lineStream >> stringVal;
                    settings_[varName] = GenericSetting(&stringVal);
                }

            }
        }

        std::cout << "Loaded settings from " << fileName <<"." << std::endl;

    }
}
