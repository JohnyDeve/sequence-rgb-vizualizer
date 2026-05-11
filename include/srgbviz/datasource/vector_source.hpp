#ifndef SRGBVIZ_DATASOURCE_VECTOR_SOURCE_HPP
#define SRGBVIZ_DATASOURCE_VECTOR_SOURCE_HPP

#include <vector>

#include "idatasource.hpp"

namespace srgbviz::datasource {

template<typename T>
class VectorSource : public IDataSource<T> {
public:
    explicit VectorSource(const std::vector<T>& data) : data_(data) {}

    T get(size_t inx) const override {
        return data_[inx];
    }

    size_t size() const override {
        return data_.size();
    }

private:
    const std::vector<T>& data_;
};

}

#endif // SRGBVIZ_DATASOURCE_VECTOR_SOURCE_HPP