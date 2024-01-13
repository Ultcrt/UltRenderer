//
// Created by ultcrt on 24-1-9.
//

#ifndef ULTRENDERER_IMESHSHADER_H
#define ULTRENDERER_IMESHSHADER_H

#include "IShader.h"

namespace UltRenderer {
    namespace Shaders {
        template <std::derived_from<IVarying> V>
        class IMeshVertexShader: public IVertexShader<V> {
        public:
            // Uniforms
            const Math::Transform3D* pModel;
            const Math::Transform3D* pView;
            const Math::Transform3D* pProjection;
            const Math::Vector3D* pLight;

            // Attributes
            const std::vector<Math::Vector3D>* pVertices;
            const std::vector<Math::Vector3D>* pNormals;
            const std::vector<Math::Vector3D>* pTangents;
            const std::vector<Math::Vector3D>* pUvs;

            V operator()(std::size_t vIdx) const override = 0;
        };

        template <std::derived_from<IVarying> V>
        class IMeshFragmentShader: public IFragmentShader<V> {
        public:
            // Uniforms
            const Math::Transform3D* pModel;
            const Math::Transform3D* pView;
            const Math::Transform3D* pProjection;
            const Data::Image* pTexture;
            const Data::Image* pNormalMap;
            const Data::NormalMapType* pNormalMapType;
            const Data::Image* pSpecular;
            const Math::Vector3D* pLight;

            bool operator()(const V& varying, Math::Vector4D& color, double& depth) const override = 0;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_IMESHSHADER_H
