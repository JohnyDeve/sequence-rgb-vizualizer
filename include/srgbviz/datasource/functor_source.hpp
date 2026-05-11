#ifndef SRGBVIZ_DATASOURCE_FUNCTOR_SOURCE_HPP
#define SRGBVIZ_DATASOURCE_FUNCTOR_SOURCE_HPP

#include <vector>
#include <concepts>

#include <iostream>

#include "idatasource.hpp"
#include "detail/cache_source.hpp"

namespace srgbviz::datasource {

template<typename T, class Functor>
class FunctorSource : public IDataSource<T> {
public:
    FunctorSource(
        Functor func, 
        size_t size
    ) 
        : func_(func), 
        size_(size), 
        cache_(size) {}

    T get(size_t inx) const override {
        if (inx >= size_) { throw std::runtime_error("Out-of-bounds exception"); }
        if (cache_.is_contains(inx)) { return cache_.get_value(inx); }

        if constexpr (requires { func_(inx); }) {
            cache_.set_value(inx, func_(inx));
            return cache_.get_value(inx);
        } else if constexpr (requires { func_(); }) {
            do {
                cache_.set_value(cache_.get_capacity(), func_());
            } while (cache_.get_capacity() - 1 < inx);
            return cache_.get_value(inx);
        } else {
            static_assert((requires(Functor& f, size_t i) { f(i); }) || (requires(Functor& f) { f(); }), "Invalid functor, functor must have an overloaded operator() or operator(size_t inx)");
        }
    }

    size_t size() const override {
        return size_;
    }

private:
    mutable detail::CacheSource<T> cache_;
    mutable Functor func_;
    size_t size_;
};

}

#endif // SRGBVIZ_DATASOURCE_FUNCTOR_SOURCE_HPP