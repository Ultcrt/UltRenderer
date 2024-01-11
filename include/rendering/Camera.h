//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_CAMERA_H
#define ULTRENDERER_CAMERA_H

#include "math/Matrix.h"
#include "math/Transform.h"
#include "data/TriangleMesh.h"
#include "rendering/Rasterize.h"
#include "rendering/Pipeline.h"
#include "shaders/IMeshShader.h"
#include "hierarchy/TransformNode.h"
#include "rendering/Scene.h"

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
            Camera(double width, double height,
                   double zMin = 0.1, double zMax = 10, ProjectionType projectionType = ProjectionType::PERSPECTIVE);

            Math::Transform3D projectionMatrix;

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

            [[nodiscard]] Data::Image render(std::size_t width, std::size_t height) const;

            template<std::derived_from<Shaders::IVarying> V>
            [[nodiscard]] Data::Image render(std::size_t width, std::size_t height, Shaders::IMeshVertexShader<V> &vertexShader, Shaders::IMeshFragmentShader<V> &fragmentShader,
                                             const Shaders::IInterpolator<V> &interpolator) const;

        };

        template<std::derived_from<Shaders::IVarying> V>
        Data::Image Camera::render(std::size_t width, std::size_t height, Shaders::IMeshVertexShader<V> &vertexShader, Shaders::IMeshFragmentShader<V> &fragmentShader,
                                      const Shaders::IInterpolator<V> &interpolator) const {
            // Origin is always (0, 0) here, depth is scaled into (0, 1)
            Math::Transform3D viewport;
            viewport(0, 0) = static_cast<double>(width) / 2.;
            viewport(1, 1) = static_cast<double>(height) / 2.;
            viewport(2, 2) = 1. / 2.;
            viewport(0, 3) = static_cast<double>(width) / 2.;
            viewport(1, 3) = static_cast<double>(height) / 2.;
            viewport(2, 3) = 1. / 2.;

            UltRenderer::Data::Image fBuffer(width, height, Data::ImageFormat::RGBA);
            UltRenderer::Data::Image zBuffer(width, height, Data::ImageFormat::GRAY);

            // viewport * projection * view
            for (const auto &pMesh: _pScene->meshes()) {
                const Math::Transform3D view = transformMatrix.inverse();

                // TODO: Compute only the first light here for simplicity, which is wrong
                Math::Vector3D light = _pScene->lights()[0]->position;

                // TODO: nullptr is never checked
                // Set IMeshVertexShader general uniforms
                vertexShader.pModel = &pMesh->transformMatrix;
                vertexShader.pView = &view;
                vertexShader.pProjection = &projectionMatrix;
                vertexShader.pLight = &light;

                // Set IMeshVertexShader general attributes
                vertexShader.pVertices = &pMesh->vertices;
                vertexShader.pNormals = &pMesh->vertexNormals;
                vertexShader.pUvs = &pMesh->vertexTextures;

                // Set IMeshFragmentShader general uniforms
                fragmentShader.pModel = &pMesh->transformMatrix;
                fragmentShader.pView = &view;
                fragmentShader.pProjection = &projectionMatrix;
                fragmentShader.pTexture = pMesh->pTexture.get();
                fragmentShader.pNormalMap = pMesh->pNormalMap.get();
                fragmentShader.pSpecular = pMesh->pSpecular.get();
                fragmentShader.pNormalMapType = &pMesh->normalMapType;
                fragmentShader.pLight = &light;

                Pipeline::Execute<V>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {},
                                     vertexShader, fragmentShader, interpolator);

            }

            return fBuffer;
        }


    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
