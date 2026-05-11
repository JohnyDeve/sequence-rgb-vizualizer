#include "srgbviz/io/video_writer.hpp"

#include <stdexcept>
#include <sstream>
#include <iostream>

namespace srgbviz::io {

VideoWriter::VideoWriter(const Settings& settings)
    : settings_(settings) {
    open_pipe();
}

VideoWriter::~VideoWriter() {
    if (!finalized_) {
        try {
            finalize();
        } catch (...) {
            std::cout << "[CRITICAL EXCEPTION]" << "\n";
        }
    }
}

VideoWriter::VideoWriter(VideoWriter&& other) noexcept {
    *this = std::move(other);
}

VideoWriter& VideoWriter::operator=(VideoWriter&& other) noexcept {
    if (this != &other) {
        pipe_ = other.pipe_;
        settings_ = std::move(other.settings_);
        finalized_ = other.finalized_;
        error_ = other.error_;

        other.pipe_ = nullptr;
        other.finalized_ = true;
    }
    return *this;
}

bool VideoWriter::is_open() const noexcept {
    return pipe_ != nullptr;
}

void VideoWriter::open_pipe() {
    std::string cmd = build_ffmpeg_command();

    pipe_ = popen(cmd.c_str(), "w");
    if (!pipe_) {
        throw std::runtime_error("VideoWriter: failed to open ffmpeg pipe");
    }

    setvbuf(pipe_, nullptr, _IOFBF, 1 << 20);
}

std::string VideoWriter::build_ffmpeg_command() const {
    std::ostringstream cmd;

    cmd << "ffmpeg ";

    if (settings_.overwrite) {
        cmd << "-y ";
    }

    cmd << "-f rawvideo "
        << "-pixel_format " << settings_.pixel_format << " "
        << "-video_size " << settings_.width << "x" << settings_.height << " "
        << "-framerate " << settings_.fps << " "
        << "-i - ";

    for (const auto& flag : settings_.extra_flags) {
        cmd << flag << " ";
    }

    cmd << "-c:v " << settings_.codec << " "
        << "-pix_fmt yuv420p "
        << settings_.output_file;

    return cmd.str();
}

void VideoWriter::write_data(const void* data, std::size_t size, std::size_t count) {
    if (!pipe_ || finalized_) throw std::runtime_error("VideoWriter: write on closed pipe");
    
    if (std::fwrite(data, size, count, pipe_) != count) {
        error_ = true;
        throw std::runtime_error("VideoWriter: failed to write frame");
    }
}

void VideoWriter::write_frame(const types::RGB* pixel, std::size_t pixel_count) {
    write_data(pixel, sizeof(types::RGB), pixel_count);
}

void VideoWriter::write_frame(const std::uint8_t* data, std::size_t size) {
    write_data(data, 1, size);
}

void VideoWriter::finalize() {
    if (finalized_) return;
    finalized_ = true;

    if (!pipe_) return;
    std::fflush(pipe_);
    int status = pclose(pipe_);
    pipe_ = nullptr;

    if (status != 0 || error_) {
        throw std::runtime_error("VideoWriter: ffmpeg process failed");
    }
}

} 