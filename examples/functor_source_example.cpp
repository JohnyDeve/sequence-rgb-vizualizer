#include <iostream>
#include <vector>
#include <memory>

#include "srgbviz/datasource/functor_source.hpp"
#include "srgbviz/colormap/bytepart_scale.hpp"
#include "srgbviz/pipeline/generator.hpp"

#include "srgbviz/io/ppm_writer.hpp"
#include "srgbviz/io/console_writer.hpp"
#include "srgbviz/io/ppm_writer_stream.hpp"

class FunctionPRG {
public:
    long long operator() () {
        seed = A * seed + B;
        return seed;
    }
private:
    long long seed = 2;
    long long A = 3;
    long long B = 16;
};

int main() {
    try {

        const std::size_t width = 256;
        const std::size_t height = 256;

        FunctionPRG func;

        srgbviz::datasource::FunctorSource<long long, FunctionPRG> src(func, width * height);
        srgbviz::colormap::BytePartScale cmap;

        srgbviz::SequenceRGBGenerator generator(src, cmap);
        const std::vector<srgbviz::types::RGB> pixels = generator.generate();

        const std::string output_file = "output1.ppm";
        srgbviz::io::PPMWriter::write(output_file, pixels, width, height);

        //srgbviz::io::ConsoleWriter::write(pixels, width);

        /*
        srgbviz::io::PPMWriterStream ppm_wstream("output2.ppm", width, height);
        for (size_t i = 0; i < src.size(); i++) {
            auto rgb = cmap.map(src.get(i));
            ppm_wstream.write_pixel(rgb);
        }
        */

        std::cout << "Finished [OK] \n";

    } catch (const std::exception& e) {
        std::cout << "Finished [ERROR] \n";
        std::cerr << "Unhandled exception: " << e.what() << '\n';
        return 1;
    }

    return 0;
}