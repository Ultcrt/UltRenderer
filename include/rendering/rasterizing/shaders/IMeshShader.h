//
// Created by ultcrt on 24-1-9.
//

#ifndef ULTRENDERER_IMESHSHADER_H
#define ULTRENDERER_IMESHSHADER_H

#include "IShader.h"
#include "rendering/material/CommonMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            namespace Shaders {
                struct IMeshVarying: public IVarying {
                    // TODO: Not support 3D texture for now
                    Math::Vector3D uv;
                    Math::Vector3D normal;
                    Math::Vector3D tangent;
                    Math::Vector3D light;
                    Math::Vector3D intensity;
                };

                template <std::derived_from<IMeshVarying> V>
                class IMeshVertexShader: public IVertexShader<V> {
                public:
                    // Uniforms
                    const Math::Transform3D* pModel;
                    const Math::Transform3D* pView;
                    const Math::Transform3D* pProjection;
                    const Math::Vector3D* pLight;
                    Math::Vector3D intensity;
                    Math::Transform3D modelViewMatrix;
                    Math::Transform3D modelViewProjectionMatrix;

                    // Attributes
                    const std::vector<Math::Vector3D>* pVertices;
                    const std::vector<Math::Vector3D>* pNormals;
                    const std::vector<Math::Vector3D>* pTangents;
                    const std::vector<Math::Vector3D>* pUvs;

                    V operator()(std::size_t vIdx, Math::Vector4D& position) const override = 0;
                };

                template <std::derived_from<IMeshVarying> V>
                class IMeshFragmentShader: public IFragmentShader<V> {
                public:
                    // Uniforms
                    const Data::Image* pShadowMap;
                    const Data::Image* pLastDepthLayer;
                    const Math::Transform3D* pModel;
                    const Math::Transform3D* pView;
                    const Math::Transform3D* pProjection;
                    const Rendering::Material::CommonMaterial* pMaterial;
                    // Transformation from camera viewport to light viewport
                    Math::Transform3D lightMatrix;
                    Math::Transform3D modelViewMatrix;
                    Math::Transform3D modelViewProjectionMatrix;

                    bool operator()(const V& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override = 0;
                };
            } // Shaders
        } // Rasterizing
    } // Rendering

} // UltRenderer

#endif //ULTRENDERER_IMESHSHADER_H
