//
// Created by ultcrt on 24-1-25.
//

#ifndef ULTRENDERER_ALGORITHM_H
#define ULTRENDERER_ALGORITHM_H

#include <vector>
#include <numeric>
#include <algorithm>

namespace UltRenderer {
    namespace Utils {
        /*----------Declaration----------*/
        // Tips: Any type that can match the signature: bool (const T& a, const T& b). Because template is parsed at compile time.
        template <typename T, typename Compare=std::less<T>>
        std::vector<std::size_t> ArgSort(const std::vector<T>& target, Compare comp = {});

        template <typename T, typename Compare=std::less<T>>
        std::size_t ArgMax(const std::vector<T>& target, Compare comp = {});

        template <typename T, typename Compare=std::less<T>>
        std::size_t ArgMin(const std::vector<T>& target, Compare comp = {});

        /*----------Definition----------*/
        template <typename T, typename Compare>
        std::vector<std::size_t> ArgSort(const std::vector<T>& target, Compare comp) {
            std::vector<std::size_t> indices(target.size());
            std::iota(std::begin(indices), std::end(indices), 0);
            std::stable_sort(indices.begin(), indices.end(), [&target, &comp](std::size_t a, std::size_t b){return comp(target[a], target[b]);});
            return indices;
        }

        template <typename T, typename Compare>
        std::size_t ArgMax(const std::vector<T>& target, Compare comp) {
            std::vector<std::size_t> indices(target.size());
            std::iota(std::begin(indices), std::end(indices), 0);
            return *std::max_element(indices.begin(), indices.end(), [&target, &comp](std::size_t a, std::size_t b){return comp(target[a], target[b]);});
        }

        template <typename T, typename Compare>
        std::size_t ArgMin(const std::vector<T>& target, Compare comp) {
            std::vector<std::size_t> indices(target.size());
            std::iota(std::begin(indices), std::end(indices), 0);
            return *std::min_element(indices.begin(), indices.end(), [&target, &comp](std::size_t a, std::size_t b){return comp(target[a], target[b]);});
        }
    } // Utils
} // UltRenderer

#endif //ULTRENDERER_ALGORITHM_H
