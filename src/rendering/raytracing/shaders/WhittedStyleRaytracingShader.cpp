//
// Created by ultcrt on 24-1-22.
//

#include <iostream>
#include "rendering/raytracing/shaders/WhittedStyleRaytracingShader.h"
#include "math/Geometry.h"
#include "rendering/Constants.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Raytracing {
            namespace Shaders {
                Data::Color<Data::ColorFormat::RGBA>
                WhittedStyleRaytracingShader::operator()(const Math::Vector3D &pixelCenterCamera, double pixelWidthCamera,
                                                         double pixelHeightCamera, const Math::Transform3D &transform,
                                                         const Math::Vector3D &cameraOriginWorld, const Scene *pScene) const {
                    const auto pixelCenterWorld = (transform *
                                                   pixelCenterCamera.toHomogeneousCoordinates(1)).toCartesianCoordinates();
                    const Data::Ray pixelRay(cameraOriginWorld, (pixelCenterWorld - cameraOriginWorld).normalized());
                    return Cast(pixelRay, pScene, 0);
                }

                Data::Color<Data::ColorFormat::RGBA>
                WhittedStyleRaytracingShader::Cast(const Data::Ray &ray, const Scene *pScene, std::size_t recursionLayer) const {
                    Math::Vector4D color = backgroundColor;

                    // Restrict max recursion number
                    if (recursionLayer < maxRecursion) {
                        const auto info = ray.intersect(*pScene);

                        if (info.isIntersected) {
                            const auto& node = *info.pNode;
                            const auto& mat = *node.pMaterial;

                            const auto& uv = info.uv;

                            Math::Vector3D normal = info.normal;

                            // Above and below is relative to normal
                            Math::Vector3D intersectedPointAbove = ray.origin + ray.direction * (info.length - eps);
                            Math::Vector3D intersectedPointBelow = ray.origin + ray.direction * (info.length + eps);
                            // Swap above and below when ray intersected at the back side of surface
                            if (normal.dot(ray.direction) > 0) std::swap(intersectedPointAbove, intersectedPointBelow);

                            // Diffuse texture
                            Math::Vector3D diffuseRGB;
                            if (mat.pTexture->type() == Data::ColorFormat::GRAY) {
                                diffuseRGB = mat.pTexture->get<Data::ColorFormat::GRAY>(uv[0], uv[1]).to<Data::ColorFormat::RGB>();
                            }
                            else {
                                diffuseRGB = mat.pTexture->get<Data::ColorFormat::RGB>(uv[0], uv[1]);
                            }

                            // Specular texture
                            // TODO: Code can be reused here
                            double brightness = mat.pSpecularMap ? (*mat.pSpecularMap).get<Data::ColorFormat::GRAY>(uv[0], uv[1])[0] * static_cast<double>(std::numeric_limits<uint8_t>::max()) : 0;
                            Data::Color<Data::ColorFormat::RGB> finalSpecularColor = {1, 1, 1};
                            if (mat.pSpecularMap && mat.pSpecularMap->type() == Data::ColorFormat::RGB) {
                                finalSpecularColor = (*mat.pSpecularMap).get<Data::ColorFormat::RGB>(uv[0], uv[1]);
                                brightness = finalSpecularColor.to<Data::ColorFormat::GRAY>()[0] * static_cast<double>(std::numeric_limits<uint8_t>::max());         // uint8_t is the correct form of data, need convert
                            }

                            // Glow texture
                            Math::Vector3D glowColor = mat.pGlowMap ? static_cast<Math::Vector3D>((*mat.pSpecularMap).get<Data::ColorFormat::RGB>(uv[0], uv[1])) : Math::Vector3D{0, 0, 0};

                            // Iterate all light to get diffuse and specular intensity
                            double diffuseIntensity = 0;
                            double specularIntensity = 0;
                            for (const auto& pLight: pScene->lights()) {
                                // Make sure the ray will never be occluded by intersected surface
                                const Math::Vector3D lightRayDirection = -pLight->direction;
                                const Math::Vector3D lightRayOrigin = normal.dot(lightRayDirection) >= 0 ? intersectedPointAbove : intersectedPointBelow;
                                Data::Ray lightRay(lightRayOrigin, lightRayDirection);

                                const auto lightIntersectionInfo = lightRay.intersect(*pScene);

                                // Only do shading when light is not occlude
                                if (!lightIntersectionInfo.isIntersected) {
                                    // Blinn-Phong reflection model
                                    const auto& lightVec = pLight->direction * pLight->intensity;

                                    // Diffuse
                                    diffuseIntensity += normal.dot(-lightVec);

                                    // Specular
                                    Math::Vector3D viewDir = -ray.direction;     // View direction is the ray direction
                                    Math::Vector3D halfVec = ((viewDir + -lightVec) / 2).normalized();
                                    specularIntensity += std::pow(std::max(halfVec.dot(normal), 0.), brightness);
                                }
                            }

                            Math::Vector3D reflectionColor;
                            Math::Vector3D refractionColor;

                            double inRefractiveIndex = Rendering::Constants::AirRefractiveIndex;
                            double outRefractiveIndex = mat.refractiveIndex;

                            // Swap refractive index when ray is coming from the inside of mesh
                            // TODO: Assume mesh is enclosed here
                            if (normal.dot(ray.direction) > 0) std::swap(inRefractiveIndex, outRefractiveIndex);

                            if (mat.reflectionCoefficient > 0) {
                                const auto& reflectionDirection = Math::Geometry::ComputeReflectionDirection(normal, ray.direction);
                                const auto& reflectionOrigin = normal.dot(reflectionDirection) >= 0 ? intersectedPointAbove : intersectedPointBelow;
                                const Data::Ray reflectionRay(reflectionOrigin, reflectionDirection);
                                reflectionColor = Cast(reflectionRay, pScene, recursionLayer + 1).to<Data::ColorFormat::RGB>();
                            }

                            if (mat.refractionCoefficient > 0) {
                                const auto& refractionDirection = Math::Geometry::ComputeRefractionDirection(normal, ray.direction, inRefractiveIndex / outRefractiveIndex);
                                const auto& refractionOrigin = normal.dot(refractionDirection) >= 0 ? intersectedPointAbove : intersectedPointBelow;
                                const Data::Ray refractionRay(refractionOrigin, refractionDirection);
                                refractionColor = Cast(refractionRay, pScene, recursionLayer + 1).to<Data::ColorFormat::RGB>();
                            }

                            double kr = Math::Geometry::ComputeFresnel(normal, ray.direction, inRefractiveIndex, outRefractiveIndex);

                            color = (
                                    mat.diffuseCoefficient * diffuseIntensity * diffuseRGB +
                                    mat.specularCoefficient * diffuseIntensity * finalSpecularColor +
                                    mat.ambientCoefficient * mat.ambientColor +
                                    mat.glowIntensity * glowColor +
                                    mat.reflectionCoefficient * reflectionColor * kr +
                                    mat.refractionCoefficient * refractionColor * (1 - kr)
                            ).toHomogeneousCoordinates(1);
                        }
                    }

                    return color;
                }
            } // Shaders
        } // Raytracing
    } // Rendering
} // UltRenderer