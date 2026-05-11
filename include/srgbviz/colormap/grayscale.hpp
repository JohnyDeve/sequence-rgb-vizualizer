#ifndef SRGBVIZ_COLORMAP_GRAYSCALE_HPP
#define SRGBVIZ_COLORMAP_GRAYSCALE_HPP

#include "icolormap.hpp"

namespace srgbviz::colormap {

class GrayscaleMap : public IColorMap<double> {
public:
    GrayscaleMap(double min, double max);

    types::RGB map(double v) const override;
private:
    double min_, max_;
};

}

#endif // SRGBVIZ_COLORMAP_GRAYSCALE_HPP