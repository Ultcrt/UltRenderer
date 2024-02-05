//
// Created by ultcrt on 24-2-5.
//

#ifndef ULTRENDERER_RANDOM_H
#define ULTRENDERER_RANDOM_H

#include <random>
#include <utility>
#include "math/Matrix.h"

namespace UltRenderer {
    namespace Utils {
        namespace Random {
            std::vector<Math::Vector3D> SampleFromUnitSphere(std::size_t n, std::size_t seed = std::random_device()());

            std::vector<Math::Vector3D> SampleFromUnitSemiSphere(std::size_t n, std::size_t seed = std::random_device()());
        }
    } // Utils
} // UltRenderer

#endif //ULTRENDERER_RANDOM_H
