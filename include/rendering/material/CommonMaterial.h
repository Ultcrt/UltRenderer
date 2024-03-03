//
// Created by ultcrt on 24-1-30.
//

#ifndef ULTRENDERER_COMMONMATERIAL_H
#define ULTRENDERER_COMMONMATERIAL_H

#include <memory>
#include "data/Image.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Material {
            class CommonMaterial {
            public:
                std::shared_ptr<Data::Image> pTexture;
                std::shared_ptr<Data::Image> pSpecularMap;
                std::shared_ptr<Data::Image> pGlowMap;
                std::shared_ptr<Data::Image> pNormalMap;
                Data::NormalMapType normalMapType;

                double shininess = 1;
                Math::Vector3D specularColor = {1, 1, 1};
                Math::Vector3D ambientColor = {0.2, 0.1, 0};

                double diffuseCoefficient = 0.5;
                double specularCoefficient = 0.4;
                double ambientCoefficient = 0.1;
                double reflectionCoefficient = 0;
                double refractionCoefficient = 0;

                double refractiveIndex = 1;

                double shadowIntensity = 0.3;
                double glowIntensity = 1.0;

                [[nodiscard]] virtual Math::Vector3D evalBSDF(const Math::Vector3D& uv, const Math::Vector3D& normal, const Math::Vector3D& view, const Math::Vector3D& light) const;

                [[nodiscard]] virtual std::pair<std::vector<Math::Vector3D>, std::vector<Math::Vector3D>> sampleBSDF(const Math::Vector3D& uv, std::size_t n, const Math::Vector3D& normal, const Math::Vector3D& view) const;

                [[nodiscard]] Math::Vector3D getDiffuseColor(const Math::Vector3D& uv) const;

                [[nodiscard]] std::pair<double, Math::Vector3D> getSpecularInfo(const Math::Vector3D& uv) const;
            };
        }
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_COMMONMATERIAL_H
