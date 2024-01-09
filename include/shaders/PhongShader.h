//
// Created by ultcrt on 24-1-8.
//

#ifndef ULTRENDERER_PHONGSHADER_H
#define ULTRENDERER_PHONGSHADER_H

#include "shaders/IMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct PhongVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
            Math::Vector3D normal;
        };

        class PhongInterpolator: public IInterpolator<PhongVarying> {
        public:
            PhongVarying operator()(const std::array<PhongVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            PhongVarying operator()(const std::array<PhongVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class PhongVertexShader: public IMeshVertexShader<PhongVarying> {
        public:
            PhongVertexShader(const std::vector<Math::Vector3D>& vs, const std::vector<Math::Vector3D>& ns, const std::vector<Math::Vector3D>& uvs, const Math::Transform3D& model, const Math::Transform3D& view, const Math::Transform3D& projection);

            PhongVarying operator()(std::size_t vIdx) const override;
        };

        class PhongFragmentShader: public IFragmentShader<PhongVarying> {
        public:
            PhongFragmentShader(const Data::Image& t, const Math::Vector3D& l);

            bool operator()(const PhongVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_PHONGSHADER_H
