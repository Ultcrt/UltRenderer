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
                    const std::vector<std::shared_ptr<Data::TriangleMesh>>& pMeshes,
                    const Math::Vector4D& backgroundColor,
                    double eps,
                    std::size_t maxRecursion) {
                Math::Vector4D color = backgroundColor;

                // Restrict max recursion number
                if (maxRecursion != 0) {
                    const auto info = ray.intersect(pMeshes);

                    if (info.isIntersected) {
                        const auto& mesh = *info.pMesh;
                        const auto& baryCentricCoord = info.barycentricCoord;
                        const auto& triangle = mesh.triangles[info.triangleIdx];

                        const auto& uv0 = mesh.vertexTextures[triangle[0]];
                        const auto& uv1 = mesh.vertexTextures[triangle[1]];
                        const auto& uv2 = mesh.vertexTextures[triangle[2]];

                        const Math::Vector3D uv = uv0 * baryCentricCoord[0] + uv1 * baryCentricCoord[1] + uv2 * baryCentricCoord[2];

                        Math::Vector3D normal = mesh.pNormalMap->get<Data::ImageFormat::RGB>(uv[0], uv[1]);
                        if (mesh.normalMapType == Data::NormalMapType::DARBOUX) {
                            const auto& triangleNormal = (mesh.transformMatrix * mesh.triangleNormals[info.triangleIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates();
                            const auto& triangleTangent = (mesh.transformMatrix * mesh.triangleTangents[info.triangleIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates();

                            normal = Math::Geometry::ConvertDarbouxNormalToGlobal(triangleTangent, triangleNormal, normal);
                        }

                        // TODO: Reflection checking here is only for testing, not ideal
                        if (mesh.pSpecularMap == nullptr) {
                            if (mesh.pTexture->type() == Data::ImageFormat::GRAY) {
                                color = mesh.pTexture->get<Data::ImageFormat::GRAY>(uv[0], uv[1]).to<Data::ImageFormat::RGBA>();
                            }
                            else {
                                color = mesh.pTexture->get<Data::ImageFormat::RGB>(uv[0], uv[1]).to<Data::ImageFormat::RGBA>();
                            }
                        }
                        else {
                            const auto& intersectedPoint = ray.origin + ray.direction * (info.length - eps);
                            const auto& reflectionDirection = Math::Geometry::ComputeReflectionDirection(normal, ray.direction);

                            const auto& reflectionRay = Math::Ray(intersectedPoint, reflectionDirection);

                            color = Cast(reflectionRay, pMeshes, backgroundColor, maxRecursion - 1) * 0.6;
                            color.w() = 1;
                        }
                    }
                }

                return color;
            }
        } // Raytracing
    } // Rendering
} // UltRenderer