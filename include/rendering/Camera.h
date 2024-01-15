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
#include "shaders/DepthMeshShader.h"
#include "postprocessors/EmptyPostprocessor.h"

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

            [[nodiscard]] Data::Image render(std::size_t width, std::size_t height, const Postprocessors::IPostprocessor& postprocessor = Postprocessors::EmptyPostprocessor()) const;

            template<std::derived_from<Shaders::IVarying> V>
            [[nodiscard]] Data::Image render(std::size_t width, std::size_t height, Shaders::IMeshVertexShader<V> &vertexShader, Shaders::IMeshFragmentShader<V> &fragmentShader,
                                             const Shaders::IInterpolator<V> &interpolator, const Postprocessors::IPostprocessor& postprocessor = Postprocessors::EmptyPostprocessor()) const;

        };

        template<std::derived_from<Shaders::IVarying> V>
        Data::Image Camera::render(std::size_t width, std::size_t height, Shaders::IMeshVertexShader<V> &vertexShader, Shaders::IMeshFragmentShader<V> &fragmentShader,
                                   const Shaders::IInterpolator<V> &interpolator, const Postprocessors::IPostprocessor& postprocessor) const {
            // Origin is always (0, 0) here, depth is scaled into (0, 1)
            Math::Transform3D viewport;
            viewport(0, 0) = static_cast<double>(width) / 2.;
            viewport(1, 1) = static_cast<double>(height) / 2.;
            // z scaling factor is negative, to make 0~1 represent near~far
            viewport(2, 2) = -1. / 2.;
            viewport(0, 3) = static_cast<double>(width) / 2.;
            viewport(1, 3) = static_cast<double>(height) / 2.;
            viewport(2, 3) = 1. / 2.;

            Shaders::DepthMeshVertexShader depthVS;
            Shaders::DepthMeshFragmentShader depthFS;
            Shaders::DepthMeshInterpolator depthIT;

            Data::Image fBuffer(width, height, Data::ImageFormat::RGBA);
            Data::Image zBuffer(width, height, Data::Pixel<Data::ImageFormat::GRAY>(1));

            Data::Image fBufferShadow(width, height, Data::ImageFormat::RGBA);
            Data::Image zBufferShadow(width, height, Data::Pixel<Data::ImageFormat::GRAY>(1));

            // viewport * projection * view
            for (const auto &pMesh: _pScene->meshes()) {
                const Math::Transform3D view = transformMatrix.inverse();

                // TODO: Compute only the first light here for simplicity, which is wrong
                const Rendering::Light& light = *_pScene->lights()[0];

                const Math::Transform3D lightView = Math::Transform3D::FromLookAt({0, 0, 5}, {0, 0, 0}, {0, 1, 0}).inverse();

                // Shadow mapping
                depthVS.pModel = &pMesh->transformMatrix;
                depthVS.pView = &lightView;
                // TODO: Shadow mapping's projection matrix may not be identical to camera's.
                depthVS.pProjection = &projectionMatrix;
                depthVS.modelViewMatrix = lightView * pMesh->transformMatrix;
                depthVS.modelViewProjectionMatrix = projectionMatrix * depthVS.modelViewMatrix;
                depthVS.pVertices = &pMesh->vertices;

                Pipeline::Execute<Shaders::IMeshVarying>(fBufferShadow, zBufferShadow, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {},
                                                         depthVS, depthFS, depthIT);

                // TODO: nullptr is never checked
                // Set IMeshVertexShader general uniforms
                vertexShader.pModel = &pMesh->transformMatrix;
                vertexShader.pView = &view;
                vertexShader.pProjection = &projectionMatrix;
                vertexShader.pLight = &light.direction;
                vertexShader.intensity = light.intensity;
                vertexShader.modelViewMatrix = view * pMesh->transformMatrix;
                vertexShader.modelViewProjectionMatrix = projectionMatrix * vertexShader.modelViewMatrix;

                // Set IMeshVertexShader general attributes
                vertexShader.pVertices = &pMesh->vertices;
                vertexShader.pNormals = &pMesh->vertexNormals;
                vertexShader.pTangents = &pMesh->vertexTangents;
                vertexShader.pUvs = &pMesh->vertexTextures;

                // Set IMeshFragmentShader general uniforms
                fragmentShader.pTexture = pMesh->pTexture.get();
                fragmentShader.pNormalMap = pMesh->pNormalMap.get();
                fragmentShader.pSpecular = pMesh->pSpecular.get();
                fragmentShader.normalMapType = pMesh->normalMapType;
                fragmentShader.pModel = vertexShader.pModel;
                fragmentShader.pView = vertexShader.pView;
                fragmentShader.pProjection = vertexShader.pProjection;
                fragmentShader.pShadowMap = &zBufferShadow;
                fragmentShader.lightMatrix = (viewport * depthVS.modelViewProjectionMatrix) * (vertexShader.modelViewProjectionMatrix.inverse() * viewport.inverse());
                fragmentShader.modelViewMatrix = vertexShader.modelViewMatrix;
                fragmentShader.modelViewProjectionMatrix = vertexShader.modelViewProjectionMatrix;

                Pipeline::Execute<V>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {},
                                     vertexShader, fragmentShader, interpolator, postprocessor);
            }

            return fBuffer;
        }


    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
