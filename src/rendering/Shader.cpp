//
// Created by ultcrt on 24-1-4.
//

#include "rendering/Shader.h"

namespace UltRenderer {
    namespace Rendering {
        Shader::Shader(const Math::Transform3D &m, const Math::Transform3D &v, const Math::Transform3D &p,
                       const Math::Transform3D &vp, const Data::Image& t): model(m), view(v), projection(p), viewport(vp), texture(t) {}

        Math::Vector4D Shader::vertex(const Math::Vector3D &vertex) {
            const Math::Vector4D position = viewport * projection * view * model * vertex.toHomogeneousCoordinates(1);
            _positions.emplace_back(position);
            return position;
        }

        Math::Vector4D Shader::fragment(const Math::Vector3D &barycentric, Utils::MatrixProxy<double, 4, 1> &pixel) {
            return UltRenderer::Math::Vector4D();
        }
    } // Rendering
} // UltRender