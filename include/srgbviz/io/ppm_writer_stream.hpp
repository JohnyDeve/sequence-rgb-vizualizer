#ifndef SRGBVIZ_IO_PPM_WRITER_STREAM_HPP
#define SRGBVIZ_IO_PPM_WRITER_STREAM_HPP

#include <fstream>
#include <vector>
#include <cstddef>

#include "srgbviz/types/rgb.hpp"

namespace srgbviz::io {

class PPMWriterStream {
public:
    PPMWriterStream(
        const std::string& filename, 
        size_t width, 
        size_t height, 
        types::RGB fill
    );

    PPMWriterStream(
        const std::string& filename, 
        size_t width,
        size_t height
    );

    PPMWriterStream(const PPMWriterStream&) = delete;
    PPMWriterStream& operator=(const PPMWriterStream&) = delete;

    PPMWriterStream(PPMWriterStream&&) noexcept = default;
    PPMWriterStream& operator=(PPMWriterStream&&) noexcept = default;

    void write_pixel(const types::RGB& pixel);

    void finalize();

    ~PPMWriterStream();
private:
    static inline constexpr size_t PPM_BUFFER_SIZE = 64 * 1024;
    static inline constexpr types::RGB DEFAULT_COLOR = {255, 255, 255};

    std::ofstream file_;
    size_t width_;
    size_t height_;
    size_t total_pixels_;
    size_t written_;
    types::RGB fill_color_;
    std::vector<char> buffer_;
    size_t buffer_pos_;
    bool finalized_;

    void flush_buffer();
};

}

#endif // SRGBVIZ_IO_PPM_WRITER_STREAM_HPP