#ifndef SRGBVIZ_DATASOURCE_CACHE_SOURCE_HPP
#define SRGBVIZ_DATASOURCE_CACHE_SOURCE_HPP

#include <vector>
#include <optional>

namespace srgbviz::datasource::detail {

template <typename T>
class CacheSource {
public:
    CacheSource(size_t size) : size_(size), data_(size), capacity_(0) {}

    bool is_contains(size_t inx) const noexcept {
        return inx < size_ && data_[inx].has_value();
    }

    void set_value(size_t inx, const T& value) noexcept {
        if (inx >= size_) { return; }
        if (!is_contains(inx)) { capacity_++; }
        data_[inx] = value;
        return;
    }
    
    const T& get_value(size_t inx) const noexcept {
        if (is_contains(inx)) { return *data_[inx]; };
        return empty_object;
    }

    size_t get_capacity() const noexcept {
        return capacity_;
    }

    size_t get_size() const noexcept {
        return size_;
    }
    
private:
    static inline const T empty_object{};
    std::vector<std::optional<T>> data_;
    size_t capacity_;
    size_t size_;
};

}

#endif // SRGBVIZ_DATASOURCE_CACHE_SOURCE_HPP