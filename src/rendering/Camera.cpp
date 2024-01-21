//
// Created by ultcrt on 23-12-26.
//

#include "rendering/Camera.h"
#include "shaders/BlinnPhongReflectionMeshShader.h"

namespace UltRenderer {
    namespace Rendering {
        Data::Image Camera::render(std::size_t width, std::size_t height, Data::Pixel<Data::ImageFormat::RGBA> backgroundColor, const Postprocessors::IPostprocessor& postprocessor) const {
            Shaders::BlinnPhongReflectionMeshVertexShader vertexShader;
            Shaders::BlinnPhongReflectionMeshFragmentShader fragmentShader;
            Shaders::BlinnPhongReflectionMeshInterpolator interpolator;
            return Camera::render<Shaders::BlinnPhongReflectionMeshVarying>(width, height, vertexShader, fragmentShader, interpolator, backgroundColor, postprocessor);
        }

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
            projectionMatrix = ComputeProjectionMatrix(_width, _height, _zMin, _zMax, _projectionType);
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

        Math::Transform3D Camera::ComputeProjectionMatrix(double width, double height, double zMin, double zMax,
                                                          ProjectionType projectionType) {
            double w = width;
            double h = height;
            // Camera is facing at -z
            double n = -zMin;
            double f = -zMax;

            // Pressing frustum to cuboid first when using perspective projection
            Math::Transform3D press;
            if (projectionType == ProjectionType::PERSPECTIVE) {
                press(2, 2) = (n + f) / n;
                press(2, 3) = -f;
                press(3, 2) = 1 / n;
                press(3, 3) = 0;
            }

            // Orthogonal projection can be decomposed into translation and scaling
            Math::Transform3D translation({1, 1, 1}, {0, 0, 0}, {0, 0, -(n + f) / 2});
            Math::Transform3D scaling({2 / w, 2 / h, 2 / (n - f)}, {0, 0, 0}, {0, 0, 0});

            return scaling * translation * press;
        }

        Math::Transform3D Camera::ComputeViewportMatrix(std::size_t width, std::size_t height) {
            Math::Transform3D viewport;
            viewport(0, 0) = static_cast<double>(width) / 2.;
            viewport(1, 1) = static_cast<double>(height) / 2.;
            // z scaling factor is negative, to make 0~1 represent near~far
            viewport(2, 2) = -1. / 2.;
            viewport(0, 3) = static_cast<double>(width) / 2.;
            viewport(1, 3) = static_cast<double>(height) / 2.;
            viewport(2, 3) = 1. / 2.;

            return viewport;
        }
    } // Rendering
} // UltRenderer