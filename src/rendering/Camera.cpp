//
// Created by ultcrt on 23-12-26.
//

#include "rendering/Camera.h"
#include "rendering/Rasterize.h"
#include "rendering/Scene.h"

namespace UltRenderer {
    namespace Rendering {
        Camera::Camera(double width, double height, double zMin, double zMax, ProjectionType projectionType):
        _width(width), _height(height), _zMin(zMin), _zMax(zMax), _projectionType(projectionType) {}

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
                press(0, 0) = 0;
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

        Data::Image
        Camera::render(std::size_t width, std::size_t height, Data::ImageFormat format) const {
            // Origin is always (0, 0) here, depth is scaled into (0, 1)
            Math::Transform3D viewport;
            viewport(0, 0) = static_cast<double>(width) / 2.;
            viewport(1, 1) = static_cast<double>(height) / 2.;
            viewport(2, 2) = 1. / 2.;
            viewport(0, 3) = static_cast<double>(width) / 2.;
            viewport(1, 3) = static_cast<double>(height) / 2.;
            viewport(2, 3) = 1. / 2.;

            UltRenderer::Data::Image result(width, height, format);
            UltRenderer::Data::Image zBuffer(width, height, Data::ImageFormat::GRAY);
            zBuffer.fill(1);

            // viewport * projection * view
            const Math::Transform3D constTransform = viewport * projectionMatrix * transformMatrix.inverse();
            for (const auto& pMesh: _pScene->meshes()) {
                const auto& pTexture = pMesh->pTexture;
                const Math::Transform3D overallTransform = constTransform * pMesh->transformMatrix;
                for (const auto& tri: pMesh->triangles) {
                    UltRenderer::Math::Vector3D vertex0 = pMesh->vertices[tri[0]];
                    UltRenderer::Math::Vector3D vertex1 = pMesh->vertices[tri[1]];
                    UltRenderer::Math::Vector3D vertex2 = pMesh->vertices[tri[2]];

                    UltRenderer::Math::Vector3D transformedVertex0 = (overallTransform * vertex0.toHomogeneousCoordinates(1)).toCartesianCoordinates();
                    UltRenderer::Math::Vector3D transformedVertex1 = (overallTransform * vertex1.toHomogeneousCoordinates(1)).toCartesianCoordinates();
                    UltRenderer::Math::Vector3D transformedVertex2 = (overallTransform * vertex2.toHomogeneousCoordinates(1)).toCartesianCoordinates();

                    // TODO: Need to support texture-less mesh
                    UltRenderer::Math::Vector3D uv0 = pMesh->vertexTextures[tri[0]];
                    UltRenderer::Math::Vector3D uv1 = pMesh->vertexTextures[tri[1]];
                    UltRenderer::Math::Vector3D uv2 = pMesh->vertexTextures[tri[2]];

                    Math::Vector3D avgLight;
                    // TODO: Compute Average light here for simplicity, which is wrong
                    for (const auto& light: _pScene->lights()) {
                        avgLight = avgLight + light->position;
                    }
                    UltRenderer::Math::Vector3D triangleNormal = ((vertex0 - vertex2).cross(vertex0 - vertex1)).normalized();
                    double ratio = triangleNormal.dot(avgLight.normalized());

                    if (ratio > 0) {
                        Math::Vector2S point0 = {static_cast<std::size_t>(std::round(transformedVertex0.x())), static_cast<std::size_t>(std::round(transformedVertex0.y()))};
                        Math::Vector2S point1 = {static_cast<std::size_t>(std::round(transformedVertex1.x())), static_cast<std::size_t>(std::round(transformedVertex1.y()))};
                        Math::Vector2S point2 = {static_cast<std::size_t>(std::round(transformedVertex2.x())), static_cast<std::size_t>(std::round(transformedVertex2.y()))};

                        // TODO: Use fixed Image format here
                        Rasterize::Triangle<Data::ImageFormat::RGB>(result, {point0, point1, point2}, {transformedVertex0.z(), transformedVertex1.z(), transformedVertex2.z()}, {uv0, uv1, uv2}, *pTexture, ratio, zBuffer);
                    }
                }
            }
        }
    } // Rendering
} // UltRenderer