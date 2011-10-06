# ifndef MAP_GENERATOR_HPP
# define MAP_GENERATOR_HPP

# include <vector>
# include <map>

class MapGenerator {
    public:
        MapGenerator();
        std::vector<unsigned char> const generateHeightMap();

    private:
        std::vector<unsigned char> heightMap_;
        std::map<int, float> randomPointsHor_;
        std::map<int, float> randomPointsVer_;

        void generateRandomPoints();

        void interpolate();

        float linear(int x, std::map<int, float> const& points) const;
        void generateNoise();
};


# endif //MAP_GENERATOR_HPP

