//
// Created by ultcrt on 24-2-4.
//

#include <iostream>
#include "rendering/raytracing/shaders/BackwardsPathtracingShader.h"
#include "utils/Random.h"
#include "math/Geometry.h"
#include "rendering/BSDF.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            namespace Shaders {
                const Math::Matrix3D BackwardsPathtracingShader::_transformY2Z = {
                        1, 0, 0,
                        0, 0, -1,
                        0, 1, 0
                };

                const double BackwardsPathtracingShader::_uniformSamplingPossibility = 1 / (2 * M_PI);

                Data::Color<Data::ColorFormat::RGBA>
                BackwardsPathtracingShader::operator()(const Math::Vector3D &pixelCenterCamera, double pixelWidthCamera,
                                                       double pixelHeightCamera, const Math::Transform3D &transform,
                                                       const Math::Vector3D &cameraOriginWorld, const Scene *pScene) const {
                    Math::Vector4D sum = {0, 0, 0, 0};

                    for (std::size_t idx = 0; idx < samplesPerPixel; idx++) {
                        const double widthOffset = Utils::Random::Range(-1, 1) * pixelWidthCamera / 2;
                        const double heightOffset = Utils::Random::Range(-1, 1) * pixelHeightCamera / 2;
                        const Math::Vector3D offsetCenterCamera = {
                            pixelCenterCamera.x() + widthOffset,
                            pixelCenterCamera.y() + heightOffset,
                            pixelCenterCamera.z()
                        };

                        const auto offsetCenterWorld = (transform *
                                offsetCenterCamera.toHomogeneousCoordinates(1)).toCartesianCoordinates();

                        const Math::Ray pixelRay(cameraOriginWorld, (offsetCenterWorld - cameraOriginWorld).normalized());

                        auto res = Cast(pixelRay, pScene);

                        sum += res;
                    }

                    return sum / static_cast<double>(samplesPerPixel);
                }

                Data::Color<Data::ColorFormat::RGBA> BackwardsPathtracingShader::Cast(
                        const Math::Ray& ray,
                        const Scene* pScene) const {
                    // If not intersect with anything, then return background color
                    Data::Color<Data::ColorFormat::RGBA> color = backgroundColor;

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

                        // Above and below is relative to ray origin
                        Math::Vector3D intersectedPointCloser = ray.origin + ray.direction * (info.length - eps);
                        Math::Vector3D intersectedPointFurther = ray.origin + ray.direction * (info.length + eps);

                        // TODO: Only implement Lambertian BRDF here
                        Math::Vector3D lambertianBRDF = BSDF::LambertianDiffuseBRDF(uv, mat);

                        Math::Vector4D directIr = {0, 0, 0, 1};
                        Math::Vector4D indirectIr = {0, 0, 0, 1};

                        // Direct illumination using direct/explicit light sampling
                        for (const auto& pLight: pScene->lights()) {
                            // Directional light
                            const Math::Vector3D dir = -pLight->direction;
                            const Math::Vector3D ori = intersectedPointCloser;
                            const Math::Ray reversedLightRay = {ori, dir};
                            // Shadow check
                            if (!reversedLightRay.intersect(pScene->meshes()).isIntersected) {
                                const double cos = std::abs(normal.dot(pLight->direction));
                                directIr += pLight->intensity * lambertianBRDF.toHomogeneousCoordinates(1) * cos;
                            }
                        }

                        // Indirect illumination using russian roulette
                        if (Utils::Random::Range() > dropout) {
                            // sampledDirection is facing at y-axis
                            auto sampledDirections = Utils::Random::SampleFromUnitSemiSphere(numBouncedRays);

                            // Reverse direction is ray is coming from inside
                            if (normal.dot(ray.direction) > 0) {
                                for (auto& dir: sampledDirections) {
                                    dir = -dir;
                                }
                            }

                            // Transform sampledDirection into world space
                            for (auto& dir: sampledDirections) {
                                dir = (tbn * _transformY2Z * dir).normalized();
                            }

                            // Monte Carlo integration
                            for (const auto& dir: sampledDirections) {
                                const Math::Vector3D ori = normal.dot(dir) >= 0 ? intersectedPointCloser : intersectedPointFurther;
                                const double cos = std::abs(normal.dot(dir));

                                const auto sampledRay = Math::Ray(ori, dir);

                                const auto sampledInfo = sampledRay.intersect(*pScene);

                                // TODO: Check sampled ray is not intersected with emitting object (because all emitting object should already be considered in direct illumination)
                                indirectIr += Cast(sampledRay, pScene).componentWiseProduct(lambertianBRDF.toHomogeneousCoordinates(1)) * cos / _uniformSamplingPossibility / (1 - dropout) / static_cast<double>(numBouncedRays);
                            }
                        }

                        color = directIr + indirectIr;
                    }

                    return color;
                }
            } // Shaders
        } // Raytracing
    } // Rendering
} // UltRenderer