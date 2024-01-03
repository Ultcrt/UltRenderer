//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_CAMERA_H
#define ULTRENDERER_CAMERA_H

#include "math/Matrix.h"
#include "math/Transform.h"
#include "data/TriangleMesh.h"
#include "hierarchy/TransformNode.h"
#include "rendering/Scene.h"
#include "rendering/Rasterize.h"

namespace UltRenderer {
    namespace Rendering {
        enum class ProjectionType {
            ORTHOGONAL, PERSPECTIVE
        };

        class Camera: public Hierarchy::TransformNode {
        private:
            double _width;
            double _height;

            double _zMin = 0.1;
            double _zMax = 10;

            ProjectionType _projectionType;
        public:
            Math::Transform3D projectionMatrix;

            Camera(double width, double height, double zMin=0.1, double zMax=10, ProjectionType projectionType=ProjectionType::PERSPECTIVE);

            [[nodiscard]] double width() const;
            [[nodiscard]] double height() const;
            [[nodiscard]] double zMin() const;
            [[nodiscard]] double zMax() const;

            void updateProjectionMatrix();

            void setProjectionType(ProjectionType projectionType);
            void setWidth(double width);
            void setHeight(double height);
            void setZMin(double zMin);
            void setZMax(double zMax);

            template<Data::ImageFormat FORMAT>
            [[nodiscard]] Data::Image render(std::size_t width, std::size_t height) const;
        };

        template<Data::ImageFormat FORMAT>
        Data::Image
        Camera::render(std::size_t width, std::size_t height) const {
            // Origin is always (0, 0) here, depth is scaled into (0, 1)
            Math::Transform3D viewport;
            viewport(0, 0) = static_cast<double>(width) / 2.;
            viewport(1, 1) = static_cast<double>(height) / 2.;
            viewport(2, 2) = 1. / 2.;
            viewport(0, 3) = static_cast<double>(width) / 2.;
            viewport(1, 3) = static_cast<double>(height) / 2.;
            viewport(2, 3) = 1. / 2.;

            UltRenderer::Data::Image result(width, height, FORMAT);
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
                        // TODO: Discard triangle when one vertex is outside image
                        if (transformedVertex0.x() >= 0 && transformedVertex0.x() <= static_cast<double>(width)) {
                            if (transformedVertex1.x() >= 0 && transformedVertex1.x() <= static_cast<double>(width)) {
                                if (transformedVertex2.x() >= 0 && transformedVertex2.x() <= static_cast<double>(width)) {
                                    if (transformedVertex0.y() >= 0 && transformedVertex0.y() <= static_cast<double>(height)) {
                                        if (transformedVertex1.y() >= 0 && transformedVertex1.y() <= static_cast<double>(height)) {
                                            if (transformedVertex2.y() >= 0 && transformedVertex2.y() <= static_cast<double>(height)) {
                                                Math::Vector2S point0 = {static_cast<std::size_t>(std::round(transformedVertex0.x())), static_cast<std::size_t>(std::round(transformedVertex0.y()))};
                                                Math::Vector2S point1 = {static_cast<std::size_t>(std::round(transformedVertex1.x())), static_cast<std::size_t>(std::round(transformedVertex1.y()))};
                                                Math::Vector2S point2 = {static_cast<std::size_t>(std::round(transformedVertex2.x())), static_cast<std::size_t>(std::round(transformedVertex2.y()))};

                                                Rasterize::Triangle<FORMAT>(result, {point0, point1, point2}, {transformedVertex0.z(), transformedVertex1.z(), transformedVertex2.z()}, {uv0, uv1, uv2}, *pTexture, ratio, zBuffer);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        }
                }
            }

            return result;
        }
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
