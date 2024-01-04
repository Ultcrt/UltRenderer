//
// Created by ultcrt on 24-1-4.
//

#ifndef ULTRENDERER_SHADER_H
#define ULTRENDERER_SHADER_H

#include <vector>
#include "math/Matrix.h"
#include "math/Transform.h"

namespace UltRenderer {
    namespace Rendering {
        // Tips: Use private member, public member, member function param to simulate varying, uniform, attribute in glsl
        class Shader {
        protected:
            // All varyings are the protected/private member of shader class (make it possible to pass data from vertex shader to fragment shader)
            std::vector<Math::Vector4D> _positions;

        public:
            // All uniforms are the public member of shader class
            const Math::Transform3D& model;
            const Math::Transform3D& view;
            const Math::Transform3D& projection;
            const Math::Transform3D& viewport;

            Shader(const Math::Transform3D& m, const Math::Transform3D& v, const Math::Transform3D& p, const Math::Transform3D& vp);

        public:
            // All attributes are the params of member function
            virtual Math::Vector4D vertex(const Math::Vector3D& vertex);

            virtual Math::Vector4D fragment(const Math::Vector3D& barycentric);
        };

        class FragmentShader {
        public:
        };
    } // Rendering
} // UltRender

#endif //ULTRENDERER_SHADER_H
