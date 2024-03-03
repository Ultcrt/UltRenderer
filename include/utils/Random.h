//
// Created by ultcrt on 24-2-5.
//

#ifndef ULTRENDERER_RANDOM_H
#define ULTRENDERER_RANDOM_H

#include <random>
#include <utility>
#include <memory>
#include "math/Matrix.h"

namespace UltRenderer {
    namespace Utils {
        class Random {
        private:
            static std::size_t _seed;
            static std::shared_ptr<std::mt19937> _pEngine;

        public:
            static void SetSeed(std::size_t seed);

            static double Range(double l = 0, double r = 1, std::size_t* pSeed = nullptr);

            static std::vector<Math::Vector3D> SampleFromUnitSphere(std::size_t n, std::size_t* pSeed = nullptr);

            static std::vector<Math::Vector3D> SampleFromUnitSemiSphere(std::size_t n, std::size_t* pSeed = nullptr);

            static std::vector<Math::Vector3D> SampleWithPhongBRDF(std::size_t n, double shininess, std::size_t* pSeed = nullptr);
        };
    } // Utils
} // UltRenderer

#endif //ULTRENDERER_RANDOM_H
