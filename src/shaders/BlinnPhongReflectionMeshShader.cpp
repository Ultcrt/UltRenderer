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
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1] + varyings[2].normal * weights[2];
            res.tangent = varyings[0].tangent * weights[0] + varyings[1].tangent * weights[1] + varyings[2].tangent * weights[2];
            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1] + varyings[2].light * weights[2];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1] + varyings[2].intensity * weights[2];
            res.viewPosition = varyings[0].viewPosition * weights[0] + varyings[1].viewPosition * weights[1] + varyings[2].viewPosition * weights[2];

            return res;
        }

        BlinnPhongReflectionMeshVarying
        BlinnPhongReflectionMeshInterpolator::operator()(const std::array<BlinnPhongReflectionMeshVarying, 2> &varyings,
                                                  const Math::Vector2D &weights) const {
            BlinnPhongReflectionMeshVarying res;
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1];
            res.tangent = varyings[0].tangent * weights[0] + varyings[1].tangent * weights[1];
            res.light = varyings[0].light * weights[0] + varyings[1].light * weights[1];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1];
            res.viewPosition = varyings[0].viewPosition * weights[0] + varyings[1].viewPosition * weights[1];

            return res;
        }

        BlinnPhongReflectionMeshVarying BlinnPhongReflectionMeshVertexShader::operator()(std::size_t vIdx, Math::Vector4D& position) const {
            BlinnPhongReflectionMeshVarying res;

            res.uv = (*pUvs)[vIdx];
            // Transform the vector (the w of 3D vector is zero)
            res.normal = (modelViewMatrix * (*pNormals)[vIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            res.tangent = (modelViewMatrix * (*pTangents)[vIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            // Light is in world space
            res.light = ((*pView) * (*pLight).toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
            res.intensity = intensity;
            res.viewPosition = (modelViewMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1)).toCartesianCoordinates();

            position = modelViewProjectionMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            return res;
        }

        bool BlinnPhongReflectionMeshFragmentShader::operator()(const BlinnPhongReflectionMeshVarying &varying, const Math::Vector4D& fragCoord, Math::Vector4D &color,
                                                         double &depth) const {
            if ((*pLastDepthLayer).at<Data::ImageFormat::GRAY>(static_cast<std::size_t>(fragCoord.x()), static_cast<std::size_t>(fragCoord.y()))[0] < depth) {
                // Apply intensity here
                Math::Vector3D light = varying.light * varying.intensity;
                Math::Vector3D normal = (*pNormalMap).get<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]) * 2. - Math::Vector3D{1, 1, 1};
                auto shadowPosition = lightMatrix * Math::Vector4D(fragCoord.x(), fragCoord.y(), fragCoord.z(), 1);
                // 0.01 is a coefficient to fix z-fighting
                bool inShadow = shadowPosition.z() - 0.01 > (*pShadowMap).at<Data::ImageFormat::GRAY>(static_cast<std::size_t>(shadowPosition.x()),  static_cast<std::size_t>(shadowPosition.y()))[0];
                Math::Vector3D glowColor = (*pGlowMap).get<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);

                Math::Vector3D rgb;
                if ((*pTexture).type() == Data::ImageFormat::GRAY) {
                    rgb = (*pTexture).get<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0] * Math::Vector3D{1, 1, 1};
                }
                else {
                    rgb = (*pTexture).get<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);
                }

                // Specular map can be RGB, if so, use RGB as specular color and grayscale of the map as brightness
                double brightness = (*pSpecular).get<Data::ImageFormat::GRAY>(varying.uv[0], varying.uv[1])[0] * static_cast<double>(std::numeric_limits<uint8_t>::max());
                Data::Pixel<Data::ImageFormat::RGB> finalSpecularColor = specularColor;
                if (pSpecular->type() == Data::ImageFormat::RGB) {
                    finalSpecularColor = (*pSpecular).get<Data::ImageFormat::RGB>(varying.uv[0], varying.uv[1]);
                    brightness = finalSpecularColor.to<Data::ImageFormat::GRAY>()[0] * static_cast<double>(std::numeric_limits<uint8_t>::max());
                }

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

                // Diffuse
                double diffuse = normal.dot(-light);

                // Specular
                Math::Vector3D viewDir = (-varying.viewPosition).normalized();     // Camera is always at origin, so the view direction can be represented by the position
                Math::Vector3D halfVec = ((viewDir + -light) / 2).normalized();
                double specular = std::pow(std::max(halfVec.dot(normal), 0.), brightness);         // uint8_t is the correct form of data, need convert

                color = (
                        ambientCoefficient * ambientColor +
                        diffuseCoefficient * diffuse * rgb +
                        specularCoefficient * specular * finalSpecularColor +
                        glowColor * glowIntensity).toHomogeneousCoordinates(1);

                if (inShadow) {
                    color = shadowIntensity * color;
                    color.w() = 1;
                }

                return true;
            }

            return false;
        }
    } // Shaders
} // UltRenderer