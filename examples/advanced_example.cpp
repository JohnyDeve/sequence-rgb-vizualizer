#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>


#include <srgbviz/datasource/functor_source.hpp>
#include <srgbviz/colormap/grayscale.hpp>
#include <srgbviz/pipeline/generator.hpp>
#include <srgbviz/io/ppm_writer.hpp>
#include <srgbviz/io/console_writer.hpp>
#include <srgbviz/io/video_writer.hpp>

void static_heatmap() {
    const std::size_t width = 128;
    const std::size_t height = 128;

    auto func = [=](std::size_t i) {
        std::size_t x = i % width;
        std::size_t y = i / width;

        double fx = static_cast<double>(x) / width;
        double fy = static_cast<double>(y) / height;

        return std::sin(10 * fx) * std::cos(10 * fy);
    };

    srgbviz::datasource::FunctorSource<double, decltype(func)> source(func, width * height);

    srgbviz::colormap::GrayscaleMap cmap(-1, 1);

    srgbviz::SequenceRGBGenerator generator(source, cmap);

    auto pixels = generator.generate();

    srgbviz::io::PPMWriter::write("heatmap.ppm", pixels, width, height);

    std::cout << "[OK] heatmap.ppm generated\n";
}

void console_animation() {
    const std::size_t width = 64;
    const std::size_t height = 32;

    srgbviz::colormap::GrayscaleMap cmap(-1, 1);

    for (int frame = 0; frame < 200; ++frame) {
        double t = frame * 0.05;

        auto func = [=](std::size_t i) {
            std::size_t x = i % width;
            std::size_t y = i / width;

            double fx = static_cast<double>(x) / width;
            double fy = static_cast<double>(y) / height;

            return std::sin(10 * fx + t) * t + std::cos(10 * fy + t);
        };

        srgbviz::datasource::FunctorSource<double, decltype(func)> source(func, width * height);

        srgbviz::SequenceRGBGenerator generator(source, cmap);
        auto pixels = generator.generate();

        std::cout << "\033[2J\033[H";
        srgbviz::io::ConsoleWriter::write(pixels, width);
        std::cout << std::flush;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void file_cube_animation() {
    const std::size_t width = 1920;
    const std::size_t height = 1080;
    const int frames = 300;
    const int fps = 35;

    srgbviz::colormap::GrayscaleMap cmap(-1, 1);

    /*std::string command =
        "ffmpeg -y -f rawvideo "
        "-pixel_format rgb24 "
        "-video_size " + std::to_string(width) + "x" + std::to_string(height) + " "
        "-framerate " + std::to_string(fps) + " "
        "-i - "
        "-vf vflip "
        "-c:v libx264rgb "
        "-crf 0 "
        "-preset ultrafast "
        "-pix_fmt rgb24 "
        "out14.mp4";*/

    std::string command =
        "ffmpeg -y -f rawvideo "
        "-pixel_format rgb24 "
        "-video_size " + std::to_string(width) + "x" + std::to_string(height) + " "
        "-framerate " + std::to_string(fps) + " "
        "-i - "
        "-vf vflip "
        "-c:v libx264 -pix_fmt yuv420p out14.mp4";

    FILE* pipe = popen(command.c_str(), "w");
    if (!pipe) {
        throw std::runtime_error("ffmpeg pipe error");
    }
    
    setvbuf(pipe, nullptr, _IOFBF, 1 << 20);

    struct Vec3 { double x,y,z; };

    auto rotate = [](Vec3 v, double t) {
        // X
        double cy = cos(t), sy = sin(t);
        double y = v.y*cy - v.z*sy;
        double z = v.y*sy + v.z*cy;

        // Y
        double cx = cos(t*0.7), sx = sin(t*0.7);
        double x = v.x*cx + z*sx;
        z = -v.x*sx + z*cx;

        // Z
        double cz = cos(t*0.5), sz = sin(t*0.5);
        double x2 = x*cz - y*sz;
        double y2 = x*sz + y*cz;

        return Vec3{x2,y2,z};
    };

    std::vector<Vec3> vertices = {
        {-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},
        {-1,-1, 1},{1,-1, 1},{1,1, 1},{-1,1, 1}
    };

    std::vector<std::array<int,3>> tris = {
        {0,1,2},{0,2,3},
        {4,5,6},{4,6,7},
        {0,1,5},{0,5,4},
        {2,3,7},{2,7,6},
        {1,2,6},{1,6,5},
        {0,3,7},{0,7,4}
    };

    for (int frame = 0; frame < frames; ++frame) {
        double t = frame * 0.04;

        std::vector<Vec3> tv(8);
        for (int i=0;i<8;++i) tv[i] = rotate(vertices[i], t);

        std::vector<double> zbuf(width * height, 1e9);
        std::vector<double> intensity(width * height, 0.0);

        for (auto& tri : tris) {
            Vec3 v0 = tv[tri[0]];
            Vec3 v1 = tv[tri[1]];
            Vec3 v2 = tv[tri[2]];

            // нормаль
            Vec3 u{v1.x-v0.x, v1.y-v0.y, v1.z-v0.z};
            Vec3 v{v2.x-v0.x, v2.y-v0.y, v2.z-v0.z};

            Vec3 n{
                u.y*v.z - u.z*v.y,
                u.z*v.x - u.x*v.z,
                u.x*v.y - u.y*v.x
            };

            double len = std::sqrt(n.x*n.x+n.y*n.y+n.z*n.z);
            if (len == 0) continue;
            n.x/=len; n.y/=len; n.z/=len;

            Vec3 view_dir{0, 0, -1};

            // dot(normal, view_dir)
            double facing = n.x*view_dir.x + n.y*view_dir.y + n.z*view_dir.z;

            if (facing <= 0) continue;

            // проекция
            auto proj = [&](Vec3 p) {
                double camera_distance = 4.0;
                p.z += camera_distance;
                double f = 1.0 / p.z;

                return Vec3{
                    (p.x * f * 0.5 + 0.5) * width,
                    (p.y * f * 0.5 + 0.5) * height,
                    p.z
                };
            };

            Vec3 p0 = proj(v0);
            Vec3 p1 = proj(v1);
            Vec3 p2 = proj(v2);

            int minx = std::max(0, (int)std::floor(std::min({p0.x,p1.x,p2.x})));
            int maxx = std::min((int)width-1, (int)std::ceil(std::max({p0.x,p1.x,p2.x})));
            int miny = std::max(0, (int)std::floor(std::min({p0.y,p1.y,p2.y})));
            int maxy = std::min((int)height-1, (int)std::ceil(std::max({p0.y,p1.y,p2.y})));

            auto edge = [](Vec3 a, Vec3 b, double x, double y) {
                return (x - a.x)*(b.y - a.y) - (y - a.y)*(b.x - a.x);
            };

            for (int y = miny; y <= maxy; ++y) {
                for (int x = minx; x <= maxx; ++x) {

                    double w0 = edge(p1,p2,x,y);
                    double w1 = edge(p2,p0,x,y);
                    double w2 = edge(p0,p1,x,y);

                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                        (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                        double area = edge(p0,p1,p2.x,p2.y);
                        if (area == 0) continue;

                        double inv_area = 1.0 / area;
                        w0 *= inv_area;
                        w1 *= inv_area;
                        w2 *= inv_area;

                        double z = w0*p0.z + w1*p1.z + w2*p2.z;

                        int idx = y*width + x;
                        if (z < zbuf[idx]) {
                            zbuf[idx] = z;

                            // освещение
                            double light = std::max(0.0, -(n.z));
                            intensity[idx] = light;
                        }
                    }
                }
            }
        }

        auto func = [&](std::size_t i) {
            return intensity[i];
        };

        srgbviz::datasource::FunctorSource<double, decltype(func)>
            source(func, width * height);

        srgbviz::SequenceRGBGenerator generator(source, cmap);
        auto pixels = generator.generate();

        static_assert(sizeof(srgbviz::types::RGB) == 3);
        std::fwrite(pixels.data(), sizeof(srgbviz::types::RGB), pixels.size(), pipe);
    }

    pclose(pipe);

    std::cout << "[OK] 3D CUBE generated\n";
}

/*
double w = static_cast<double>(width);
            double h = static_cast<double>(height);

            double centerX = (w / 2.0) + std::sin(n) * 400;
            double centerY = (h / 2.0) + std::cos(n) * 500 * (3.0/7.0);

            double x = i % width;
            double y = i / width;

            return (std::pow((x - (centerX)), 2) + std::pow((y - (centerY)), 2) <= std::pow(70, 2)) 
            || ((std::pow(x - (w / 2.0), 2) * (3.0/7.0) + std::pow(y - (h / 2.0), 2)) <= std::pow(115, 2));
*/



void file_animation() {
    const std::size_t width = 1920;
    const std::size_t height = 1080;
    const int fps = 30;
    const int frames = 600;

    double t = -30;
    double n = 0;

    srgbviz::io::VideoWriter::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.output_file = "out17.mp4";

    srgbviz::io::VideoWriter vw(settings);

    for (int frame = 0; frame < frames; ++frame) {

        auto func = [=](std::size_t i) {

            double w = static_cast<double>(width);
            double h = static_cast<double>(height);

            double centerX = (w / 2.0);
            double centerY = (h / 2.0);

            double cX1 = centerX;
            double cY1 = centerY + 20;

            double cX2 = centerX + 20;
            double cY2 = centerY;

            double cX3 = centerX - 20;
            double cY3 = centerY;

            double x = i % width;
            double y = i / width;

            

            return ;
        };

        srgbviz::colormap::GrayscaleMap cmap(0, 1);
        srgbviz::datasource::FunctorSource<double, decltype(func)> source(func, width * height);

        srgbviz::SequenceRGBGenerator generator(source, cmap);
        auto pixels = generator.generate();
        

        vw.write_frame(pixels.data(), pixels.size());

        t += 1;
        n += (M_PI / 44.0);

    }

    vw.finalize();

    std::cout << "[OK] frames generated (frame_000.ppm ...)\n";
}

int main() {
    try {
        std::cout << "Running advanced examples...\n";

        //static_heatmap();
        //console_animation();
        file_animation();
        //file_cube_animation();

        std::cout << "Done.\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}