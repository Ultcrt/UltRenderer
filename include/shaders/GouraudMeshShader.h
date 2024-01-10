//
// Created by ultcrt on 24-1-7.
//

#ifndef ULTRENDERER_GOURAUDMESHSHADER_H
#define ULTRENDERER_GOURAUDMESHSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct GouraudMeshVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
            double intensity;
        };

        class GouraudMeshInterpolator: public IInterpolator<GouraudMeshVarying> {
        public:
            GouraudMeshVarying operator()(const std::array<GouraudMeshVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            GouraudMeshVarying operator()(const std::array<GouraudMeshVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class GouraudMeshVertexShader: public IMeshVertexShader<GouraudMeshVarying> {
        public:
            GouraudMeshVarying operator()(std::size_t vIdx) const override;
        };

        class GouraudMeshFragmentShader: public IMeshFragmentShader<GouraudMeshVarying> {
        public:
            bool operator()(const GouraudMeshVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_GOURAUDMESHSHADER_H
