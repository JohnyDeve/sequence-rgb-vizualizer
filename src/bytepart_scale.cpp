#include "srgbviz/colormap/bytepart_scale.hpp"

namespace srgbviz::colormap {

types::RGB BytePartScale::map(long v) const {
    return {get_byte_part(v, 0), get_byte_part(v, 1), get_byte_part(v, 2)};
}

uint8_t BytePartScale::get_byte_part(long v, int byte_inx) const {
    return (((v >> 8 * byte_inx) % 256) + 256) % 256; 
}

}