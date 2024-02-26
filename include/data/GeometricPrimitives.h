//
// Created by ultcrt on 24-2-21.
//

#ifndef ULTRENDERER_GEOMETRICPRIMITIVES_H
#define ULTRENDERER_GEOMETRICPRIMITIVES_H

#include "hierarchy/IntersectableNode.h"

namespace UltRenderer {
    namespace Data {
        namespace GeometricPrimitives {
            class Sphere: public Hierarchy::IntersectableNode {
            private:
                double _radius = 1;

            public:
                Sphere(const Math::Vector3D& center, double radius);

                Utils::MatrixProxy<double, 3, 1> center();

                Data::IntersectionInfo intersect(const Ray &ray, double eps) override;
            };
        } // GeometricPrimitives
    } // UltRenderer
} // Data


#endif //ULTRENDERER_GEOMETRICPRIMITIVES_H
