# include "include/BMPwriter.hpp"

# include "include/bitmap_image.hpp"

void BMPwriter::save(std::string const& fileName, std::vector<unsigned char> const& pixels, int width, int height) const{
    bitmap_image bmpFile(width, height);

    for (auto y(0); y <height; ++y) {
        for (auto x(0); x <width; ++x) {
            auto colorVal(pixels[x+y*width]);
            bmpFile.set_pixel(x, y, colorVal, colorVal, colorVal);
        }
    }

    bmpFile.save_image(fileName);
}
