//
// Created by ultcrt on 23-12-26.
//

#include "rendering/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        Camera::Camera(double width, double height, double zMin, double zMax, ProjectionType projectionType):
        _width(width), _height(height), _zMin(zMin), _zMax(zMax), _projectionType(projectionType) {
            updateProjectionMatrix();
        }

        double Camera::width() const {
            return _width;
        }

        double Camera::height() const {
            return _height;
        }

        double Camera::zMin() const {
            return _zMin;
        }

        double Camera::zMax() const {
            return _zMax;
        }

        void Camera::updateProjectionMatrix() {
            double w = _width;
            double h = _height;
            // Camera is facing at -z
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

        void Camera::setWidth(double width) {
            _width = width;
            updateProjectionMatrix();
        }

        void Camera::setHeight(double height) {
            _height = height;
            updateProjectionMatrix();
        }

        void Camera::setZMin(double zMin) {
            _zMin = zMin;
            updateProjectionMatrix();
        }

        void Camera::setZMax(double zMax) {
            _zMax = zMax;
            updateProjectionMatrix();
        }

        void Camera::setProjectionType(ProjectionType projectionType) {
            _projectionType = projectionType;
            updateProjectionMatrix();
        }
    } // Rendering
} // UltRenderer