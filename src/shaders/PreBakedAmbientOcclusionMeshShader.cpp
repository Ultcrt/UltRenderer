//
// Created by ultcrt on 24-1-16.
//

#include "shaders/PreBakedAmbientOcclusionMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        PreBakedAmbientOcclusionMeshFragmentShader::PreBakedAmbientOcclusionMeshFragmentShader(
                Data::Image &bakedTexture): _bakedTexture(bakedTexture) {}

        bool PreBakedAmbientOcclusionMeshFragmentShader::operator()(const IMeshVarying &varying, Math::Vector4D &color,
                                                                    double &depth) const {
            if ((*pShadowMap).at<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0] >= varying.position.z()) {
                _bakedTexture.at<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0] = 1;
            }
            return true;
        }
    } // Shaders
} // UltRenderer