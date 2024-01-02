//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_CAMERA_H
#define ULTRENDERER_CAMERA_H

#include "math/Matrix.h"
#include "math/Transform.h"

namespace UltRenderer {
    namespace Rendering {
        enum class ProjectionType {
            ORTHOGONAL, PERSPECTIVE
        };

        class Camera {
        private:
            double _width;
            double _height;

            double _zMin = 0.1;
            double _zMax = 10;

            ProjectionType _projectionType;
        public:
            Math::Transform3D viewMatrix;
            Math::Transform3D projectionMatrix;

            Camera(double width, double height, double zMin=0.1, double zMax=10, ProjectionType projectionType=ProjectionType::PERSPECTIVE);

            [[nodiscard]] double width() const;
            [[nodiscard]] double height() const;
            [[nodiscard]] double zMin() const;
            [[nodiscard]] double zMax() const;

            void updateProjectionMatrix();

            void setProjectionType(ProjectionType projectionType);
            void setWidth(double width);
            void setHeight(double height);
            void setZMin(double zMin);
            void setZMax(double zMax);

            void render(std::size_t width, std::size_t height) const;
        };

    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_CAMERA_H
