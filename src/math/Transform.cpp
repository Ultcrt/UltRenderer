//
// Created by ultcrt on 23-12-26.
//

#include "math/Transform.h"

namespace UltRenderer {
    namespace Math {
        Transform3D::Transform3D(const Vector3D &scaling, const Vector3D &rotation, const Vector3D &position) {
            Matrix3D scalingMat = {
                scaling.x(), 0, 0,
                0, scaling.y(), 0,
                0, 0, scaling.z(),
            };

            Matrix3D roll = {
                1, 0, 0,
                0, std::cos(rotation.z()), -std::sin(rotation.z()),
                0, std::sin(rotation.z()), std::cos(rotation.z()),
            };

            Matrix3D pitch = {
                    std::cos(rotation.y()), 0, std::sin(rotation.y()),
                    0, 1, 0,
                    -std::sin(rotation.y()), 0, std::cos(rotation.y())
            };

            Matrix3D yaw = {
                    std::cos(rotation.x()), -std::sin(rotation.x()), 0,
                    std::sin(rotation.x()), std::cos(rotation.x()), 0,
                    0, 0, 1
            };

            Matrix3D linear = yaw * pitch * roll * scalingMat;

            _data = {
                    linear[0], linear[1], linear[2], position.x(),
                    linear[3], linear[4], linear[5], position.y(),
                    linear[6], linear[7], linear[8], position.z(),
                    0, 0, 0, 1
            };
        }

        Transform3D::Transform3D(): Matrix4D(Matrix4D::Identity()) {}

        Transform3D::Transform3D(const Matrix4D &target) : Matrix(target) {}

        Transform3D Transform3D::FromLookAt(const Math::Vector3D& position, const Math::Vector3D& lookAt, const Math::Vector3D& up) {
            const auto zAxis = (position - lookAt).normalized();
            const auto xAxis = up.cross(zAxis).normalized();
            // Should not use up vector directly as y-axis, to make more flexibility for users
            const auto yAxis = zAxis.cross(xAxis).normalized();

            return {
                    xAxis.x(), yAxis.x(), zAxis.x(), position.x(),
                    xAxis.y(), yAxis.y(), zAxis.y(), position.y(),
                    xAxis.z(), yAxis.z(), zAxis.z(), position.z(),
                    0, 0, 0, 1
            };
        }
    } // Math
} // UltRenderer