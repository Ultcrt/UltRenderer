//
// Created by ultcrt on 24-1-22.
//

#ifndef ULTRENDERER_BOUNDINGINFO_H
#define ULTRENDERER_BOUNDINGINFO_H

#include "math/Matrix.h"

namespace UltRenderer {
    namespace Data {
        class BoundingInfo {
        public:
            Math::Vector3D min;
            Math::Vector3D max;

            [[nodiscard]] double radius() const;
            [[nodiscard]] double width() const;
            [[nodiscard]] double height() const;
            [[nodiscard]] double depth() const;
            [[nodiscard]] Math::Vector3D origin() const;

            BoundingInfo(const Math::Vector3D& min = {0, 0, 0}, const Math::Vector3D& max = {0, 0, 0});
        };
    } // Data
} // UltRenderer

#endif //ULTRENDERER_BOUNDINGINFO_H
