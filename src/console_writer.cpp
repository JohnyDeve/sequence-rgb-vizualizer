#include "srgbviz/io/console_writer.hpp"

namespace srgbviz::io {

void ConsoleWriter::add_string_on_ptr(char** ptr, const char* src) {
    while (*src) {
        **ptr = *src;
        (*ptr)++;
        src++;
    }
}

void ConsoleWriter::parse_uint8_to_buff(char** ptr, uint8_t x) {
    if (x == 0) {
        **ptr = '0';
        (*ptr)++;
        return;
    }

    char result[4];
    char *res_ptr = result;

    while (x > 0) {
        *res_ptr++ = ('0' + (x % 10));
        x /= 10;
    }

    while (res_ptr > result) {
        res_ptr--;
        **ptr = *res_ptr;
        (*ptr)++;
    }
    return;
}

void ConsoleWriter::add_pixel_to_buff(char** ptr, uint8_t r, uint8_t g, uint8_t b) {

    add_string_on_ptr(ptr, "\x1b[38;2;");

    parse_uint8_to_buff(ptr, r);
    **ptr = ';';
    (*ptr)++;

    parse_uint8_to_buff(ptr, g);
    **ptr = ';';
    (*ptr)++;

    parse_uint8_to_buff(ptr, b);
    **ptr = 'm';
    (*ptr)++;
    
    **ptr = (char)0xE2;
    (*ptr)++;

    **ptr = (char)0x96;
    (*ptr)++;

    **ptr = (char)0x88;
    (*ptr)++;
}

void ConsoleWriter::write(const std::vector<types::RGB>& pixels, size_t width) {
    char buff[CONSOLE_BUFFER_SIZE];

    char *buf_st = buff;
    char *buf_end = buff + CONSOLE_BUFFER_SIZE;

    size_t pix_cnt = pixels.size();

    for (size_t i = 0; i < pix_cnt; i++) {
        const types::RGB& p = pixels[i];

        if (buf_st + SAFETY_SPACE >= buf_end) {
            fwrite(buff, 1, buf_st - buff, stdout);
            buf_st = buff;
        }

        add_pixel_to_buff(&buf_st, p.r, p.g, p.b);

        if ((i + 1) % width == 0) {
            *buf_st++ = '\n';
        }
    }

    add_string_on_ptr(&buf_st, "\x1b[0m");
    fwrite(buff, 1, buf_st - buff, stdout);
}

}