//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_TRANSFORM_H
#define ULTRENDERER_TRANSFORM_H

#include "Matrix.h"

namespace UltRenderer {
    namespace Math {
        class Transform3D: public Matrix4D {
        public:
            Transform3D();

            explicit Transform3D(const Vector3D& scaling, const Vector3D& position, const Vector3D& rotation);
        };

    } // Math
} // UltRenderer

#endif //ULTRENDERER_TRANSFORM_H