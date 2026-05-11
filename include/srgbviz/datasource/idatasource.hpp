#ifndef SRGBVIZ_DATASOURCE_IDATASOURCE_HPP
#define SRGBVIZ_DATASOURCE_IDATASOURCE_HPP

#include <cstddef>

namespace srgbviz::datasource {

template<typename T>
class IDataSource {
public:
    virtual T get(size_t inx) const = 0;
    virtual size_t size() const = 0;
    virtual ~IDataSource() = default;
};

}

#endif // SRGBVIZ_DATASOURCE_IDATASOURCE_HPP