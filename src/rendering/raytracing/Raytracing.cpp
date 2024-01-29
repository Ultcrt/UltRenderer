//
// Created by ultcrt on 24-1-22.
//

#include <iostream>
#include "rendering/raytracing/Raytracing.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            Math::Vector4D Cast(
                    const Math::Ray& ray,
                    const Scene* pScene,
                    const Math::Vector4D& backgroundColor,
                    double eps,
                    std::size_t maxRecursion) {
                Math::Vector4D color = backgroundColor;

                // Restrict max recursion number
                if (maxRecursion != 0) {
                    const auto info = ray.intersect(pScene->meshes());

                    if (info.isIntersected) {
                        const auto& mesh = *info.pMesh;
                        const auto& baryCentricCoord = info.barycentricCoord;
                        const auto& triangle = mesh.triangles[info.triangleIdx];

                        const auto& uv0 = mesh.vertexTextures[triangle[0]];
                        const auto& uv1 = mesh.vertexTextures[triangle[1]];
                        const auto& uv2 = mesh.vertexTextures[triangle[2]];

                        const Math::Vector3D uv = uv0 * baryCentricCoord[0] + uv1 * baryCentricCoord[1] + uv2 * baryCentricCoord[2];

                        // TODO: Should make a option to interpolate normal
                        Math::Vector3D normal = mesh.pNormalMap->get<Data::ImageFormat::RGB>(uv[0], uv[1]) * 2 - Math::Vector3D(1, 1, 1);
                        if (mesh.normalMapType == Data::NormalMapType::DARBOUX) {
                            const auto& triangleNormal = (mesh.transformMatrix * mesh.triangleNormals[info.triangleIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates();
                            const auto& triangleTangent = (mesh.transformMatrix * mesh.triangleTangents[info.triangleIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates();

                            normal = Math::Geometry::ConvertDarbouxNormalToGlobal(triangleTangent, triangleNormal, normal);
                        }

                        // TODO: Reflection checking here is only for testing, not ideal
                        if (true) {
                            Math::Vector3D rgb;
                            if (mesh.pTexture->type() == Data::ImageFormat::GRAY) {
                                rgb = mesh.pTexture->get<Data::ImageFormat::GRAY>(uv[0], uv[1]).to<Data::ImageFormat::RGB>();
                            }
                            else {
                                rgb = mesh.pTexture->get<Data::ImageFormat::RGB>(uv[0], uv[1]);
                            }

                            // Glow
                            Math::Vector3D glowColor = mesh.pGlowMap ? static_cast<Math::Vector3D>((*mesh.pSpecularMap).get<Data::ImageFormat::RGB>(uv[0], uv[1])) : Math::Vector3D{0, 0, 0};

                            // Blinn-Phong reflection model
                            // TODO: Only one light is considered here
                            const auto& light = pScene->lights()[0]->direction * pScene->lights()[0]->intensity;
                            // TODO: Code can be reused here
                            double brightness = mesh.pSpecularMap ? (*mesh.pSpecularMap).get<Data::ImageFormat::GRAY>(uv[0], uv[1])[0] * static_cast<double>(std::numeric_limits<uint8_t>::max()) : 0;
                            Data::Pixel<Data::ImageFormat::RGB> finalSpecularColor = {1, 1, 1};
                            if (mesh.pSpecularMap && mesh.pSpecularMap->type() == Data::ImageFormat::RGB) {
                                finalSpecularColor = (*mesh.pSpecularMap).get<Data::ImageFormat::RGB>(uv[0], uv[1]);
                                brightness = finalSpecularColor.to<Data::ImageFormat::GRAY>()[0] * static_cast<double>(std::numeric_limits<uint8_t>::max());
                            }

                            // Diffuse
                            double diffuse = normal.dot(-light);

                            // Specular
                            Math::Vector3D viewDir = -ray.direction;     // View direction is the ray direction
                            Math::Vector3D halfVec = ((viewDir + -light) / 2).normalized();
                            double specular = std::pow(std::max(halfVec.dot(normal), 0.), brightness);         // uint8_t is the correct form of data, need convert

                            // TODO: Test code here
                            double diffuseCoefficient = 1;
                            double specularCoefficient = 0.5;
                            double glowIntensity = 0.5;

                            color = (
                                diffuseCoefficient * diffuse * rgb +
                                specularCoefficient * specular * finalSpecularColor +
                                glowIntensity * glowColor
                            ).toHomogeneousCoordinates(1);
                        }
                        else {
                            const auto& intersectedPoint = ray.origin + ray.direction * (info.length - eps);
                            const auto& reflectionDirection = Math::Geometry::ComputeReflectionDirection(normal, ray.direction);

                            const auto& reflectionRay = Math::Ray(intersectedPoint, reflectionDirection);

                            color = Cast(reflectionRay, pScene, backgroundColor, eps, maxRecursion - 1) * 0.6;
                            color.w() = 1;
                        }
                    }
                }

                return color;
            }
        } // Raytracing
    } // Rendering
} // UltRenderer