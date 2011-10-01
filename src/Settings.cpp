# include "include/Settings.hpp"


# include <iostream>

Settings::Settings():
    settings_() {}

void Settings::load() {
    int  intVal(40);
    settings_["width"]=GenericSetting(&intVal);

    intVal = 40;
    settings_["height"]=GenericSetting(&intVal);

    intVal = 2;
    settings_["stepSizeX"]=GenericSetting(&intVal);

    intVal = 2;
    settings_["stepSizeY"]=GenericSetting(&intVal);

    float floatVal(1.f);
    settings_["maxHeight"]=GenericSetting(&floatVal);

    floatVal = 0.f;
    settings_["minHeight"]=GenericSetting(&floatVal);

    floatVal = 0.05f;
    settings_["maxHeightFrequency"]=GenericSetting(&floatVal);

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
