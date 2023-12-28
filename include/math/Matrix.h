//
// Created by ultcrt on 23-11-23.
//

#ifndef ULTRENDERER_MATRIX_H
#define ULTRENDERER_MATRIX_H

#include <string>
#include <array>
#include <cassert>
#include <cmath>
#include <format>

namespace UltRenderer {
    namespace Math {
        /*----------Declaration----------*/
        template<typename T, std::size_t M, std::size_t N>
        class Matrix {
            // Tips: Foo<int> and Foo<float> is not the same class
            template<typename, std::size_t, std::size_t>
            friend class Matrix;

        protected:
            std::array<T, M * N> _data;

        public:
            explicit Matrix(const T& fill=T());

            template<typename CAST>
            explicit Matrix(const Matrix<CAST, M, N>& target);

            Matrix(const std::array<T, M * N>& data);

            Matrix(const std::array<std::array<T, N>, M>& target);

            Matrix(T x, T y);

            Matrix(T x, T y, T z);

            Matrix(T x, T y, T z, T w);

            // Tips: Use parameter pack to make initialization easier, xyzw is used to make sure this constructor will not hide the default constructors
            template<typename ...Ts>
            Matrix(T x, T y, T z, T w, Ts... components);

            static Matrix<T, M, N> Identity();

            T& operator()(std::size_t rowIdx, std::size_t colIdx);

            T operator()(std::size_t rowIdx, std::size_t colIdx) const;

            T& operator[](std::size_t idx);

            T operator[](std::size_t idx) const;

            explicit operator std::string() const;

            [[nodiscard]] Matrix<std::size_t, 2, 1> shape() const;

            bool operator==(const Matrix<T, M, N>& target) const;

            bool operator!=(const Matrix<T, M, N>& target) const;

            Matrix<T, M, N> operator+(const Matrix<T, M, N>& target) const;

            Matrix<T, M, N> operator+() const;

            template<std::size_t K>
            Matrix<T, M, K> operator*(const Matrix<T, N, K>& target) const;

            Matrix<T, M, N> operator*(T target) const;

            Matrix<T, M, N> operator/(T target) const;

            Matrix<T, M, N> operator-(const Matrix<T, M, N>& target) const;

            Matrix<T, M, N> operator-() const;

            [[nodiscard]] Matrix<T, N, M> transpose() const;

            [[nodiscard]] T determinant() const;

            [[nodiscard]] Matrix<T, M, N> inverse() const;

            [[nodiscard]] Matrix<T, M+1, N> toHomogeneousCoordinates(T lastDim) const;

            [[nodiscard]] Matrix<T, M-1, N> toCartesianCoordinates() const;

            [[nodiscard]] Matrix<T, 3, 1> cross(const Matrix<T, 3, 1>& target) const;

            [[nodiscard]] T dot(const Matrix<T, M, N>& target) const;

            [[nodiscard]] T norm() const;

            [[nodiscard]] T norm2() const;

            void normalize();

            [[nodiscard]] Matrix<T, M, N> normalized() const;

            [[nodiscard]] std::array<T, M * N>::const_iterator begin() const;

            [[nodiscard]] std::array<T, M * N>::const_iterator end() const;

            [[nodiscard]] std::array<T, M * N>::iterator begin();

            [[nodiscard]] std::array<T, M * N>::iterator end();

            [[nodiscard]] Matrix<T, M, N> componentWiseProduct(const Matrix<T, M, N>& target) const;

            T& x();

            T& y();

            T& z();

            T& w();

            [[nodiscard]] T x() const;

            [[nodiscard]] T y() const;

            [[nodiscard]] T z() const;

            [[nodiscard]] T w() const;
        };

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> operator*(T target, const Matrix<T, M, N>& source);

        template<typename T, std::size_t M, std::size_t N>
        std::ostream& operator<< (std::ostream& stream, const Matrix<T, M, N>& target);

        // Tips: Use using instead of typedef when creating alias of a template
        template<typename T>
        using Vector4 = Matrix<T, 4, 1>;

        template<typename T>
        using Vector3 = Matrix<T, 3, 1>;

        template<typename T>
        using Vector2 = Matrix<T, 2, 1>;

        typedef Vector4<int> Vector4I;
        typedef Vector4<double> Vector4D;
        typedef Vector4<float> Vector4F;
        typedef Vector4<std::size_t> Vector4S;

        typedef Vector3<int> Vector3I;
        typedef Vector3<double> Vector3D;
        typedef Vector3<float> Vector3F;
        typedef Vector3<std::size_t> Vector3S;

        typedef Vector2<int> Vector2I;
        typedef Vector2<double> Vector2D;
        typedef Vector2<float> Vector2F;
        typedef Vector2<std::size_t> Vector2S;

        template<typename T>
        using Matrix4 = Matrix<T, 4, 4>;

