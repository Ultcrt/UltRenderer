//
// Created by ultcrt on 24-1-7.
//

#ifndef ULTRENDERER_FLATSHADER_H
#define ULTRENDERER_FLATSHADER_H

#include "shaders/IShader.h"

namespace UltRenderer {
    namespace Shaders {
        struct FlatVarying: public IVarying {
            // TODO: Not support 3D texture for now
            Math::Vector3D uv;
            Math::Vector3D normal;
        };

        class FlatInterpolator: public IInterpolator<FlatVarying> {
        public:
            FlatVarying operator()(const std::array<FlatVarying, 3>& varyings, const Math::Vector3D& weights) const override;
            FlatVarying operator()(const std::array<FlatVarying, 2>& varyings, const Math::Vector2D& weights) const override;
        };

        class FlatVertexShader: public IVertexShader<FlatVarying> {
        public:
            // Uniforms
            const Math::Transform3D& model;
            const Math::Transform3D& view;
            const Math::Transform3D& projection;

            // Attributes
            const std::vector<Math::Vector3D>& vertices;
            const std::vector<Math::Vector3D>& normals;
            const std::vector<Math::Vector3D>& uvs;

            FlatVertexShader(const std::vector<Math::Vector3D>& vs, const std::vector<Math::Vector3D>& ns, const std::vector<Math::Vector3D>& uvs, const Math::Transform3D& model, const Math::Transform3D& view, const Math::Transform3D& projection);

            FlatVarying operator()(std::size_t vIdx) const override;
        };

        class FlatFragmentShader: public IFragmentShader<FlatVarying> {
        public:
            // Uniforms
            const Data::Image& texture;
            const Math::Vector3D& light;

            FlatFragmentShader(const Data::Image& t, const Math::Vector3D& l);

            bool operator()(const FlatVarying& varying, Math::Vector4D& color, double& depth) const override;
        };
    } // Shaders
} // UltRenderer

#endif //ULTRENDERER_FLATSHADER_H
