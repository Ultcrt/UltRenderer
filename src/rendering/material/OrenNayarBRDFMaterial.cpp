//
// Created by ultcrt on 24-3-3.
//

#include <iostream>
#include "rendering/material/OrenNayarBRDFMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            Math::Vector3D OrenNayarBRDFMaterial::getBSDF(const Math::Vector3D &uv, const Math::Vector3D &normal,
                                                          const Math::Vector3D &view,
                                                          const Math::Vector3D &light) const {
                double thetaV = std::acos(view.dot(normal));
                double thetaL = std::acos(-light.dot(normal));
                double alpha = std::max(thetaL, thetaV);
                double beta = std::min(thetaL, thetaV);
                Math::Vector3D v = view - view.dot(normal) * normal;
                Math::Vector3D l = -light + light.dot(normal) * normal;

                Math::Vector3D color;
                if (pTexture->type() == Data::ColorFormat::GRAY) {
                    color = pTexture->get<Data::ColorFormat::GRAY>(uv[0], uv[1]).to<Data::ColorFormat::RGB>();
                }
                else {
                    color = pTexture->get<Data::ColorFormat::RGB>(uv[0], uv[1]);
                }

                double r2 = roughness * roughness;
                double a = 1. - 0.5 * r2 / (r2 + 0.33);
                double b = 0.45 * r2 / (r2 + 0.09);
                double theta = std::acos(v.dot(l));

                return color / M_PI * normal.dot(-light) * (a + b * std::max(0., std::cos(theta)) * std::sin(alpha) * std::tan(beta));
            }
        } // Material
    } // Rendering
} // UltRenderer