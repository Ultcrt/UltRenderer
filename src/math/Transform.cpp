//
// Created by ultcrt on 23-12-26.
//

#include "math/Transform.h"

namespace UltRenderer {
    namespace Math {
        Transform3D::Transform3D(const Vector3D &scaling, const Vector3D &position, const Vector3D &rotation) {

        }

        Transform3D::Transform3D(): Matrix4D(Matrix4D::Identity()) {}
    } // Math
} // UltRenderer