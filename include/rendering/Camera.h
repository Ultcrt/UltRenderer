//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_CAMERA_H
#define ULTRENDERER_CAMERA_H

#include "math/Matrix.h"
#include "math/Transform.h"
#include "data/TriangleMesh.h"
#include "rendering/Scene.h"
#include "rendering/Rasterize.h"
#include "rendering/Pipeline.h"
#include "shaders/PhongShader.h"
#include "rendering/ICamera.h"

namespace UltRenderer {
    namespace Rendering {
        template <
                std::derived_from<Shaders::IVarying> V = Shaders::PhongVarying,
                std::derived_from<Shaders::IInterpolator<V>> IT = Shaders::PhongInterpolator,
                std::derived_from<Shaders::IVertexShader<V>> VS = Shaders::PhongVertexShader,
                std::derived_from<Shaders::IFragmentShader<V>> FS = Shaders::PhongFragmentShader
                >
        class Camera: public ICamera {
        private:
            const VS& _vertexShader;
            const FS& _fragmentShader;
            const IT& _interpolator;

        public:
            Camera(double width, double height, const VS& vertexShader, const FS& fragmentShader, const IT& interpolator = {}, double zMin=0.1, double zMax=10, ProjectionType projectionType=ProjectionType::PERSPECTIVE);

            [[nodiscard]] Data::Image render(std::size_t width, std::size_t height) const override;
        };

        template <std::derived_from<Shaders::IVarying> V, std::derived_from<Shaders::IInterpolator<V>> IT, std::derived_from<Shaders::IVertexShader<V>> VS, std::derived_from<Shaders::IFragmentShader<V>> FS>
        Data::Image Camera<V, IT, VS, FS>::render(std::size_t width, std::size_t height) const {
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
//                Shaders::GouraudVertexShader vertexShader(light, pMesh->vertices, pMesh->vertexNormals, pMesh->vertexTextures, pMesh->transformMatrix, transformMatrix.inverse(), projectionMatrix);
//                Shaders::GouraudFragmentShader fragmentShader(*pMesh->pTexture);
//
//                Pipeline::Execute<Shaders::GouraudVarying, Shaders::GouraudInterpolator, Shaders::GouraudVertexShader, Shaders::GouraudFragmentShader>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {}, vertexShader, fragmentShader);

//                Shaders::FlatVertexShader vertexShader(pMesh->vertices, pMesh->vertexNormals, pMesh->vertexTextures, pMesh->transformMatrix, transformMatrix.inverse(), projectionMatrix);
//                Shaders::FlatFragmentShader fragmentShader(*pMesh->pTexture, light);
//
//                Pipeline::Execute<Shaders::FlatVarying, Shaders::FlatInterpolator, Shaders::FlatVertexShader, Shaders::FlatFragmentShader>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {}, vertexShader, fragmentShader);

                Shaders::PhongVertexShader vertexShader(pMesh->vertices, pMesh->vertexNormals, pMesh->vertexTextures, pMesh->transformMatrix, transformMatrix.inverse(), projectionMatrix);
                Shaders::PhongFragmentShader fragmentShader(*pMesh->pTexture, light);

                Pipeline::Execute<Shaders::PhongVarying, Shaders::PhongInterpolator, Shaders::PhongVertexShader, Shaders::PhongFragmentShader>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {}, vertexShader, fragmentShader);

            }

            return fBuffer;
        }

        template <std::derived_from<Shaders::IVarying> V, std::derived_from<Shaders::IInterpolator<V>> IT, std::derived_from<Shaders::IVertexShader<V>> VS, std::derived_from<Shaders::IFragmentShader<V>> FS>
        Camera<V, IT, VS, FS>::Camera(double width, double height,
                                      const VS& vertexShader, const FS& fragmentShader, const IT& interpolator,
                                      double zMin, double zMax, ProjectionType projectionType) :
                                      ICamera(width, height, zMin, zMax, projectionType),
                                      _vertexShader(vertexShader), _fragmentShader(fragmentShader), _interpolator(interpolator){}
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
