//
// Created by ultcrt on 24-1-10.
//

#include <iostream>
#include "shaders/NormalMappingMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        IMeshVarying
        NormalMappingMeshInterpolator::operator()(const std::array<IMeshVarying, 3> &varyings,
                                                         const Math::Vector3D &weights) const {
            IMeshVarying res;
            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1] + varyings[2].normal * weights[2];
            res.tangent = varyings[0].tangent * weights[0] + varyings[1].tangent * weights[1] + varyings[2].tangent * weights[2];
            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1] + varyings[2].light * weights[2];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1] + varyings[2].intensity * weights[2];

            return res;
        }

        IMeshVarying
        NormalMappingMeshInterpolator::operator()(const std::array<IMeshVarying, 2> &varyings,
                                                         const Math::Vector2D &weights) const {
            IMeshVarying res;
            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] ;
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1];
            res.tangent = varyings[0].tangent * weights[0] + varyings[1].tangent * weights[1];
            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1];

            return res;
        }

        IMeshVarying NormalMappingMeshVertexShader::operator()(std::size_t vIdx) const {
            IMeshVarying res;

            res.uv = (*pUvs)[vIdx];
            res.position = modelViewProjectionMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1);
            // Tips: In orthogonal projection, the inverse and transpose of M is itself
            res.normal = (modelViewMatrix * (*pNormals)[vIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            res.tangent = (modelViewMatrix * (*pTangents)[vIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            // Light is in world space
            res.light = ((*pView) * (*pLight).toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            res.intensity = intensity;

            return res;
        }

        bool NormalMappingMeshFragmentShader::operator()(const IMeshVarying &varying, Math::Vector4D &color,
                                                                double &depth) const {
            // Apply intensity here
            Math::Vector3D light = varying.light * varying.intensity;
            Math::Vector3D rgb;
            if ((*pTexture).type() == Data::ImageFormat::GRAY) {
                rgb = (*pTexture).get<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0] * Math::Vector3D{1, 1, 1};
            }
            else {
                rgb = (*pTexture).get<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);
            }            Math::Vector3D normal = (*pNormalMap).get<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]) * 2. - Math::Vector3D{1, 1, 1};
            double shininess = (*pSpecular).get<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0];

            // TODO: Non-normal mapping should be checked here
            if (normalMapType == Data::NormalMapType::DARBOUX) {
                // Make sure TBN are orthogonal
                auto t = varying.tangent;
                auto n = varying.normal;
                auto b = n.cross(t);
                t = b.cross(n);

                t.normalize();
                b.normalize();
                n.normalize();

                // Just a rotation matrix of TBN basis
                Math::Matrix3D tbn = {
                        t.x(), b.x(), n.x(),
                        t.y(), b.y(), n.y(),
                        t.z(), b.z(), n.z(),
                };

                normal = (tbn * normal).normalized();
            }
            else {
                normal = (modelViewMatrix * normal.toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            }

            color = (normal.dot(-light) * rgb).toHomogeneousCoordinates(1);
            return true;
        }
    } // Shaders
} // UltRenderer