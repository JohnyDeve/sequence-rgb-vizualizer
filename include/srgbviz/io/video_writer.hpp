#ifndef SRGBVIZ_IO_VIDEO_WRITER_HPP
#define SRGBVIZ_IO_VIDEO_WRITER_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "srgbviz/types/rgb.hpp"

namespace srgbviz::io {

class VideoWriter {
public:
    struct Settings {
        std::size_t width = 256;
        std::size_t height = 256;
        int fps = 30;

        std::string pixel_format = "rgb24";
        std::string codec = "libx264";
        std::string output_file = "output.mp4";

        std::vector<std::string> extra_flags;

        bool overwrite = true;
    };

public:
    explicit VideoWriter(const Settings& settings);

    VideoWriter(const VideoWriter&) = delete;
    VideoWriter& operator=(const VideoWriter&) = delete;

    VideoWriter(VideoWriter&& other) noexcept;
    VideoWriter& operator=(VideoWriter&& other) noexcept;

    ~VideoWriter();

public:
    void write_frame(const types::RGB* pixel, std::size_t pixel_count);
    void write_frame(const std::uint8_t* data, std::size_t size);
    bool is_open() const noexcept;

    void finalize();

private:
    void open_pipe();
    std::string build_ffmpeg_command() const;
    void write_data(const void* data, std::size_t size, std::size_t count);

private:
    Settings settings_;
    FILE* pipe_ = nullptr;

    bool finalized_ = false;
    bool error_ = false;
};

}

#endif