//
// Created by ultcrt on 24-1-30.
//

#ifndef ULTRENDERER_MATERIAL_H
#define ULTRENDERER_MATERIAL_H

#include <memory>
#include "data/Image.h"

namespace UltRenderer {
    namespace Rendering {
        class Material {
        public:
            std::shared_ptr<Data::Image> pTexture;
            std::shared_ptr<Data::Image> pSpecularMap;
            std::shared_ptr<Data::Image> pGlowMap;
            std::shared_ptr<Data::Image> pNormalMap;
            Data::NormalMapType normalMapType;

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
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_MATERIAL_H
