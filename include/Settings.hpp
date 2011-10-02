# ifndef SETTINGS_HPP
# define SETTINGS_HPP

# include "include/KnownObject.hpp"

# include <string>
# include <map>

class Settings: public KnownObject<Settings> {
    public:
        enum SettingsType {ST_INT, ST_BOOL, ST_FLOAT, ST_STRING, ST_NONE};

        Settings();

        void load(std::string const& fileName);

        template <typename T>
        T const getValueOf(char const* varName) const {
            std::string const& tmp(varName);
            return getValueOf<T>(tmp);
        }


        template <typename T>
        T const getValueOf(std::string const& varName) const {
            auto searchedSetting(settings_.find(varName));
            if (searchedSetting != settings_.end()) {
                auto valPair(searchedSetting->second.getValue());
                auto valCast = reinterpret_cast<T const*>(valPair.first);
                if (valCast)
                    return *valCast;
                return T();
            }
            return T();
        }


        template <typename T>
        void setValueOf(char const* varName, T const& varVal);

        template <typename T>
        void setValueOf(std::string const& varName, T const& varVal);

    private:

        class GenericSetting {
            public:
                GenericSetting();
                GenericSetting(void* val);
                std::pair<void const*, SettingsType> getValue() const;

            private:

                SettingsType type;

                int         intVal;
                bool        boolVal;
                float       floatVal;
                std::string stringVal;
        };


        std::map<std::string, GenericSetting> settings_;

        void loadDefault();
        void loadFromFile(std::string const& fileName);
};

# endif //SETTINGS_HPP


