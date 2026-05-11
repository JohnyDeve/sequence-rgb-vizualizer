#ifndef SRGBVIZ_COLORMAP_BYTEPART_SCALE_HPP
#define SRGBVIZ_COLORMAP_BYTEPART_SCALE_HPP

#include "icolormap.hpp"

namespace srgbviz::colormap {

class BytePartScale : public IColorMap<long> {
public:
    BytePartScale() = default;

    types::RGB map(long v) const override;
private:
    uint8_t get_byte_part(long v, int byte_inx) const; 
};

}

#endif // SRGBVIZ_COLORMAP_BYTEPART_SCALE_HPP