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

                        const Data::Ray pixelRay(cameraOriginWorld, (offsetCenterWorld - cameraOriginWorld).normalized());

                        auto res = Cast(pixelRay, pScene);

                        sum += res;
                    }

                    return sum / static_cast<double>(samplesPerPixel);
                }

                Data::Color<Data::ColorFormat::RGBA> BackwardsPathtracingShader::Cast(
                        const Data::Ray& ray,
                        const Scene* pScene) const {
                    // If not intersect with anything, then return background color
                    Data::Color<Data::ColorFormat::RGBA> color = backgroundColor;

                    const auto info = ray.intersect(*pScene);

                    if (info.isIntersected) {
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
                        for (const auto& pLight: pScene->lights()) {
                            // Directional light
                            const Math::Vector3D dir = -pLight->direction;
                            const Math::Vector3D ori = intersectedPointCloser;
                            const Data::Ray reversedLightRay = {ori, dir};
                            // TODO: Only implement Lambertian BRDF here
                            Math::Vector3D bsdf = mat.getBSDF(uv, normal, -ray.direction, pLight->direction);
                            // Shadow check
                            if (!reversedLightRay.intersect(*pScene).isIntersected) {
                                const double cos = std::abs(normal.dot(pLight->direction));
                                directIr += pLight->intensity * bsdf.toHomogeneousCoordinates(1) * cos;
                            }
                        }

                        // Indirect illumination using russian roulette
                        if (Utils::Random::Range() > dropout) {
                            // sampledDirection is facing at z-axis
                            auto sampledDirections = Utils::Random::SampleFromUnitSemiSphere(numBouncedRays);

                            // Transform sampledDirection into world space
                            Math::Vector3D tangent = normal.cross((normal == Math::Vector3D::X() || normal == -Math::Vector3D::X()) ? Math::Vector3D::Y() : Math::Vector3D::X()).normalized();
                            const auto tbn = Math::Geometry::GetTBN(tangent, normal);
                            for (auto& dir: sampledDirections) {
                                dir = (tbn * dir).normalized();
                            }

                            // Monte Carlo integration
                            for (const auto& dir: sampledDirections) {
                                const Math::Vector3D ori = normal.dot(dir) >= 0 ? intersectedPointCloser : intersectedPointFurther;
                                const double cos = std::abs(normal.dot(dir));

                                const auto sampledRay = Data::Ray(ori, dir);

                                Math::Vector3D bsdf = mat.getBSDF(uv, normal, -ray.direction, -dir);

                                // TODO: Check sampled ray is not intersected with emitting object (because all emitting object should already be considered in direct illumination)
                                indirectIr += Cast(sampledRay, pScene).componentWiseProduct(bsdf.toHomogeneousCoordinates(1)) * cos / _uniformSamplingPossibility / (1 - dropout) / static_cast<double>(numBouncedRays);
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