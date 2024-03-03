//
// Created by ultcrt on 24-1-30.
//

#include "rendering/material/CommonMaterial.h"

namespace UltRenderer {
    namespace Rendering {
        Math::Vector3D Material::CommonMaterial::evalBSDF(const Math::Vector3D& uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const {
            throw std::runtime_error("Material not support Physic based rendering (i.e. not implement evalBSDF method)");
        }

        Math::Vector3D Material::CommonMaterial::getDiffuseColor(const Math::Vector3D &uv) const {
            Math::Vector3D color;
            if (pTexture->type() == Data::ColorFormat::GRAY) {
                color = pTexture->get<Data::ColorFormat::GRAY>(uv[0], uv[1]).to<Data::ColorFormat::RGB>();
            }
            else {
                color = pTexture->get<Data::ColorFormat::RGB>(uv[0], uv[1]);
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