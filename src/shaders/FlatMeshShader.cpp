//
// Created by ultcrt on 24-1-7.
//

#include <iostream>
#include "shaders/FlatMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        FlatMeshVarying
        FlatMeshInterpolator::operator()(const std::array<FlatMeshVarying, 2> &varyings, const Math::Vector2D &weights) const {
            FlatMeshVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.normal = (varyings[0].normal + varyings[1].normal).normalized();

            return res;
        }

        FlatMeshVarying
        FlatMeshInterpolator::operator()(const std::array<FlatMeshVarying, 3> &varyings, const Math::Vector3D &weights) const {
            FlatMeshVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];

            // TODO: Use the average normal of three vertex to represent the surface normal, not ideal.
            res.normal = (varyings[0].normal + varyings[1].normal + varyings[2].normal).normalized();

            return res;
        }

        // TODO: Should apply here?
        FlatMeshVarying FlatMeshVertexShader::operator()(std::size_t vIdx) const {
            FlatMeshVarying res;

            res.position = (*pProjection) * (*pView) * (*pModel) * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            res.uv = (*pUvs)[vIdx];

            res.normal = (*pNormals)[vIdx];

            return res;
        }

        bool FlatMeshFragmentShader::operator()(const UltRenderer::Shaders::FlatMeshVarying &varying, Math::Vector4D &color,
                                                double &depth) const {
            std::size_t width = (*pTexture).width();
            std::size_t height = (*pTexture).height();

            Math::Vector3D rgb;
            switch ((*pTexture).type()) {
                case Data::ImageFormat::RGBA:
                    rgb = (*pTexture).at<Data::ImageFormat::RGBA>(std::lround(static_cast<double>(width) * varying.uv[0]), std::lround(static_cast<double>(height) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
                case Data::ImageFormat::RGB:
                    rgb = (*pTexture).at<Data::ImageFormat::RGB>(std::lround(static_cast<double>(width) * varying.uv[0]), std::lround(static_cast<double>(height) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
                case Data::ImageFormat::GRAY:
                    rgb = (*pTexture).at<Data::ImageFormat::GRAY>(std::lround(static_cast<double>(width) * varying.uv[0]), std::lround(static_cast<double>(height) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
            }

            color = (varying.normal.dot(-(*pLight)) * rgb).toHomogeneousCoordinates(1);

            return true;
        }
    } // Shaders
} // UltRenderer