# ifndef RANDOMIZER_HPP
# define RANDOMIZER_HPP

# include "include/KnownObject.hpp"

class Randomizer: public KnownObject<Randomizer> {
    public:
        void  init() const;

        float random(float begin, float end) const;
        int   random(int begin, int end)     const;
};


# endif //RANDOMIZER_HPP

