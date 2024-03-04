//
// Created by ultcrt on 24-1-30.
//

#include "rendering/material/CommonMaterial.h"
#include "utils/Random.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Rendering {
        Math::Vector3D Material::CommonMaterial::evalBSDF(const Math::Vector3D& uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const {
            throw std::runtime_error("Material not support Physic based rendering (i.e. not implement evalBSDF method)");
        }

        // Default implementation: Uniform sampling
        std::pair<std::vector<Math::Vector3D>, std::vector<Math::Vector3D>>
        Material::CommonMaterial::sampleBSDF(const Math::Vector3D &uv, std::size_t n, const Math::Vector3D &normal,
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

        Math::Vector3D Material::CommonMaterial::getDiffuseColor(const Math::Vector3D &uv) const {
            Math::Vector3D color = diffuseColor;
            if (pTexture) {
                if (pTexture->type() == Data::ColorFormat::GRAY) {
                    color = pTexture->get<Data::ColorFormat::GRAY>(uv[0], uv[1]).to<Data::ColorFormat::RGB>();
                }
                else {
                    color = pTexture->get<Data::ColorFormat::RGB>(uv[0], uv[1]);
                }
            }

            return color;
        }

        std::pair<double, Math::Vector3D> Material::CommonMaterial::getSpecularInfo(const Math::Vector3D &uv) const {
            double s = pSpecularMap ? (*pSpecularMap).get<Data::ColorFormat::GRAY>(uv[0], uv[1])[0] * static_cast<double>(std::numeric_limits<uint8_t>::max()) : shininess;
            Data::Color<Data::ColorFormat::RGB> finalSpecularColor = {1, 1, 1};
            if (pSpecularMap && pSpecularMap->type() == Data::ColorFormat::RGB) {
                finalSpecularColor = (*pSpecularMap).get<Data::ColorFormat::RGB>(uv[0], uv[1]);
                s = finalSpecularColor.to<Data::ColorFormat::GRAY>()[0] * static_cast<double>(std::numeric_limits<uint8_t>::max());         // uint8_t is the correct form of data, need convert
            }

            return {s, finalSpecularColor};
        }
    } // Rendering
} // UltRenderer