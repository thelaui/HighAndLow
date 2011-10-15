# ifndef CLAMPER_HPP
# define CLAMPER_HPP


class Clamper {
    public:
        Clamper() {}

        template <typename T>
        void clamp(T& value, T const& minVal, T const& maxVal) const {
            if (value < minVal)
                value = minVal;
            else if (value > maxVal)
                value = maxVal;
        }

};


# endif //CLAMPER_HPP


