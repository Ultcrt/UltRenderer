//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_CAMERA_H
#define ULTRENDERER_CAMERA_H

#include "math/Matrix.h"

namespace UltRenderer {
    namespace Rendering {
        class Camera {
        private:
            std::size_t _width;
            std::size_t _height;

            double _zMin;
            double _zMax;
        public:
        };

    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
