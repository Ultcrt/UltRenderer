//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_PROXY_H
#define ULTRENDERER_PROXY_H

#include "math/Matrix.h"

namespace UltRenderer {
    namespace Utils {
        /*----------Declaration----------*/
        // Proxy class of Matrix to make returning Matrix can be assignable
        template<typename T, std::size_t M, std::size_t N>
        class MatrixProxy {
        private:
            std::array<T*, M * N> _componentPtrs;

        public:
            explicit MatrixProxy(const std::array<T*, M * N>& componentPtrs);

            // TODO: Should override more operator like +=
            MatrixProxy<T, M, N>& operator=(const Math::Matrix<T, M, N>& target);

            operator Math::Matrix<T, M, N>() const;

            T& operator()(std::size_t rowIdx, std::size_t colIdx);

            T& operator[](std::size_t idx);

            T& x();

            T& y();

            T& z();

            T& w();
        };

        /*----------Definition----------*/
        template<typename T, std::size_t M, std::size_t N>
        T &MatrixProxy<T, M, N>::operator()(std::size_t rowIdx, std::size_t colIdx) {
            assert(rowIdx < M);
            assert(colIdx < N);
            return *_componentPtrs[N * rowIdx + colIdx];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &MatrixProxy<T, M, N>::operator[](std::size_t idx) {
            assert(idx < M * N);
            return *_componentPtrs[idx];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &MatrixProxy<T, M, N>::x() {
            static_assert(M * N > 0);

            return *_componentPtrs[0];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &MatrixProxy<T, M, N>::y() {
            static_assert(M * N > 1);

            return *_componentPtrs[1];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &MatrixProxy<T, M, N>::z() {
            static_assert(M * N > 2);

            return *_componentPtrs[2];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &MatrixProxy<T, M, N>::w() {
            static_assert(M * N > 3);

            return *_componentPtrs[3];
        }

        template<typename T, std::size_t M, std::size_t N>
        MatrixProxy<T, M, N>::operator Math::Matrix<T, M, N>() const {
            Math::Matrix<T, M, N> res;

            for (std::size_t idx = 0; idx < M * N; idx++) {
                res[idx] = *_componentPtrs[idx];
            }

            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        MatrixProxy<T, M, N> &MatrixProxy<T, M, N>::operator=(const Math::Matrix<T, M, N> &target) {
            for (std::size_t idx = 0; idx < M * N; idx++) {
                *_componentPtrs[idx] = target[idx];
            }

            return *this;
        }

        template<typename T, std::size_t M, std::size_t N>
        MatrixProxy<T, M, N>::MatrixProxy(const std::array<T*, M * N> &componentPtrs): _componentPtrs(componentPtrs) {}
    } // Utils
} // UltRenderer

#endif //ULTRENDERER_PROXY_H
