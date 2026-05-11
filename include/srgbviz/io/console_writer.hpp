#ifndef SRGBVIZ_IO_CONSOLE_WRITER_HPP
#define SRGBVIZ_IO_CONSOLE_WRITER_HPP

#include <vector>

#include "srgbviz/types/rgb.hpp"

namespace srgbviz::io {

class ConsoleWriter {
public:
    static void write(const std::vector<types::RGB>& pixels, size_t width);
private:
    static inline constexpr size_t CONSOLE_BUFFER_SIZE = 1 << 20;
    static inline constexpr size_t SAFETY_SPACE = 32;

    static void add_string_on_ptr(char** ptr, const char* src);
    static void add_pixel_to_buff(char** ptr, uint8_t r, uint8_t g, uint8_t b);
    static void parse_uint8_to_buff(char **ptr, uint8_t x);
};

}

#endif // SRGBVIZ_IO_CONSOLE_WRITER_HPP