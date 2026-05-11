#include <iostream>
#include <vector>
#include <memory>

#include "srgbviz/datasource/vector_source.hpp"
#include "srgbviz/colormap/bytepart_scale.hpp"
#include "srgbviz/pipeline/generator.hpp"

#include "srgbviz/io/ppm_writer.hpp"
#include "srgbviz/io/console_writer.hpp"
#include "srgbviz/io/ppm_writer_stream.hpp"

int main() {
    try {
        std::vector<double> data = {
            5630, 17, 4558363, 100000,
            345678934, 45513, 33331, 0,
            2313, 7252, 135135135, 55555521,
            65536, 117725387, 576432, 7
        };

        const std::size_t width = 4;
        const std::size_t height = 4;

        if (data.size() != width * height) {
            std::cerr << "Error: data size does not match dimensions\n";
            return 1;
        }
        
        srgbviz::datasource::VectorSource<double> src(data);
        srgbviz::colormap::BytePartScale cmap;

        srgbviz::SequenceRGBGenerator generator(src, cmap);
        const std::vector<srgbviz::types::RGB> pixels = generator.generate();

        const std::string output_file = "output1.ppm";
        srgbviz::io::PPMWriter::write(output_file, pixels, width, height);

        srgbviz::io::ConsoleWriter::write(pixels, width);

        srgbviz::io::PPMWriterStream ppm_wstream("output2.ppm", width, height);
        for (size_t i = 0; i < src.size(); i++) {
            auto rgb = cmap.map(src.get(i));
            ppm_wstream.write_pixel(rgb);
        }

        std::cout << "Finished [OK] \n";

    } catch (const std::exception& e) {
        std::cout << "Finished [ERROR] \n";
        std::cerr << "Unhandled exception: " << e.what() << '\n';
        return 1;
    }

    return 0;
}