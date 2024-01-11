//
// Created by ultcrt on 24-1-8.
//

#include "shaders/PhongMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        PhongMeshVarying
        PhongMeshInterpolator::operator()(const std::array<PhongMeshVarying, 2> &varyings, const Math::Vector2D &weights) const {
            PhongMeshVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1];

            return res;
        }

        PhongMeshVarying
        PhongMeshInterpolator::operator()(const std::array<PhongMeshVarying, 3> &varyings, const Math::Vector3D &weights) const {
            PhongMeshVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1] + varyings[2].normal * weights[2];

            return res;
        }

        // TODO: Should apply here?
        PhongMeshVarying PhongMeshVertexShader::operator()(std::size_t vIdx) const {
            PhongMeshVarying res;

            res.position = (*pProjection) * (*pView) * (*pModel) * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            res.uv = (*pUvs)[vIdx];

            res.normal = (*pNormals)[vIdx];

            return res;
        }

        bool PhongMeshFragmentShader::operator()(const UltRenderer::Shaders::PhongMeshVarying &varying, Math::Vector4D &color,
                                                 double &depth) const {
            Math::Vector3D rgb = (*pTexture).at<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);

            color = (varying.normal.dot(-(*pLight)) * rgb).toHomogeneousCoordinates(1);

            return true;
        }
    } // Shaders
} // UltRenderer