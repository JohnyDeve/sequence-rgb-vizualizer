#include "srgbviz/colormap/grayscale.hpp"

namespace srgbviz::colormap {

GrayscaleMap::GrayscaleMap(double min, double max)
    : min_(min), max_(max) {}

types::RGB GrayscaleMap::map(double v) const {
    double normalized = (v - min_) / (max_ - min_);
    uint8_t c = static_cast<uint8_t>(normalized * 255);
    return {c, c, c};
}

}