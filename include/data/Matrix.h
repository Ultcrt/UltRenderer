//
// Created by ultcrt on 23-11-23.
//

#ifndef ULTRENDERER_MATRIX_H
#define ULTRENDERER_MATRIX_H

#include <array>
#include <cassert>

namespace UltRenderer {
    namespace Data {
        template<typename T, std::size_t M, std::size_t N>
        class Matrix {
            // Tips: Foo<int> and Foo<float> is not the same class
            template<typename, std::size_t, std::size_t>
            friend class Matrix;

        private:
            std::array<T, M * N> _data;

        public:
            Matrix();
            explicit Matrix(const std::array<T, M * N>& data);

            Matrix(T x, T y);

            Matrix(T x, T y, T z);

            Matrix(T x, T y, T z, T w);

            T& operator()(std::size_t rowIdx, std::size_t colIdx);

            T operator()(std::size_t rowIdx, std::size_t colIdx) const;

            T& operator[](std::size_t idx);

            T operator[](std::size_t idx) const;

            [[nodiscard]] std::pair<std::size_t, std::size_t> shape() const;

            Matrix<T, M, N> operator+(const Matrix<T, M, N>& target) const;

            Matrix<T, M, N> operator+() const;

            template<std::size_t K>
            Matrix<T, M, K> operator*(const Matrix<T, N, K>& target) const;

            Matrix<T, M, N> operator*(int target) const;

            Matrix<T, M, N> operator/(int target) const;

            Matrix<T, M, N> operator-(const Matrix<T, M, N>& target) const;

            Matrix<T, M, N> operator-() const;

            T& x();

            T& y();

            T& z();

            T& w();

            T x() const;

            T y() const;

            T z() const;

            T w() const;
        };

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator/(int target) const {
            Matrix<T, M, N> res;
            for (std::size_t idx = 0; idx < M * N; idx++) {
                res._data[idx] = _data[idx] / target;
            }
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix(T x, T y, T z, T w) {
            static_assert(M * N > 3);
            _data[0] = x;
            _data[1] = y;
            _data[2] = z;
            _data[3] = w;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix(T x, T y, T z) {
            static_assert(M * N > 2);
            _data[0] = x;
            _data[1] = y;
            _data[2] = z;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix(T x, T y) {
            static_assert(M * N > 1);
            _data[0] = x;
            _data[1] = y;
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::x() const {
            static_assert(M * N > 0);
            return _data[0];
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::y() const {
            static_assert(M * N > 1);
            return _data[1];
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::z() const {
            static_assert(M * N > 2);
            return _data[2];
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::w() const {
            static_assert(M * N > 3);
            return _data[3];
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::operator()(std::size_t rowIdx, std::size_t colIdx) const {
            assert(rowIdx < M);
            assert(colIdx < N);
            return _data[N * rowIdx + colIdx];
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::operator[](std::size_t idx) const {
            assert(idx < _data.size());
            return _data[idx];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &Matrix<T, M, N>::x() {
            // Tips: Use static_assert to check errors at compile time
            static_assert(M * N > 0);
            return _data[0];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &Matrix<T, M, N>::y() {
            static_assert(M * N > 1);
            return _data[1];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &Matrix<T, M, N>::z() {
            static_assert(M * N > 2);
            return _data[2];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &Matrix<T, M, N>::w() {
            static_assert(M * N > 3);
            return _data[3];
        }

        template<typename T, std::size_t M, std::size_t N>
        T &Matrix<T, M, N>::operator[](std::size_t idx) {
            assert(idx < _data.size());
            return _data[idx];
        }

        template<typename T, std::size_t M, std::size_t N>
        std::pair<std::size_t, std::size_t> Matrix<T, M, N>::shape() const {
            return {M, N};
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator+() const {
            return *this;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator*(int target) const {
            Matrix<T, M, N> res;
            for (std::size_t idx = 0; idx < M * N; idx++) {
                res._data[idx] = target * _data[idx];
            }
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator-() const {
            return this->operator*(-1);
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator-(const Matrix<T, M, N> &target) const {
            Matrix<T, M, N> res;
            for (std::size_t idx = 0; idx < M * N; idx++) {
                res._data[idx] = _data[idx] - target._data[idx];
            }
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        template<std::size_t K>
        Matrix<T, M, K> Matrix<T, M, N>::operator*(const Matrix<T, N, K> &target) const {
            Matrix<T, M, K> res;
            for (std::size_t rowIdx = 0; rowIdx < M; rowIdx++) {
                for (std::size_t colIdx = 0; colIdx < K; colIdx++) {
                    for (std::size_t i = 0; i < N; i++) {
                        res._data[K * rowIdx + colIdx] += _data[N * rowIdx + i] * target._data[K * i + colIdx];
                    }
                }
            }
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix() {
            _data.fill(0);
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator+(const Matrix<T, M, N> &target) const {
            Matrix<T, M, N> res;
            for (std::size_t idx = 0; idx < M * N; idx++) {
                res._data[idx] = _data[idx] + target._data[idx];
            }
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        T &Matrix<T, M, N>::operator()(std::size_t rowIdx, std::size_t colIdx) {
            assert(rowIdx < M);
            assert(colIdx < N);
            return _data[N * rowIdx + colIdx];
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix(const std::array<T, M * N> &data): _data(data) {}

        // Tips: Use using instead of typedef when creating alias of a template
        template<typename T>
        using Vector3 = Matrix<T, 3, 1>;

        template<typename T>
        using Vector2 = Matrix<T, 2, 1>;

        typedef Vector3<int> Vector3I;
        typedef Vector3<double> Vector3D;
        typedef Vector3<float> Vector3F;
        typedef Vector3<std::size_t> Vector3S;

        typedef Vector2<int> Vector2I;
        typedef Vector2<double> Vector2D;
        typedef Vector2<float> Vector2F;
        typedef Vector2<std::size_t> Vector2S;
    } // UltRenderer
} // Data

#endif //ULTRENDERER_MATRIX_H
