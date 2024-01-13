//
// Created by ultcrt on 24-1-10.
//

#include <iostream>
#include "shaders/BlinnPhongReflectionMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        BlinnPhongReflectionMeshVarying
        BlinnPhongReflectionMeshInterpolator::operator()(const std::array<BlinnPhongReflectionMeshVarying, 3> &varyings,
                                                  const Math::Vector3D &weights) const {
            BlinnPhongReflectionMeshVarying res;
            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1] + varyings[2].normal * weights[2];
            res.tangent = varyings[0].tangent * weights[0] + varyings[1].tangent * weights[1] + varyings[2].tangent * weights[2];

            return res;
        }

        BlinnPhongReflectionMeshVarying
        BlinnPhongReflectionMeshInterpolator::operator()(const std::array<BlinnPhongReflectionMeshVarying, 2> &varyings,
                                                  const Math::Vector2D &weights) const {
            BlinnPhongReflectionMeshVarying res;
            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] ;
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1];
            res.tangent = varyings[0].tangent * weights[0] + varyings[1].tangent * weights[1];

            return res;
        }

        BlinnPhongReflectionMeshVarying BlinnPhongReflectionMeshVertexShader::operator()(std::size_t vIdx) const {
            BlinnPhongReflectionMeshVarying res;

            res.uv = (*pUvs)[vIdx];
            res.position = (*pProjection) * (*pView) * (*pModel) * (*pVertices)[vIdx].toHomogeneousCoordinates(1);
            res.normal = (*pNormals)[vIdx];
            res.tangent = (*pTangents)[vIdx];

            return res;
        }

        bool BlinnPhongReflectionMeshFragmentShader::operator()(const BlinnPhongReflectionMeshVarying &varying, Math::Vector4D &color,
                                                         double &depth) const {
            auto mvp = (*pProjection) * (*pView) * (*pModel);
            // Apply intensity here
            Math::Vector3D light = (mvp * (*pLight).toHomogeneousCoordinates(1)).toCartesianCoordinates().normalized() * lightIntensity;
            Math::Vector3D rgb = (*pTexture).at<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);
            Math::Vector3D normal = (*pNormalMap).at<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]) * 2. - Math::Vector3D{1, 1, 1};
            double shininess = (*pSpecular).at<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0];

            normal = (mvp.transpose().inverse() * normal.toHomogeneousCoordinates(1)).toCartesianCoordinates().normalized();

            // TODO: Too slow

            // Diffuse
            double diffuse = normal.dot(-light);

            // Specular
            Math::Vector3D viewDir = (-varying.position.toCartesianCoordinates()).normalized();     // Camera is always at origin, so the view direction can be represented by the position
            Math::Vector3D halfVec = ((viewDir + -light) / 2).normalized();
            double specular = std::pow(halfVec.dot(normal), shininess * static_cast<double>(std::numeric_limits<uint8_t>::max()));         // uint8_t is the correct form of data, need convert

            color = (
                    ambientCoefficient * ambientColor +
                    diffuseCoefficient * diffuse * rgb +
                    specularCoefficient * specular * specularColor
            ).toHomogeneousCoordinates(1);
            return true;
        }
    } // Shaders
} // UltRenderer