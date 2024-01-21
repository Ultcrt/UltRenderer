//
// Created by ultcrt on 24-1-16.
//

#include <iostream>
#include "shaders/PreBakedAmbientOcclusionMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        IMeshVarying
        PreBakedAmbientOcclusionMeshInterpolator::operator()(const std::array<IMeshVarying, 3> &varyings,
                                                         const Math::Vector3D &weights) const {
            IMeshVarying res;
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];

            return res;
        }

        IMeshVarying
        PreBakedAmbientOcclusionMeshInterpolator::operator()(const std::array<IMeshVarying, 2> &varyings,
                                                         const Math::Vector2D &weights) const {
            IMeshVarying res;
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];

            return res;
        }

        IMeshVarying PreBakedAmbientOcclusionMeshVertexShader::operator()(std::size_t vIdx, Math::Vector4D& position) const {
            IMeshVarying res;

            res.uv = (*pUvs)[vIdx];

            position = modelViewProjectionMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            return res;
        }

        PreBakedAmbientOcclusionMeshFragmentShader::PreBakedAmbientOcclusionMeshFragmentShader(
                Data::Image &bakedTexture): _bakedTexture(bakedTexture) {}

        bool PreBakedAmbientOcclusionMeshFragmentShader::operator()(const IMeshVarying &varying, const Math::Vector4D& fragCoord, Math::Vector4D &color,
                                                                    double &depth) const {
            if ((*pShadowMap).at<Data::ImageFormat::GRAY>(static_cast<std::size_t>(fragCoord.x()),  static_cast<std::size_t>(fragCoord.y()))[0] >= fragCoord.z()) {
                _bakedTexture.at<Data::ImageFormat::GRAY>(static_cast<std::size_t>(varying.uv[0]), static_cast<std::size_t>(varying.uv[1]))[0] = 1;
            }
            return true;
        }
    } // Shaders
} // UltRenderer