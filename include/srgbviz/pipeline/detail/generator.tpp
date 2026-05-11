#ifndef SRGBVIZ_GENERATOR_TPP
#define SRGBVIZ_GENERATOR_TPP

namespace srgbviz {

template<typename T1, typename T2>
SequenceRGBGenerator<T1, T2>::SequenceRGBGenerator(
    const datasource::IDataSource<T1>& src,
    const colormap::IColorMap<T2>& cmap
) : source_(src), colormap_(cmap) {}

template<typename T1, typename T2>
std::vector<types::RGB> SequenceRGBGenerator<T1, T2>::generate() {
    std::vector<types::RGB> pixels(source_.size());

    for (size_t i = 0; i < source_.size(); i++) {
        pixels[i] = colormap_.map(source_.get(i));
    }

    return pixels;
}

}

#endif // SRGBVIZ_GENERATOR_TPP