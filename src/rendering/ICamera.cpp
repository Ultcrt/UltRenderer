//
// Created by ultcrt on 24-1-8.
//

#include "rendering/ICamera.h"

namespace UltRenderer {
    namespace Rendering {
        ICamera::ICamera(double width, double height, double zMin, double zMax, ProjectionType projectionType):
                _width(width), _height(height), _zMin(zMin), _zMax(zMax), _projectionType(projectionType) {
            updateProjectionMatrix();
        }

        double ICamera::width() const {
            return _width;
        }

        double ICamera::height() const {
            return _height;
        }

        double ICamera::zMin() const {
            return _zMin;
        }

        double ICamera::zMax() const {
            return _zMax;
        }

        void ICamera::updateProjectionMatrix() {
            double w = _width;
            double h = _height;
            // ICamera is facing at -z
            double n = -_zMin;
            double f = -_zMax;

            // Pressing frustum to cuboid first when using perspective projection
            Math::Transform3D press;
            if (_projectionType == ProjectionType::PERSPECTIVE) {
                press(2, 2) = (n + f) / n;
                press(2, 3) = -f;
                press(3, 2) = 1 / n;
                press(3, 3) = 0;
            }

            // Orthogonal projection can be decomposed into translation and scaling
            Math::Transform3D translation({1, 1, 1}, {0, 0, 0}, {0, 0, -(n + f) / 2});
            Math::Transform3D scaling({2 / w, 2 / h, 2 / (n - f)}, {0, 0, 0}, {0, 0, 0});

            projectionMatrix = scaling * translation * press;
        }

        void ICamera::setWidth(double width) {
            _width = width;
            updateProjectionMatrix();
        }

        void ICamera::setHeight(double height) {
            _height = height;
            updateProjectionMatrix();
        }

        void ICamera::setZMin(double zMin) {
            _zMin = zMin;
            updateProjectionMatrix();
        }

        void ICamera::setZMax(double zMax) {
            _zMax = zMax;
            updateProjectionMatrix();
        }

        void ICamera::setProjectionType(ProjectionType projectionType) {
            _projectionType = projectionType;
            updateProjectionMatrix();
        }
    } // Rendering
} // UltRenderer