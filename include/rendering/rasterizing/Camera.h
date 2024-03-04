//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_RASTERIZING_CAMERA_H
#define ULTRENDERER_RASTERIZING_CAMERA_H

#include <ranges>

#include "math/Matrix.h"
#include "math/Transform.h"
#include "data/TriangleMesh.h"
#include "Rasterizing.h"
#include "Pipeline.h"
#include "rendering/rasterizing/shaders/IMeshShader.h"
#include "rendering/ICamera.h"
#include "rendering/Scene.h"
#include "rendering/rasterizing/shaders/DepthMeshShader.h"
#include "postprocessors/EmptyPostprocessor.h"
#include "Helper.h"
#include "rendering/rasterizing/shaders/DepthPeelingMeshShader.h"
#include "rendering/light/INonAreaLight.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            // TODO: Should inherit from base options
            struct RenderOptions {
                // TODO: Automatically decide numDepthPeelingLayer
                std::size_t numDepthPeelingLayer = 10;
                Data::Color<Data::ColorFormat::RGBA> backgroundColor = {0, 0, 0, 1};
                // TODO: Should put post processor here
            };

            class Camera: public ICamera {
            public:
                Camera(double width, double height,
                       double zMin = 0.1, double zMax = 10, ProjectionType projectionType = ProjectionType::PERSPECTIVE);

                Math::Transform3D projectionMatrix;

                [[nodiscard]] double width() const;
                [[nodiscard]] double height() const;
                [[nodiscard]] double zMin() const;
                [[nodiscard]] double zMax() const;

                void updateProjectionMatrix();

                void setProjectionType(ProjectionType projectionType) override;
                void setWidth(double width) override;
                void setHeight(double height) override;
                void setZMin(double zMin) override;
                void setZMax(double zMax) override;
                [[nodiscard]] Data::Image render(std::size_t width, std::size_t height) const override;

                template<std::derived_from<Shaders::IVarying> V>
                [[nodiscard]] Data::Image render(std::size_t width, std::size_t height, Shaders::IMeshVertexShader<V> &vertexShader, Shaders::IMeshFragmentShader<V> &fragmentShader,
                                                 const Shaders::IInterpolator<V> &interpolator, const RenderOptions& options = {}, const Postprocessors::IPostprocessor& postprocessor = Postprocessors::EmptyPostprocessor()) const;

                static Math::Transform3D ComputeProjectionMatrix(double width, double height, double zMin, double zMax, ProjectionType projectionType);

                static Math::Transform3D ComputeViewportMatrix(std::size_t width, std::size_t height);
            };

            template<std::derived_from<Shaders::IVarying> V>
            Data::Image Camera::render(std::size_t width, std::size_t height, Shaders::IMeshVertexShader<V> &vertexShader, Shaders::IMeshFragmentShader<V> &fragmentShader,
                                       const Shaders::IInterpolator<V> &interpolator, const RenderOptions& options, const Postprocessors::IPostprocessor& postprocessor) const {
                // Origin is always (0, 0) here, depth is scaled into (0, 1)
                Math::Transform3D viewport = ComputeViewportMatrix(width, height);

                Data::Image fBuffer(width, height, Data::ColorFormat::RGBA);
                Data::Image zBufferA(width, height, Data::Color<Data::ColorFormat::GRAY>(0));
                Data::Image zBufferB(width, height, Data::Color<Data::ColorFormat::GRAY>(1));

                Data::Image shadowMap(width, height, Data::Color<Data::ColorFormat::GRAY>(1));

                // TODO: Compute only the first light here for simplicity, which is wrong
                const Rendering::Light::INonAreaLight& light = *_pScene->nonAreaLights()[0];
                const auto lightDir = light.getDirectionAt({0, 0, 0});

                // Shadow mapping
                // TODO: Shadow mapping's projection matrix may not be identical to camera's, e.g. directional light should use orthogonal projection.
                Math::Transform3D lightModelView;
                Math::Transform3D lightProjection;
                Math::Transform3D lightViewport;
                // TODO: Only support directional light
                RenderDepthImageOfMeshes(_pScene->meshes(), lightDir, shadowMap, &lightModelView,
                                         &lightProjection, &lightViewport);

                const Math::Transform3D view = transformMatrix.inverse();

                // Depth peeling
                Data::Image* pLastDepthLayer = &zBufferA;
                Data::Image* pCurDepthLayer = &zBufferB;
                std::vector<Data::Image> layers;
                for (std::size_t layerIdx = 0; layerIdx < options.numDepthPeelingLayer; layerIdx++) {
                    // Clean up for recording current layer
                    fBuffer.fill(0);
                    pCurDepthLayer->fill(1);

                    for (const auto &pMesh: _pScene->meshes()) {
                        // TODO: nullptr is never checked
                        // Set IMeshVertexShader general uniforms
                        vertexShader.pModel = &pMesh->transformMatrix;
                        vertexShader.pView = &view;
                        vertexShader.pProjection = &projectionMatrix;
                        vertexShader.pLight = &lightDir;
                        vertexShader.intensity = light.intensity;
                        vertexShader.modelViewMatrix = view * pMesh->transformMatrix;
                        vertexShader.modelViewProjectionMatrix = projectionMatrix * vertexShader.modelViewMatrix;

                        // Set IMeshVertexShader general attributes
                        vertexShader.pVertices = &pMesh->vertices;
                        vertexShader.pNormals = &pMesh->vertexNormals;
                        vertexShader.pTangents = &pMesh->vertexTangents;
                        vertexShader.pUvs = &pMesh->vertexTextures;

                        // Set IMeshFragmentShader general uniforms
                        fragmentShader.pMaterial = pMesh->pMaterial.get();
                        fragmentShader.pModel = vertexShader.pModel;
                        fragmentShader.pView = vertexShader.pView;
                        fragmentShader.pProjection = vertexShader.pProjection;
                        fragmentShader.pShadowMap = &shadowMap;
                        fragmentShader.lightMatrix = (lightViewport * lightProjection * lightModelView) * (vertexShader.modelViewProjectionMatrix.inverse() * viewport.inverse());
                        fragmentShader.modelViewMatrix = vertexShader.modelViewMatrix;
                        fragmentShader.modelViewProjectionMatrix = vertexShader.modelViewProjectionMatrix;
                        fragmentShader.pLastDepthLayer = pLastDepthLayer;

                        Pipeline::Execute<V>(fBuffer, *pCurDepthLayer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {},
                                             vertexShader, fragmentShader, interpolator, postprocessor);
                    }

                    // Record current layer RGBA
                    layers.emplace_back(fBuffer);

                    // Swap zBuffer
                    std::swap(pLastDepthLayer, pCurDepthLayer);
                }

                // Blend all layers
                for (std::size_t w = 0; w < width; w++) {
                    for (std::size_t h = 0; h < height; h++) {
                        Math::Vector4D rgba = options.backgroundColor;
                        for (const Data::Image & layer : std::ranges::reverse_view(layers)) {
                            // TODO: Should implement more blending type
                            const auto topRGBA = Math::Vector4D(layer.at<Data::ColorFormat::RGBA>(w, h));
                            rgba = topRGBA * topRGBA.w() + rgba * (1 - topRGBA.w());
                        }

                        fBuffer.at<Data::ColorFormat::RGBA>(w, h) = rgba;
                    }
                }

                return fBuffer;
            }
        } // Rasterizing
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_RASTERIZING_CAMERA_H
