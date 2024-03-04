//
// Created by ultcrt on 24-3-4.
//

#ifndef ULTRENDERER_INONAREALIGHT_H
#define ULTRENDERER_INONAREALIGHT_H

#include "hierarchy/Node.h"
#include "math/Matrix.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Light {
            class INonAreaLight: public Hierarchy::Node {
            public:
                double intensity = 1;

                explicit INonAreaLight(double i);

                virtual Math::Vector3D getDirectionAt(const Math::Vector3D& point) const = 0;
                virtual double getDistanceTo(const Math::Vector3D& point) const = 0;
            };
        } // Light
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_INONAREALIGHT_H
