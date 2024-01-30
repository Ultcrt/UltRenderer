//
// Created by ultcrt on 24-1-7.
//

#include <iostream>
#include "shaders/FlatMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        IMeshVarying
        FlatMeshInterpolator::operator()(const std::array<IMeshVarying, 2> &varyings, const Math::Vector2D &weights) const {
            IMeshVarying res;

            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.normal = (varyings[0].normal + varyings[1].normal).normalized();
            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1];

            return res;
        }

        IMeshVarying
        FlatMeshInterpolator::operator()(const std::array<IMeshVarying, 3> &varyings, const Math::Vector3D &weights) const {
            IMeshVarying res;

            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];

            // TODO: Use the average normal of three vertex to represent the surface normal, not ideal.
            res.normal = (varyings[0].normal + varyings[1].normal + varyings[2].normal).normalized();

            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1] + varyings[2].light * weights[2];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1] + varyings[2].intensity * weights[2];

            return res;
        }

        // TODO: Should apply here?
        IMeshVarying FlatMeshVertexShader::operator()(std::size_t vIdx, Math::Vector4D& position) const {
            IMeshVarying res;

            res.uv = (*pUvs)[vIdx];

            res.normal = (modelViewMatrix * (*pNormals)[vIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();

            res.light = ((*pView) * (*pLight).toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            res.intensity = intensity;

            position = modelViewProjectionMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            return res;
        }

        bool FlatMeshFragmentShader::operator()(const UltRenderer::Shaders::IMeshVarying &varying, const Math::Vector4D& fragCoord, Math::Vector4D &color,
                                                double &depth) const {
            Math::Vector3D rgb;
            if ((*pMaterial->pTexture).type() == Data::ImageFormat::GRAY) {
                rgb = (*pMaterial->pTexture).get<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0] * Math::Vector3D{1, 1, 1};
            }
            else {
                rgb = (*pMaterial->pTexture).get<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);
            }

            color = (varying.normal.dot(-varying.light * varying.intensity) * rgb).toHomogeneousCoordinates(1);

            return true;
        }
    } // Shaders
} // UltRenderer