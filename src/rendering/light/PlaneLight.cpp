//
// Created by ultcrt on 24-3-4.
//

#include "rendering/light/PlaneLight.h"
#include "data/IntersectionInfo.h"
#include "data/Ray.h"
#include "utils/Random.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            Data::IntersectionInfo PlaneLight::intersect(const Data::Ray &ray, double eps) {
                // Local normal is always Y axis, and local origin is always (0, 0, 0)
                const auto normal = Math::Vector3D::Y();
                const auto origin = Math::Vector3D(0, 0, 0);

                const auto transInv = transformMatrix.inverse();
                const auto localRayDir = (transInv * ray.direction.toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
                const auto localRayOri = (transInv * ray.origin.toHomogeneousCoordinates(1)).toCartesianCoordinates();

                const double h = (localRayOri - origin).dot(normal);
                const double cos = -localRayDir.dot(normal);
                const double l = h / cos;

                Data::IntersectionInfo res;
                if (l >= 0) {
                    const Math::Vector3D intersectedPoint = localRayOri + localRayDir * l;

                    if (intersectedPoint.x() <= width / 2. && intersectedPoint.x() >= -width / 2.) {
                        if (intersectedPoint.z() <= height / 2. && intersectedPoint.z() >= -height / 2.) {
                            res.isIntersected = true;
                            res.length = l;
                            // TODO: Need more accurate uv
                            res.uv = {0, 0, 0};
                            res.normal = (transformMatrix * normal.toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
                            res.pNode = this;
                        }
                    }
                }
                return res;
            }

            PlaneLight::PlaneLight(double w, double h, const Math::Vector3D& intensity): IAreaLight(intensity), width(w), height(h) {}

            Math::Vector3D PlaneLight::sample(std::size_t n, const Math::Vector3D &p, const Material::CommonMaterial& target, const Math::Vector3D& v, const Math::Vector3D& uv, const Math::Vector3D& normal, const Scene& scene, double eps) const {
                Math::Vector3D radiance = {0, 0, 0};

                for (std::size_t idx = 0; idx < n; idx++) {
                    const double w = Utils::Random::Range(-1, 1) * width / 2;
                    const double h = Utils::Random::Range(-1, 1) * height / 2;
                    const Math::Vector3D sampledPoint = {w, eps, h};
                    const Math::Vector3D worldSampledPoint = (transformMatrix * sampledPoint.toHomogeneousCoordinates(1)).toCartesianCoordinates();
                    const Math::Vector3D worldNormal = (transformMatrix * Math::Vector3D::Y().toHomogeneousCoordinates(0)).toCartesianCoordinates().normalized();
                    const Math::Vector3D lightDir = (p - worldSampledPoint).normalized();
                    const double cosTheta = -normal.dot(lightDir);
                    const double cosThetaPrime = worldNormal.dot(lightDir);

                    Math::Vector3D i = {0., 0., 0.};
                    // Visibility
                    const Data::Ray lightRay(worldSampledPoint, lightDir);
                    const auto info = lightRay.intersect(scene);
                    if (info.isIntersected) {
                        const auto intersectedPoint = worldSampledPoint + lightDir * info.length;
                        if ((p - intersectedPoint).norm() < eps) {
                            i = intensity();
                        }
                    }

                    radiance += i.componentWiseProduct(target.evalBSDF(uv, normal, v, lightDir)) * std::max(cosTheta, 0.) * cosThetaPrime / (p - worldSampledPoint).norm2() / (1. / (width * height)) / static_cast<double>(n);
                }

                return radiance;
            }
        } // Light
    } // Rendering
} // UltRenderer