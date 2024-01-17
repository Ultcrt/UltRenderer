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
#include "rendering/Helper.h"

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


            static Math::Transform3D ComputeProjectionMatrix(double width, double height, double zMin, double zMax, ProjectionType projectionType);

            static Math::Transform3D ComputeViewportMatrix(std::size_t width, std::size_t height);
        };

        template<std::derived_from<Shaders::IVarying> V>
        Data::Image Camera::render(std::size_t width, std::size_t height, Shaders::IMeshVertexShader<V> &vertexShader, Shaders::IMeshFragmentShader<V> &fragmentShader,
                                   const Shaders::IInterpolator<V> &interpolator, const Postprocessors::IPostprocessor& postprocessor) const {
            // Origin is always (0, 0) here, depth is scaled into (0, 1)
            Math::Transform3D viewport = ComputeViewportMatrix(width, height);

            Data::Image fBuffer(width, height, Data::ImageFormat::RGBA);
            Data::Image zBuffer(width, height, Data::Pixel<Data::ImageFormat::GRAY>(1));

            Data::Image shadowMap(width, height, Data::Pixel<Data::ImageFormat::GRAY>(1));

            // viewport * projection * view
            for (const auto &pMesh: _pScene->meshes()) {
                const Math::Transform3D view = transformMatrix.inverse();

                // TODO: Compute only the first light here for simplicity, which is wrong
                const Rendering::Light& light = *_pScene->lights()[0];

                // Shadow mapping
                // TODO: Shadow mapping's projection matrix may not be identical to camera's, e.g. directional light should use orthogonal projection.
                const auto lightMatrix = Rendering::RenderDepthImageOfMesh(*pMesh, light.direction, shadowMap);

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
                fragmentShader.pShadowMap = &shadowMap;
                fragmentShader.lightMatrix = lightMatrix * (vertexShader.modelViewProjectionMatrix.inverse() * viewport.inverse());
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
