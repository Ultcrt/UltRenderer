//
// Created by ultcrt on 24-1-7.
//

#ifndef ULTRENDERER_GOURAUDSHADER_H
#define ULTRENDERER_GOURAUDSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct GouraudVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
            double intensity;
        };

        class GouraudInterpolator: public IInterpolator<GouraudVarying> {
        public:
            GouraudVarying operator()(const std::array<GouraudVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            GouraudVarying operator()(const std::array<GouraudVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class GouraudVertexShader: public IMeshVertexShader<GouraudVarying> {
        public:
            GouraudVertexShader(const Math::Vector3D& l, const std::vector<Math::Vector3D>& vs, const std::vector<Math::Vector3D>& ns, const std::vector<Math::Vector3D>& uvs, const Math::Transform3D& model, const Math::Transform3D& view, const Math::Transform3D& projection);

            GouraudVarying operator()(std::size_t vIdx) const override;
        };

        class GouraudFragmentShader: public IMeshFragmentShader<GouraudVarying> {
        public:
            GouraudFragmentShader(const Data::Image& t);

            bool operator()(const GouraudVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_GOURAUDSHADER_H
