//
// Created by ultcrt on 24-1-8.
//

#ifndef ULTRENDERER_ICAMERA_H
#define ULTRENDERER_ICAMERA_H

#include "hierarchy/TransformNode.h"
#include "data/Image.h"

namespace UltRenderer {
    namespace Rendering {
        enum class ProjectionType {
            ORTHOGONAL, PERSPECTIVE
        };

        class ICamera: public Hierarchy::TransformNode {
        protected:
            double _width;
            double _height;

            double _zMin = 0.1;
            double _zMax = 10;

            ProjectionType _projectionType;

        public:
            Math::Transform3D projectionMatrix;

            ICamera(double width, double height, double zMin=0.1, double zMax=10, ProjectionType projectionType=ProjectionType::PERSPECTIVE);

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

            [[nodiscard]] virtual Data::Image render(std::size_t width, std::size_t height) const = 0;
        };

    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_ICAMERA_H
