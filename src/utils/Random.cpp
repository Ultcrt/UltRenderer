//
// Created by ultcrt on 24-2-5.
//

#include "utils/Random.h"

namespace UltRenderer {
    namespace Utils {
        std::size_t Random::_seed = std::mt19937::default_seed;
        std::shared_ptr<std::mt19937> Random::_pEngine = std::make_shared<std::mt19937>(Random::_seed);

        double Random::Range(double l, double r, std::size_t* pSeed) {
            assert(l < r);

            std::shared_ptr<std::mt19937> pEngine;

            if (pSeed) {
                pEngine = std::make_shared<std::mt19937>(*pSeed);
            }
            else {
                pEngine = _pEngine;
            }

            // Tips: Use std::nextafter to random pick from [0, 1] instead of [0, 1)
            std::uniform_real_distribution<double> dist(l, r);

            return dist(*pEngine);
        }


        std::vector<Math::Vector3D> Random::SampleFromUnitSphere(std::size_t n, std::size_t* pSeed) {
            // Uniformly picking from sphere, from: https://mathworld.wolfram.com/SpherePointPicking.html
            std::vector<Math::Vector3D> res;
            for (std::size_t idx = 0; idx < n; idx++) {
                const double u = Range(0, 1, pSeed);
                const double v = Range(0, 1, pSeed);
                const double theta = 2 * M_PI * u;
                const double phi = std::acos(2 * v - 1);

                const double x = std::cos(theta) * std::sin(phi);
                const double y = std::sin(theta) * std::sin(phi);
                const double z = std::cos(phi);

                res.emplace_back(x, y, z);
            }

            return res;
        }

        /**
         * Sampling uniformly from semi-sphere. Semi-sphere is facing z-axis
         * @param n Sampling number
         * @param pSeed Random seed pointer
         * @return
         */
        std::vector<Math::Vector3D> Random::SampleFromUnitSemiSphere(std::size_t n, std::size_t* pSeed) {
            std::vector<Math::Vector3D> res;
            for (std::size_t idx = 0; idx < n; idx++) {
                const double u = Range(0, 1, pSeed);
                const double v = Range(0, 1, pSeed);
                const double theta = 2 * M_PI * u;
                const double phi = std::acos(v);

                const double x = std::cos(theta) * std::sin(phi);
                const double y = std::sin(theta) * std::sin(phi);
                const double z = std::cos(phi);

                res.emplace_back(x, y, z);
            }

            return res;
        }

        void Random::SetSeed(std::size_t seed) {
            Random::_seed = seed;
            Random::_pEngine = std::make_shared<std::mt19937>(seed);
        }

        std::vector<Math::Vector3D> Random::SampleWithPhongBRDF(std::size_t n, double shininess, std::size_t *pSeed) {
            // From https://www.mathematik.uni-marburg.de/%7Ethormae/lectures/graphics1/code/ImportanceSampling/importance_sampling_notes.pdf
            std::vector<Math::Vector3D> res;
            for (std::size_t idx = 0; idx < n; idx++) {
                const double u = Range(0, 1, pSeed);
                const double v = Range(0, 1, pSeed);
                const double theta = 2 * M_PI * u;
                const double phi = std::acos(std::pow((1. - v), 1. / (shininess + 1.)));

                const double x = std::cos(theta) * std::sin(phi);
                const double y = std::sin(theta) * std::sin(phi);
                const double z = std::cos(phi);

                res.emplace_back(x, y, z);
            }

            return res;
        }

        std::vector<Math::Vector3D>
        Random::SampleWithCosWeightedBRDF(std::size_t n, std::size_t *pSeed) {
            // From https://zhuanlan.zhihu.com/p/503163354
            std::vector<Math::Vector3D> res;
            for (std::size_t idx = 0; idx < n; idx++) {
                const double u = Range(0, 1, pSeed);
                const double v = Range(0, 1, pSeed);
                const double theta = 2 * M_PI * u;
                const double phi = std::acos(std::sqrt(1. - v));

                const double x = std::cos(theta) * std::sin(phi);
                const double y = std::sin(theta) * std::sin(phi);
                const double z = std::cos(phi);

                res.emplace_back(x, y, z);
            }

            return res;
        }
    } // Utils
} // UltRenderer