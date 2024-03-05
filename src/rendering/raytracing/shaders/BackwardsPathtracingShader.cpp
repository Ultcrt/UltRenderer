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

                        const Data::Ray pixelRay(cameraOriginWorld, (offsetCenterWorld - cameraOriginWorld).normalized());

                        auto res = Cast(pixelRay, pScene, 0);

                        sum += res;
                    }

                    return sum / static_cast<double>(samplesPerPixel);
                }

                Data::Color<Data::ColorFormat::RGBA> BackwardsPathtracingShader::Cast(
                        const Data::Ray& ray,
                        const Scene* pScene,
                        std::size_t depth) const {
                    // If not intersect with anything, then return background color
                    Data::Color<Data::ColorFormat::RGBA> color = backgroundColor;

                    const auto info = ray.intersect(*pScene);

                    if (info.isIntersected) {
                        // Check if direct illumination
                        if (depth > 0) {
                            // If is not direct illumination, check shading point is on area lights or not
                            for (const auto& pLight: pScene->areaLights()) {
                                if (pLight.get() == info.pNode) {
                                    // If on area lights, return 0 radiance, since current ray is already considered in direct illumination
                                    return {0, 0, 0, 1};
                                }
                            }
                        }

                        // TODO: Codes can be reused here
                        const auto& node = *info.pNode;
                        const auto& mat = *node.pMaterial;

                        const Math::Vector3D uv = info.uv;

                        Math::Vector3D normal = info.normal;

                        // Above and below is relative to ray origin
                        Math::Vector3D intersectedPointCloser = ray.origin + ray.direction * (info.length - eps);
                        Math::Vector3D intersectedPointFurther = ray.origin + ray.direction * (info.length + eps);

                        Math::Vector4D directIr = {0, 0, 0, 1};
                        Math::Vector4D indirectIr = {0, 0, 0, 1};

                        // Direct illumination using direct/explicit light sampling
                        // Deal with non area light
                        for (const auto& pLight: pScene->nonAreaLights()) {
                            const Math::Vector3D dir = -pLight->getDirectionAt(intersectedPointCloser);
                            const Math::Vector3D ori = intersectedPointCloser;
                            const Data::Ray reversedLightRay = {ori, dir};
                            // TODO: Only implement Lambertian BRDF here
                            Math::Vector3D bsdf = mat.evalBSDF(uv, normal, -ray.direction, -dir);
                            // Shadow check
                            if (!reversedLightRay.intersect(*pScene).isIntersected) {
                                const double cos = std::abs(normal.dot(-dir));
                                // distance^2 is needed because intensity in non area light is irradiance instead of radiance
                                directIr += pLight->intensity * bsdf.toHomogeneousCoordinates(1) * cos / std::pow(pLight->getDistanceTo(intersectedPointCloser), 2.);
                            }
                        }
                        // Deal with area light
                        for (const auto& pLight: pScene->areaLights()) {
                            directIr += pLight->sample(numBouncedRays, intersectedPointCloser, mat, -ray.direction, uv, normal, *pScene).toHomogeneousCoordinates(1);
                        }

                        // Indirect illumination using russian roulette
                        if (Utils::Random::Range() > dropout) {
                            // sampledDirection is facing at z-axis
                            auto [sampledDirections, bsdfs] = mat.sampleBSDF(uv, numBouncedRays, normal, -ray.direction);

                            // Monte Carlo integration
                            for (std::size_t sampleIdx = 0; sampleIdx < numBouncedRays; sampleIdx++) {
                                const auto dir = sampledDirections[sampleIdx];
                                const auto bsdf = bsdfs[sampleIdx];

                                const Math::Vector3D ori = normal.dot(dir) >= 0 ? intersectedPointCloser : intersectedPointFurther;

                                const auto sampledRay = Data::Ray(ori, dir);

                                // TODO: Check sampled ray is not intersected with emitting object (because all emitting object should already be considered in direct illumination)
                                indirectIr += Cast(sampledRay, pScene, depth + 1).componentWiseProduct(bsdf.toHomogeneousCoordinates(1)) / (1 - dropout);
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