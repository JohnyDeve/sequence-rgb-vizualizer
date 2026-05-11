#include <fstream>

#include "srgbviz/io/ppm_writer.hpp"
#include "srgbviz/types/rgb.hpp"

namespace srgbviz::io {

void PPMWriter::write(
    const std::string& filename,
    const std::vector<types::RGB>& pixels,
    size_t width,
    size_t height
) {

    if (pixels.size() > width * height) {
        throw std::runtime_error("Too many pixels");
    }

    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Cannot open file");
    }

    file << "P6\n" << width << " " << height << "\n255\n";

    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(types::RGB));
}

}