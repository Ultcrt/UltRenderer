//
// Created by ultcrt on 24-3-4.
//

#ifndef ULTRENDERER_PLANELIGHT_H
#define ULTRENDERER_PLANELIGHT_H

#include "IAreaLight.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            class PlaneLight: public IAreaLight {
            public:
                double width;
                double height;

                PlaneLight(double w, double h, double intensity = 1);

                [[nodiscard]] Data::IntersectionInfo intersect(const Data::Ray &ray, double eps) override;
                [[nodiscard]] Math::Vector3D sample(std::size_t n, const Math::Vector3D &p, const Material::CommonMaterial& target, const Math::Vector3D& v, const Math::Vector3D& uv, const Math::Vector3D& normal) const override;
            };
        } // Light
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_PLANELIGHT_H
