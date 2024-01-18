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

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.normal = (varyings[0].normal + varyings[1].normal).normalized();
            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1];

            return res;
        }

        IMeshVarying
        FlatMeshInterpolator::operator()(const std::array<IMeshVarying, 3> &varyings, const Math::Vector3D &weights) const {
            IMeshVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];

            // TODO: Use the average normal of three vertex to represent the surface normal, not ideal.
            res.normal = (varyings[0].normal + varyings[1].normal + varyings[2].normal).normalized();

            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1] + varyings[2].light * weights[2];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1] + varyings[2].intensity * weights[2];

            return res;
        }

        // TODO: Should apply here?
        IMeshVarying FlatMeshVertexShader::operator()(std::size_t vIdx) const {
            IMeshVarying res;

            res.position = modelViewProjectionMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            res.uv = (*pUvs)[vIdx];

            res.normal = (modelViewMatrix * (*pNormals)[vIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();

            res.light = ((*pView) * (*pLight).toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            res.intensity = intensity;

            return res;
        }

        bool FlatMeshFragmentShader::operator()(const UltRenderer::Shaders::IMeshVarying &varying, Math::Vector4D &color,
                                                double &depth) const {
            Math::Vector3D rgb;
            if ((*pTexture).type() == Data::ImageFormat::GRAY) {
                rgb = (*pTexture).get<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0] * Math::Vector3D{1, 1, 1};
            }
            else {
                rgb = (*pTexture).get<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);
            }

            color = (varying.normal.dot(-varying.light * varying.intensity) * rgb).toHomogeneousCoordinates(1);

            return true;
        }
    } // Shaders
} // UltRenderer