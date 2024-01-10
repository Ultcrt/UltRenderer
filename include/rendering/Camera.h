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
#include "shaders/IMeshShader.h"
#include "rendering/ICamera.h"
#include "shaders/PhongMeshShader.h"

namespace UltRenderer {
    namespace Rendering {
        template <
                std::derived_from<Shaders::IVarying> V = Shaders::PhongMeshVarying,
                std::derived_from<Shaders::IInterpolator<V>> IT = Shaders::PhongMeshInterpolator,
                std::derived_from<Shaders::IMeshVertexShader<V>> VS = Shaders::PhongMeshVertexShader,
                std::derived_from<Shaders::IMeshFragmentShader<V>> FS = Shaders::PhongMeshFragmentShader
                >
        class Camera: public ICamera {
        private:
            VS& _vertexShader;
            FS& _fragmentShader;
            const IT& _interpolator;

        public:
            Camera(double width, double height, VS& vertexShader, FS& fragmentShader, const IT& interpolator, double zMin=0.1, double zMax=10, ProjectionType projectionType=ProjectionType::PERSPECTIVE);

            [[nodiscard]] Data::Image render(std::size_t width, std::size_t height) const override;
        };

        template <std::derived_from<Shaders::IVarying> V, std::derived_from<Shaders::IInterpolator<V>> IT, std::derived_from<Shaders::IMeshVertexShader<V>> VS, std::derived_from<Shaders::IMeshFragmentShader<V>> FS>
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
                const Math::Transform3D view = transformMatrix.inverse();

                // TODO: Compute only the first light here for simplicity, which is wrong
                Math::Vector3D light = _pScene->lights()[0]->position;

                // Set IMeshVertexShader general uniforms
                _vertexShader.pModel = &pMesh->transformMatrix;
                _vertexShader.pView = &view;
                _vertexShader.pProjection = &projectionMatrix;
                _vertexShader.pLight = &light;

                // Set IMeshVertexShader general attributes
                _vertexShader.pVertices = &pMesh->vertices;
                _vertexShader.pNormals = &pMesh->vertexNormals;
                _vertexShader.pUvs = &pMesh->vertexTextures;

                // Set IMeshFragmentShader general attributes
                _fragmentShader.pTexture = pMesh->pTexture.get();
                _fragmentShader.pLight = &light;

                Pipeline::Execute<V, IT, VS, FS>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {}, _vertexShader, _fragmentShader, _interpolator);

            }

            return fBuffer;
        }

        template <std::derived_from<Shaders::IVarying> V, std::derived_from<Shaders::IInterpolator<V>> IT, std::derived_from<Shaders::IMeshVertexShader<V>> VS, std::derived_from<Shaders::IMeshFragmentShader<V>> FS>
        Camera<V, IT, VS, FS>::Camera(double width, double height,
                                      VS& vertexShader, FS& fragmentShader, const IT& interpolator,
                                      double zMin, double zMax, ProjectionType projectionType) :
                                      ICamera(width, height, zMin, zMax, projectionType),
                                      _vertexShader(vertexShader), _fragmentShader(fragmentShader), _interpolator(interpolator){}
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
