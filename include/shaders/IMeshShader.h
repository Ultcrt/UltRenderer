//
// Created by ultcrt on 24-1-9.
//

#ifndef ULTRENDERER_IMESHSHADER_H
#define ULTRENDERER_IMESHSHADER_H

#include "IShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct IMeshVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
            Math::Vector3D normal;
            Math::Vector3D tangent;
            Math::Vector3D light;
            double intensity;
        };

        template <std::derived_from<IVarying> V>
        class IMeshVertexShader: public IVertexShader<V> {
        public:
            // Uniforms
            const Math::Transform3D* pModel;
            const Math::Transform3D* pView;
            const Math::Transform3D* pProjection;
            const Math::Vector3D* pLight;
            double intensity;
            Math::Transform3D modelViewMatrix;
            Math::Transform3D modelViewProjectionMatrix;

            // Attributes
            const std::vector<Math::Vector3D>* pVertices;
            const std::vector<Math::Vector3D>* pNormals;
            const std::vector<Math::Vector3D>* pTangents;
            const std::vector<Math::Vector3D>* pUvs;

            V operator()(std::size_t vIdx, Math::Vector4D& position) const override = 0;
        };

        template <std::derived_from<IVarying> V>
        class IMeshFragmentShader: public IFragmentShader<V> {
        public:
            // Uniforms
            const Data::Image* pTexture;
            const Data::Image* pNormalMap;
            const Data::Image* pSpecular;
            const Data::Image* pShadowMap;
            const Data::Image* pGlowMap;
            Data::NormalMapType normalMapType;
            const Math::Transform3D* pModel;
            const Math::Transform3D* pView;
            const Math::Transform3D* pProjection;
            // Transformation from camera viewport to light viewport
            Math::Transform3D lightMatrix;
            Math::Transform3D modelViewMatrix;
            Math::Transform3D modelViewProjectionMatrix;
            double shadowIntensity;
            double glowIntensity;

            bool operator()(const V& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const override = 0;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_IMESHSHADER_H
