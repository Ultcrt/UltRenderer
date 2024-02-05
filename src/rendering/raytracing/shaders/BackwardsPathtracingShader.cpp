//
// Created by ultcrt on 24-2-4.
//

#include "rendering/raytracing/shaders/BackwardsPathtracingShader.h"
#include "utils/Random.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            namespace Shaders {
                const Math::Matrix3D BackwardsPathtracingShader::_transformY2Z = {
                        1, 0, 0,
                        0, 0, -1,
                        0, 1, 0
                };

                Data::Color<Data::ColorFormat::RGBA>
                BackwardsPathtracingShader::operator()(const Math::Vector3D &pixelCenterCamera, double pixelWidthCamera,
                                                       double pixelHeightCamera, const Math::Transform3D &transform,
                                                       const Math::Vector3D &cameraOriginWorld, const Scene *pScene) const {

                }

                Data::Color<Data::ColorFormat::RGBA> BackwardsPathtracingShader::Cast(
                        const Math::Ray& ray,
                        const Scene* pScene) const {
                    // When russian roulette failed, should return zero ray
                    Data::Color<Data::ColorFormat::RGBA> color = {0, 0, 0, 0};

                    // Russian Roulette
                    if (Utils::Random::Range() > dropout) {
                        color = backgroundColor;
                        const auto info = ray.intersect(pScene->meshes());

                        if (info.isIntersected) {
                            // TODO: Codes can be reused here
                            const auto& mesh = *info.pMesh;
                            const auto& mat = *mesh.pMaterial;
                            const auto& baryCentricCoord = info.barycentricCoord;
                            const auto& triangle = mesh.triangles[info.triangleIdx];

                            const auto& uv0 = mesh.vertexTextures[triangle[0]];
                            const auto& uv1 = mesh.vertexTextures[triangle[1]];
                            const auto& uv2 = mesh.vertexTextures[triangle[2]];

                            const Math::Vector3D uv = uv0 * baryCentricCoord[0] + uv1 * baryCentricCoord[1] + uv2 * baryCentricCoord[2];

                            const auto& triangleTangent = (mesh.transformMatrix * mesh.triangleTangents[info.triangleIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates();
                            const auto& triangleNormal = (mesh.transformMatrix * mesh.triangleNormals[info.triangleIdx].toHomogeneousCoordinates(0)).toCartesianCoordinates();

                            const auto tbn = Math::Geometry::GetTBN(triangleTangent, triangleNormal);

                            Math::Vector3D normal = mesh.vertexNormals[0] * baryCentricCoord[0] + mesh.vertexNormals[1] * baryCentricCoord[1] + mesh.vertexNormals[2] * baryCentricCoord[2];
                            if (mat.pNormalMap) {
                                normal = (*mat.pNormalMap).get<Data::ColorFormat::RGB>(uv[0], uv[1]) * 2. - Math::Vector3D{1, 1, 1};
                                if (mat.normalMapType == Data::NormalMapType::DARBOUX) {
                                    normal = (tbn * normal).normalized();
                                }
                                else {
                                    normal = (mesh.transformMatrix * normal.toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
                                }
                            }

                            // Above and below is relative to normal
                            Math::Vector3D intersectedPointAbove = ray.origin + ray.direction * (info.length - eps);
                            Math::Vector3D intersectedPointBelow = ray.origin + ray.direction * (info.length + eps);
                            // Swap above and below when ray intersected at the back side of surface
                            if (normal.dot(ray.direction) > 0) std::swap(intersectedPointAbove, intersectedPointBelow);

                            // sampledDirection is facing at y-axis
                            auto sampledDirections = Utils::Random::SampleFromUnitSemiSphere(numBouncedRays);

                            // Transform sampledDirection into world space
                            for (auto& dir: sampledDirections) {
                                dir = (tbn * _transformY2Z * dir).normalized();
                            }

                            // Monte Carlo integration
                            Math::Vector4D outRadiance = {0, 0, 0, 0};
                            for (const auto& dir: sampledDirections) {
                            }

                            // Transform sampled direction into
                        }
                    }

                    return color;
                }
            } // Shaders
        } // Raytracing
    } // Rendering
} // UltRenderer