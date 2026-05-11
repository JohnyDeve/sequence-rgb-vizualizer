#ifndef SRGBVIZ_IO_PPM_WRITER_HPP
#define SRGBVIZ_IO_PPM_WRITER_HPP

#include <string>
#include <vector>

#include "srgbviz/types/rgb.hpp"

namespace srgbviz::io {

class PPMWriter {
public:
    static void write(
        const std::string& filename,
        const std::vector<types::RGB>& pixels,
        size_t width,
        size_t height
    );
};

}

#endif // SRGBVIZ_IO_PPM_WRITER_HPP