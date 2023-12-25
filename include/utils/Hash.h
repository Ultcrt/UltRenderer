//
// Created by ultcrt on 23-12-11.
//

#ifndef ULTRENDERER_HASH_H
#define ULTRENDERER_HASH_H

#include <algorithm>
#include "data/Matrix.h"

namespace UltRenderer {
    namespace Utils {
        namespace Hash {
            /*----------Declaration----------*/
            template <typename T>
            struct SpatialHash3D {
            private:
                std::size_t _n;

            public:
                explicit SpatialHash3D(std::size_t n);
                std::size_t operator() (const Data::Vector3<T>& target) const;
            };

            template <typename T>
            struct SpatialHash2D {
            private:
                std::size_t _n;

            public:
                explicit SpatialHash2D(std::size_t n);
                std::size_t operator() (const Data::Vector2<T>& target) const;
            };

            /*----------Definition----------*/
            template<typename T>
            SpatialHash3D<T>::SpatialHash3D(std::size_t n): _n(n) {}

            template<typename T>
            std::size_t SpatialHash3D<T>::operator()(const Data::Vector3<T> &target) const {
                std::size_t h = (target.x() * 92837111) ^ (target.y() * 689287499) ^ (target.z() * 283923481);
                return h % _n;
            }

            template<typename T>
            SpatialHash2D<T>::SpatialHash2D(std::size_t n): _n(n) {}

            template<typename T>
            std::size_t SpatialHash2D<T>::operator()(const Data::Vector2<T> &target) const {
                std::size_t h = (target.x() * 92837111) ^ (target.y() * 689287499);
                return h % _n;
            }
        }
    } // Utils
} // UltRenderer

#endif //ULTRENDERER_HASH_H
