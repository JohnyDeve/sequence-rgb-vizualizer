#ifndef SRGBVIZ_DATASOURCE_MATRIX_SOURCE_HPP
#define SRGBVIZ_DATASOURCE_MATRIX_SOURCE_HPP

#include <vector>

#include "idatasource.hpp"

namespace srgbviz::datasource {

template<typename T>
class MatrixSource : public IDataSource<T> {
public:
    explicit MatrixSource(const std::vector<std::vector<T>>& data) : data_(data) {}

    T get(size_t inx) const override {
        return data_[inx / data_[0].size()][inx % data_[0].size()];
    }

    size_t size() const override {
        return data_.size() * data_[0].size();
    }

private:
    const std::vector<std::vector<T>>& data_;
};

}

#endif // SRGBVIZ_DATASOURCE_MATRIX_SOURCE_HPP