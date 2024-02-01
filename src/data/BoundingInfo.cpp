//
// Created by ultcrt on 24-1-22.
//

#include "data/BoundingInfo.h"

namespace UltRenderer {
    namespace Data {
        double BoundingInfo::radius() const {
            return (min - max).norm() / 2;
        }

        double BoundingInfo::width() const {
            return max.x() - min.x();
        }

        double BoundingInfo::height() const {
            return max.y() - min.y();
        }

        double BoundingInfo::depth() const {
            return max.z() - min.z();
        }

        Math::Vector3D BoundingInfo::origin() const {
            return (max + min) / 2;
        }

        BoundingInfo::BoundingInfo(const Math::Vector3D &min, const Math::Vector3D &max): min(min), max(max) {}
    } // Data
} // UltRenderer