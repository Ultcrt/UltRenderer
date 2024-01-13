//
// Created by ultcrt on 24-1-7.
//

#include <iostream>
#include "shaders/GouraudMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        IMeshVarying
        GouraudMeshInterpolator::operator()(const std::array<IMeshVarying, 2> &varyings, const Math::Vector2D &weights) const {
            IMeshVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1];

            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1];

            return res;
        }

        IMeshVarying
        GouraudMeshInterpolator::operator()(const std::array<IMeshVarying, 3> &varyings, const Math::Vector3D &weights) const {
            IMeshVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1] + varyings[2].intensity * weights[2];

            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1] + varyings[2].light * weights[2];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1] + varyings[2].intensity * weights[2];

            return res;
        }

        IMeshVarying GouraudMeshVertexShader::operator()(std::size_t vIdx) const {
            IMeshVarying res;

            res.normal = (modelViewMatrix * (*pNormals)[vIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            res.light = ((*pView) * (*pLight).toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();

            res.intensity = (-res.light).dot(res.normal) * intensity;
            res.uv = (*pUvs)[vIdx];

            res.position = modelViewProjectionMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            return res;
        }

        bool GouraudMeshFragmentShader::operator()(const UltRenderer::Shaders::IMeshVarying &varying, Math::Vector4D &color,
                                                   double &depth) const {
            Math::Vector3D rgb = (*pTexture).at<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);

            color = (varying.intensity * rgb).toHomogeneousCoordinates(1);

            return true;
        }
    } // Shaders
} // UltRenderer