//
// Created by ultcrt on 24-3-3.
//

#include <iostream>
#include "rendering/material/PhongBRDFMaterial.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            Math::Vector3D PhongBRDFMaterial::getBSDF(const Math::Vector3D &uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const {
                const Math::Vector3D& reflect = Math::Geometry::ComputeReflectionDirection(normal, light);

                double brightness = pSpecularMap ? (*pSpecularMap).get<Data::ColorFormat::GRAY>(uv[0], uv[1])[0] * static_cast<double>(std::numeric_limits<uint8_t>::max()) : 1;
                Data::Color<Data::ColorFormat::RGB> finalSpecularColor = {1, 1, 1};
                if (pSpecularMap && pSpecularMap->type() == Data::ColorFormat::RGB) {
                    finalSpecularColor = (*pSpecularMap).get<Data::ColorFormat::RGB>(uv[0], uv[1]);
                    brightness = finalSpecularColor.to<Data::ColorFormat::GRAY>()[0] * static_cast<double>(std::numeric_limits<uint8_t>::max());         // uint8_t is the correct form of data, need convert
                }

                return finalSpecularColor * std::pow(std::max(reflect.dot(view), 0.), brightness);
            }
        } // Material
    } // Rendering
} // UltRenderer