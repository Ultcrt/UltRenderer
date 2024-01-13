//
// Created by ultcrt on 24-1-2.
//

#include "rendering/Light.h"

namespace UltRenderer {
    namespace Rendering {
        Light::Light(const Math::Vector3D &pos, double intensity): position(pos), intensity(intensity) {}
    } // Rendering
} // UltRenderer