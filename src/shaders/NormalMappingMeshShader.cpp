//
// Created by ultcrt on 24-1-10.
//

#include <iostream>
#include "shaders/NormalMappingMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        NormalMappingMeshVarying
        NormalMappingMeshInterpolator::operator()(const std::array<NormalMappingMeshVarying, 3> &varyings,
                                            const Math::Vector3D &weights) const {
            NormalMappingMeshVarying res;
            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];

            return res;
        }

        NormalMappingMeshVarying
        NormalMappingMeshInterpolator::operator()(const std::array<NormalMappingMeshVarying, 2> &varyings,
                                            const Math::Vector2D &weights) const {
            NormalMappingMeshVarying res;
            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] ;
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];

            return res;
        }

        NormalMappingMeshVarying NormalMappingMeshVertexShader::operator()(std::size_t vIdx) const {
            NormalMappingMeshVarying res;

            res.uv = (*pUvs)[vIdx];
            res.position = (*pProjection) * (*pView) * (*pModel) * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            return res;
        }

        bool NormalMappingMeshFragmentShader::operator()(const NormalMappingMeshVarying &varying, Math::Vector4D &color,
                                                         double &depth) const {
            std::size_t tWidth = (*pTexture).width();
            std::size_t tHeight = (*pTexture).height();
            std::size_t nmWidth = (*pNormalMap).width();
            std::size_t nmHeight = (*pNormalMap).height();

            // TODO: Too slow
            auto mvp = (*pProjection) * (*pView) * (*pModel);
            Math::Vector3D normal = (*pNormalMap).at<Data::ImageFormat::RGB>(std::lround(static_cast<double>(nmWidth) * varying.uv[0]), std::lround(static_cast<double>(nmHeight) * varying.uv[1])) * 2. - Math::Vector3D{1, 1, 1};
            normal = (mvp.transpose().inverse() * normal.toHomogeneousCoordinates(1)).toCartesianCoordinates();

            Math::Vector3D light = (mvp * (*pLight).toHomogeneousCoordinates(1)).toCartesianCoordinates();

            Math::Vector3D rgb;
            switch ((*pTexture).type()) {
                case Data::ImageFormat::RGBA:
                    rgb = (*pTexture).at<Data::ImageFormat::RGBA>(std::lround(static_cast<double>(tWidth) * varying.uv[0]), std::lround(static_cast<double>(tHeight) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
                case Data::ImageFormat::RGB:
                    rgb = (*pTexture).at<Data::ImageFormat::RGB>(std::lround(static_cast<double>(tWidth) * varying.uv[0]), std::lround(static_cast<double>(tHeight) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
                case Data::ImageFormat::GRAY:
                    rgb = (*pTexture).at<Data::ImageFormat::GRAY>(std::lround(static_cast<double>(tWidth) * varying.uv[0]), std::lround(static_cast<double>(tHeight) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
            }

            color = (normal.dot(-light) * rgb).toHomogeneousCoordinates(1);

            return true;
        }
    } // Shaders
} // UltRenderer