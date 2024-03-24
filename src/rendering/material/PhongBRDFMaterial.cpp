//
// Created by ultcrt on 24-3-3.
//

#include <iostream>
#include "rendering/material/PhongBRDFMaterial.h"
#include "math/Geometry.h"
#include "utils/Random.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            Math::Vector3D PhongBRDFMaterial::evalBSDF(const Math::Vector3D &uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const {
                const Math::Vector3D& reflect = Math::Geometry::ComputeReflectionDirection(normal, light);

                auto [s, finalSpecularColor] = getSpecularInfo(uv);

                return finalSpecularColor * std::pow(std::max(reflect.dot(view), 0.), s);
            }

            std::pair<std::vector<Math::Vector3D>, std::vector<Math::Vector3D>> PhongBRDFMaterial::sampleBSDF(const Math::Vector3D &uv, std::size_t n, const Math::Vector3D &normal,
                                                         const Math::Vector3D &view) const {
                // TODO: Should be the same with conversion vector direction
                const Math::Vector3D& idealReflect = Math::Geometry::ComputeReflectionDirection(normal, -view);

                auto [s, finalSpecularColor] = getSpecularInfo(uv);

                // sampledDirection is facing at z-axis
                auto sampledDirections = Utils::Random::SampleWithPhongBRDF(n, s);

                // Transform sampledDirection into world space (sampling with ideal reflection)
                Math::Vector3D tangent = idealReflect.cross((idealReflect == Math::Vector3D::X() || idealReflect == -Math::Vector3D::X()) ? Math::Vector3D::Y() : Math::Vector3D::X()).normalized();
                const auto tbn = Math::Geometry::GetTBN(tangent, idealReflect);
                for (auto& dir: sampledDirections) {
                    dir = (tbn * dir).normalized();
                }

                // Monte Carlo integration
                std::vector<Math::Vector3D> res;
                for (const auto& dir: sampledDirections) {
                    const double cos = std::abs(normal.dot(dir));

                    // TODO: Check sampled ray is not intersected with emitting object (because all emitting object should already be considered in direct illumination)
                    // bsdf = finalSpecularColor * std::pow(cosTheta, s);
                    // pdf = (s + 1) * std::pow(cosTheta, s) * std::sqrt(1 - cosTheta * cosTheta) / (2 * M_PI)
                    // Reduced to (s + 2.) / (s + 1.) / std::sqrt(1 - cosTheta * cosTheta)
                    // From: https://graphics.cs.kuleuven.be/publications/Phong/Phong_paper.ps
                    // std::min is used to guarantee energy conservation
                    res.emplace_back(std::min((s + 2.) / (s + 1.) * cos, 1.) / static_cast<double>(n));
                }

                return {sampledDirections, res};
            }
        } // Material
    } // Rendering
} // UltRenderer