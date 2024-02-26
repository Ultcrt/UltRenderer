//
// Created by ultcrt on 24-2-21.
//

#include "data/GeometricPrimitives.h"
#include "data/Ray.h"

namespace UltRenderer {
    namespace Data {
        namespace GeometricPrimitives {
            Sphere::Sphere(const Math::Vector3D &center, double radius): _radius(radius) {
                this->center() = center;
            }

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
        } // GeometricPrimitives
    } // UltRenderer
} // Data
