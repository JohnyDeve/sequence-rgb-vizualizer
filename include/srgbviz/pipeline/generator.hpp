#ifndef SRGBVIZ_GENERATOR_HPP
#define SRGBVIZ_GENERATOR_HPP

#include <vector>

#include "srgbviz/types/rgb.hpp"
#include "srgbviz/colormap/icolormap.hpp"
#include "srgbviz/datasource/idatasource.hpp"

namespace srgbviz {

template<typename T1, typename T2>
class SequenceRGBGenerator {
public:
    SequenceRGBGenerator(
        const datasource::IDataSource<T1>& src,
        const colormap::IColorMap<T2>& cmap
    );

    std::vector<types::RGB> generate();
private:
    const datasource::IDataSource<T1>& source_;
    const colormap::IColorMap<T2>& colormap_;
};

}

#include "detail/generator.tpp"

#endif // SRGBVIZ_GENERATOR_HPP