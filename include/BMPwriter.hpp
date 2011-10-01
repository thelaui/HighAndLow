# ifndef BMP_WRITER_HPP
# define BMP_WRITER_HPP

# include <vector>
# include <string>

class BMPwriter {
    public:

        void save(std::string const& fileName, std::vector<unsigned char> const& pixels, int width, int height) const;

};


# endif //BMP_WRITER_HPP