        template<typename T>
        using Matrix3 = Matrix<T, 3, 3>;

        typedef Matrix4<double> Matrix4D;
        typedef Matrix4<float> Matrix4F;

        typedef Matrix3<double> Matrix3D;
        typedef Matrix3<float> Matrix3F;

        /*----------Definition----------*/
        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M + 1, N> Matrix<T, M, N>::toHomogeneousCoordinates(T lastDim) const {
            static_assert(N == 1);

            Matrix<T, M + 1, N> res;

            for (std::size_t idx = 0; idx < M; idx++) {
                res._data[idx] = _data[idx];
            }

            res._data[M + 1] = lastDim;

            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M - 1, N> Matrix<T, M, N>::toCartesianCoordinates() const {
            static_assert(N == 1);
            static_assert(M > 1);

            Matrix<T, M - 1, N> res;

            for (std::size_t idx = 0; idx < M - 1; idx++) {
                res._data[idx] = _data[idx];
            }

            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::Identity() {
            Matrix<T, M, N> res;

            std::size_t maxSize = std::max(M, N);

            for (std::size_t idx = 0; idx < maxSize; idx++) {
                res._data[idx * N + idx] = 1;
            }

            return Matrix<T, M, N>();
        }

        template<typename T, std::size_t M, std::size_t N>
        std::ostream& operator<< (std::ostream& stream, const Matrix<T, M, N>& target) {
            return stream << static_cast<std::string>(target);
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::inverse() const {
            static_assert(M == N);

            T det = determinant();

            assert(det != 0);

            // TODO: Current cofactor matrix method may be very slow
            Matrix<T, M, N> cofMat;
            for (std::size_t i = 0; i < M; i++) {
                for (std::size_t j = 0; j < N; j++){
                    Matrix<T, M - 1, N - 1> subMat;
                    for (std::size_t subRowIdx = 0; subRowIdx < M - 1; subRowIdx++) {
                        for (std::size_t subColIdx = 0; subColIdx < N - 1; subColIdx++) {
                            std::size_t rowIdx = subRowIdx < i ? subRowIdx : subRowIdx + 1;
                            std::size_t colIdx = subColIdx < j ? subColIdx : subColIdx + 1;

                            subMat[subRowIdx * (N - 1) + subColIdx] = _data[rowIdx * N + colIdx];
                        }
                    }
                    // Transposed here
                    cofMat[j * N + i] = std::pow(-1, (i + j) % 2) * subMat.determinant();
                }
            }

            return cofMat / determinant();
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::determinant() const {
            static_assert(M == N);

            // TODO: Current recursive resolution may be very slow

            // The stop condition of recursion
            // Tips: if-else must be used here to make compiler stop recursive template instantiation correctly
            if constexpr(M == 1)      return + _data[0];
            else if constexpr(M == 2) return + _data[0] * _data[3]
                                             - _data[1] * _data[2];
            else if constexpr(M == 3) return + _data[0] * _data[4] * _data[8]
                                             + _data[1] * _data[5] * _data[6]
                                             + _data[2] * _data[3] * _data[7]
                                             - _data[0] * _data[5] * _data[7]
                                             - _data[1] * _data[3] * _data[8]
                                             - _data[2] * _data[4] * _data[6];
            else {
                T det = T();
                for (std::size_t subIdx = 0; subIdx < M; subIdx++) {
                    Matrix<T, M - 1, N - 1> subMat;
                    for (std::size_t subRowIdx = 0; subRowIdx < M - 1; subRowIdx++) {
                        for (std::size_t subColIdx = 0; subColIdx < N - 1; subColIdx++) {
                            std::size_t rowIdx = subRowIdx + 1;
                            std::size_t colIdx = subColIdx < subIdx ? subColIdx : subColIdx + 1;

                            subMat[subRowIdx * (N - 1) + subColIdx] = _data[rowIdx * N + colIdx];
                        }
                    }
                    det += _data[subIdx] * std::pow(-1, subIdx % 2) * subMat.determinant();
                }

                return det;
            }
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, N, M> Matrix<T, M, N>::transpose() const {
            Matrix<T, N, M> res(_data);

            for (std::size_t rowIdx = 0; rowIdx < M; rowIdx++) {
                for (std::size_t colIdx = 0; colIdx < N; colIdx++) {
                    res._data[colIdx * M + rowIdx] = _data[rowIdx * N + colIdx];
                }
            }

            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::componentWiseProduct(const Matrix<T, M, N> &target) const {
            Matrix<T, M, N> res;
            for (std::size_t idx = 0; idx < M * N; idx++) {
                res._data[idx] = _data[idx] * target._data[idx];
            }
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> operator*(T target, const Matrix<T, M, N>& source) {
            return source * target;
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::norm2() const {
            T sum = 0;
            for (const T& component: _data) {
                sum += component * component;
            }
            return sum;
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::norm() const {
            return std::sqrt(norm2());
        }

        template<typename T, std::size_t M, std::size_t N>
        void Matrix<T, M, N>::normalize() {
            T len = norm();
            for (std::size_t idx = 0; idx < M * N; idx++) {
                _data[idx] = _data[idx] / len;
            }
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::normalized() const {
            Matrix<T, M, N> res = *this;
            res.normalize();
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        T Matrix<T, M, N>::dot(const Matrix<T, M, N> &target) const {
            // TODO: Need to add assert prompt
            static_assert(M == 1 || N == 1);

            T res = 0;
            for (std::size_t rowIdx = 0; rowIdx < M; rowIdx++) {
                for (std::size_t colIdx = 0; colIdx < N; colIdx++) {
                    res += _data[N * rowIdx + colIdx] * target._data[N * rowIdx + colIdx];
                }
            }
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, 3, 1> Matrix<T, M, N>::cross(const Matrix<T, 3, 1> &target) const {
            static_assert(M * N == 3);

            return {
                y() * target.z() - z() * target.y(),
                z() * target.x() - x() * target.z(),
                x() * target.y() - y() * target.x()
            };
        }

        template<typename T, std::size_t M, std::size_t N>
        std::array<T, M * N>::iterator Matrix<T, M, N>::end() {
            return _data.end();
        }

        template<typename T, std::size_t M, std::size_t N>
        std::array<T, M * N>::iterator Matrix<T, M, N>::begin() {
            return _data.begin();
        }

        template<typename T, std::size_t M, std::size_t N>
        std::array<T, M * N>::const_iterator Matrix<T, M, N>::end() const {
            return _data.end();
        }

        template<typename T, std::size_t M, std::size_t N>
        std::array<T, M * N>::const_iterator Matrix<T, M, N>::begin() const {
            return _data.begin();
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::operator std::string() const {
            std::string res;

            for (std::size_t rowIdx = 0; rowIdx < M; rowIdx++) {
                for (std::size_t colIdx = 0; colIdx < N; colIdx++) {
                    T val = _data[N * rowIdx + colIdx];

                    res += (val >= 0 ? " " : "-") + std::to_string(std::abs(val)) + "\t";
                }
                res += "\n";
            }
            res += std::format("[{} rows x {} columns]\n", M, N);

            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        bool Matrix<T, M, N>::operator==(const Matrix<T, M, N> &target) const {
            for (std::size_t idx = 0; idx < M * N; idx++) {
                if (_data[idx] != target._data[idx]) {
                    return false;
                }
            }
            return true;
        }

        template<typename T, std::size_t M, std::size_t N>
        bool Matrix<T, M, N>::operator!=(const Matrix<T, M, N> &target) const {
            return !operator==(target);
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator/(T target) const {
            Matrix<T, M, N> res;
            for (std::size_t idx = 0; idx < M * N; idx++) {
                res._data[idx] = _data[idx] / target;
            }
            return res;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix(T x, T y, T z, T w): Matrix() {
            static_assert(M * N > 3);
            _data[0] = x;
            _data[1] = y;
            _data[2] = z;
            _data[3] = w;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix(T x, T y, T z): Matrix() {
            static_assert(M * N > 2);
            _data[0] = x;
            _data[1] = y;
            _data[2] = z;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix(T x, T y): Matrix() {
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
        Matrix<std::size_t, 2, 1> Matrix<T, M, N>::shape() const {
            return {M, N};
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator+() const {
            return *this;
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N> Matrix<T, M, N>::operator*(T target) const {
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
        Matrix<T, M, N>::Matrix(const T& fill) {
            _data.fill(fill);
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

        template<typename T, std::size_t M, std::size_t N>
        template<typename CAST>
        Matrix<T, M, N>::Matrix(const Matrix<CAST, M, N> &target) {
            for (std::size_t idx = 0; idx < _data.size(); idx++) {
                _data[idx] = static_cast<T>(target._data[idx]);
            }
        }

        template<typename T, std::size_t M, std::size_t N>
        Matrix<T, M, N>::Matrix(const std::array<std::array<T, N>, M> &target) {
            for (std::size_t idx = 0; idx < M; idx++) {
                std::copy(target[idx].cbegin(), target[idx].cend(), _data.begin() + idx * N);
            }
        }

        template<typename T, std::size_t M, std::size_t N>
        template<typename... Ts>
        // Tips: Cast can be used on parameter packs
        Matrix<T, M, N>::Matrix(T x, T y, T z, T w, Ts... components):_data{x, y, z, w, static_cast<T>(components)...} {}
    } // UltRenderer
} // Data

#endif //ULTRENDERER_MATRIX_H
