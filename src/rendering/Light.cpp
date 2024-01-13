//
// Created by ultcrt on 24-1-2.
//

#include "rendering/Light.h"

namespace UltRenderer {
    namespace Rendering {
        Light::Light(const Math::Vector3D &dir, double i): direction(dir), intensity(i) {}
    } // Rendering
} // UltRenderer