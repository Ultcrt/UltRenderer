//
// Created by ultcrt on 24-2-21.
//

#include <iostream>
#include "data/GeometricPrimitives.h"
#include "data/Ray.h"
#include "utils/Algorithm.h"

namespace UltRenderer {
    namespace Data {
        namespace GeometricPrimitives {
            Sphere::Sphere(double radius): _radius(radius) {}

            Utils::MatrixProxy<double, 3, 1> Sphere::center() {
                std::array<double*, 3> components{};

                components[0] = &transformMatrix(0, 3);
                components[1] = &transformMatrix(1, 3);
                components[2] = &transformMatrix(2, 3);

                return Utils::MatrixProxy<double, 3, 1>(components);
            }

            Data::IntersectionInfo Sphere::intersect(const Ray &ray, double eps) {
                IntersectionInfo res;

                const double r2 = _radius * _radius + eps;
                const Math::Vector3D v = static_cast<Math::Vector3D>(center()) - ray.origin;
                const double l = v.dot(ray.direction);
                const double d2 = v.norm2() - l * l;

                // Not intersected
                if (d2 > r2) {
                    return res;
                }
                else {
                    const double di = std::sqrt(r2 - d2);

                    double rayLen = l + (l >= di ? -di : di);
                    if (rayLen < 0) return res;
                    else {
                        const Math::Vector3D intersectedPoint = ray.origin + ray.direction * rayLen;

                        res.length = rayLen;
                        res.isIntersected = true;
                        res.pNode = this;
                        // TODO: Constant UV for now
                        res.uv = {0.5, 0.5};
                        res.normal = (intersectedPoint - center()).normalized();

                        return res;
                    }
                }
            }

            Cube::Cube(double w, double h, double d): _w(w), _h(h), _d(d) {}

            Data::IntersectionInfo Cube::intersect(const Ray &ray, double eps) {
                Math::Vector3D half = {_w/2, _h/2, _d/2};

                IntersectionInfo res;

                Data::BoundingInfo bInfo(-half, half);

                // Transform ray to Cube coordinates
                auto invTrans = transformMatrix.inverse();
                Math::Vector3D localOri = (invTrans * ray.origin.toHomogeneousCoordinates(1)).toCartesianCoordinates();
                Math::Vector3D localDir = (invTrans * ray.direction.toHomogeneousCoordinates(0)).toCartesianCoordinates();
                auto localRay = Ray(localOri, localDir);

                double enter, exit;
                if (localRay.isIntersected(bInfo, enter, exit)) {
                    const Math::Vector3D localIntersectedPoint = localOri + localDir * enter;

                    const std::size_t minAxis = Utils::ArgMin(std::vector<double>{
                        std::abs(localIntersectedPoint.x() - half.x()),
                        std::abs(localIntersectedPoint.y() - half.y()),
                        std::abs(localIntersectedPoint.z() - half.z()),
                        std::abs(localIntersectedPoint.x() + half.x()),
                        std::abs(localIntersectedPoint.y() + half.y()),
                        std::abs(localIntersectedPoint.z() + half.z()),
                    });

                    switch (minAxis) {
                        case 0:
                            res.normal = Math::Vector3D::X();
                            break;
                        case 1:
                            res.normal = Math::Vector3D::Y();
                            break;
                        case 2:
                            res.normal = Math::Vector3D::Z();
                            break;
                        case 3:
                            res.normal = -Math::Vector3D::X();
                            break;
                        case 4:
                            res.normal = -Math::Vector3D::Y();
                            break;
                        case 5:
                            res.normal = -Math::Vector3D::Z();
                            break;
                        default:
                            throw std::runtime_error(std::format("Unexpected index of axis {}", minAxis));
                    }

                    res.normal = (transformMatrix * res.normal.toHomogeneousCoordinates(0)).toCartesianCoordinates();

                    res.length = enter;
                    res.isIntersected = true;
                    res.pNode = this;
                    // TODO: Constant UV for now
                    res.uv = {0.5, 0.5};

                    return res;
                }

                return res;
            }
        } // GeometricPrimitives
    } // UltRenderer
} // Data
