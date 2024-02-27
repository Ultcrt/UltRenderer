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
                explicit Sphere(double radius);

                Utils::MatrixProxy<double, 3, 1> center();

                Data::IntersectionInfo intersect(const Ray &ray, double eps) override;
            };

            class Cube: public Hierarchy::IntersectableNode {
            private:
                double _w;
                double _h;
                double _d;

            public:
                Cube(double w, double h, double d);

                Data::IntersectionInfo intersect(const Ray &ray, double eps) override;
            };
        } // GeometricPrimitives
    } // UltRenderer
} // Data


#endif //ULTRENDERER_GEOMETRICPRIMITIVES_H
