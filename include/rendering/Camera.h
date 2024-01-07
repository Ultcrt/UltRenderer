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
#include "shaders/FlatShader.h"
#include "rendering/Pipeline.h"
#include "shaders/GouraudShader.h"

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

            UltRenderer::Data::Image fBuffer(width, height, Data::ImageFormat::RGBA);
            UltRenderer::Data::Image zBuffer(width, height, Data::ImageFormat::GRAY);

            // viewport * projection * view
            for (const auto& pMesh: _pScene->meshes()) {
                // TODO: Compute only the first light here for simplicity, which is wrong
                Math::Vector3D light = _pScene->lights()[0]->position;

                // TODO: Should pass by Scene
                Shaders::GouraudVertexShader vertexShader(light, pMesh->vertices, pMesh->vertexNormals, pMesh->vertexTextures, pMesh->transformMatrix, transformMatrix.inverse(), projectionMatrix);
                Shaders::GouraudFragmentShader fragmentShader(*pMesh->pTexture);

                Pipeline::Execute<Shaders::GouraudVarying, Shaders::GouraudInterpolator, Shaders::GouraudVertexShader, Shaders::GouraudFragmentShader>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {}, vertexShader, fragmentShader);

//                Shaders::FlatVertexShader vertexShader(pMesh->vertices, pMesh->vertexNormals, pMesh->vertexTextures, pMesh->transformMatrix, transformMatrix.inverse(), projectionMatrix);
//                Shaders::FlatFragmentShader fragmentShader(*pMesh->pTexture, light);
//
//                Pipeline::Execute<Shaders::FlatVarying, Shaders::FlatInterpolator, Shaders::FlatVertexShader, Shaders::FlatFragmentShader>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {}, vertexShader, fragmentShader);

            }

            return fBuffer;
        }
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
