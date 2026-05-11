#include "srgbviz/io/ppm_writer_stream.hpp"

namespace srgbviz::io {

PPMWriterStream::PPMWriterStream(
    const std::string& filename,
    size_t width,
    size_t height,
    types::RGB fill
) 
    : file_(filename, std::ios::binary), 
    width_(width), 
    height_(height), 
    total_pixels_(width * height),
    written_(0),
    fill_color_(fill),
    buffer_(PPM_BUFFER_SIZE),
    buffer_pos_(0),
    finalized_(false)
{
    if (!file_) {
        throw std::runtime_error("Failed to open file");
    }

    file_ << "P6\n" << width_ << " " << height_ << "\n255\n";
}

PPMWriterStream::PPMWriterStream(
    const std::string& filename,
    size_t width,
    size_t height
) : PPMWriterStream(filename, width, height, DEFAULT_COLOR) {}

void PPMWriterStream::write_pixel(const types::RGB& pixel) {
    if (finalized_) {
        throw std::runtime_error("Write after finalize");
    }

    if (written_ >= total_pixels_) {
        throw std::runtime_error("Too many pixels written");
    }

    if (buffer_pos_ + sizeof(types::RGB) > buffer_.size()) {
        flush_buffer();
    }

    std::memcpy(&buffer_[buffer_pos_], &pixel, sizeof(types::RGB));
    buffer_pos_ += sizeof(types::RGB);

    ++written_;
}

void PPMWriterStream::finalize() {
    if (finalized_) return;

    while (written_ < total_pixels_) {
        write_pixel(fill_color_);
    }

    flush_buffer();

    finalized_ = true;
}

void PPMWriterStream::flush_buffer() {
    if (buffer_pos_ > 0) {
        file_.write(buffer_.data(), buffer_pos_);
        buffer_pos_ = 0;
    }
}

PPMWriterStream::~PPMWriterStream() {
    try {
        finalize();
    } catch (const std::exception& e) {
        fprintf(stderr, "[WARNING]: %s\n", e.what());
    }
}

}