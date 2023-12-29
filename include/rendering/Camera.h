//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_CAMERA_H
#define ULTRENDERER_CAMERA_H

#include "math/Matrix.h"
#include "math/Transform.h"

namespace UltRenderer {
    namespace Rendering {
        enum class FOVType {
            H, V, D
        };

        class Camera {
        private:
            double _width;
            double _height;

            double _zMin = 0.1;
            double _zMax = 10;
        public:
            Math::Transform3D viewMatrix;
            Math::Transform3D projectionMatrix;

            static Math::Vector2D ConvertFOVToSize(FOVType fovType, double len, bool isWidth = true);

            Camera(double width, FOVType fovType, double zMin=0.1, double zMax=0.1);
            Camera(double width, double height, double zMin=0.1, double zMax=0.1);

            double width() const;
            double height() const;
            double zMin() const;
            double zMax() const;

            void setWidth(double width);
            void setHeight(double height);
            void setZMin(double zMin);
            void setZMax(double zMax);

            void render(std::size_t width, std::size_t height);
        };

    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
