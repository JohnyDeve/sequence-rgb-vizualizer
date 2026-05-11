#ifndef SRGBVIZ_COLORMAP_ICOLORMAP_HPP
#define SRGBVIZ_COLORMAP_ICOLORMAP_HPP

#include "srgbviz/types/rgb.hpp"

namespace srgbviz::colormap {

template<typename T>
class IColorMap {
public:
    virtual types::RGB map(T value) const = 0;
    virtual ~IColorMap() = default;
};

}

#endif // SRGBVIZ_COLORMAP_ICOLORMAP_HPP