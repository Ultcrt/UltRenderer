//
// Created by ultcrt on 24-3-4.
//

#include "rendering/material/BSDFMaterial.h"
#include "utils/Random.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            std::pair<std::vector<Math::Vector3D>, std::vector<Math::Vector3D>>
            BSDFMaterial::sampleBSDF(const Math::Vector3D &uv, std::size_t n, const Math::Vector3D &normal,
                                     const Math::Vector3D &view) const {
                // sampledDirection is facing at z-axis
                auto sampledDirections = Utils::Random::SampleFromUnitSemiSphere(n);

                // Transform sampledDirection into world space
                Math::Vector3D tangent = normal.cross((normal == Math::Vector3D::X() || normal == -Math::Vector3D::X()) ? Math::Vector3D::Y() : Math::Vector3D::X()).normalized();
                const auto tbn = Math::Geometry::GetTBN(tangent, normal);
                for (auto& dir: sampledDirections) {
                    dir = (tbn * dir).normalized();
                }

                // Monte Carlo integration
                std::vector<Math::Vector3D> res;
                for (const auto& dir: sampledDirections) {
                    const double cos = std::abs(normal.dot(dir));

                    Math::Vector3D bsdf = evalBSDF(uv, normal, view, -dir);

                    res.emplace_back(bsdf * cos / (1. / 2. * M_PI) / static_cast<double>(n));
                }

                return {sampledDirections, res};
            }
        } // Material
    } // Rendering
} // UltRenderer