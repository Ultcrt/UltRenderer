//
// Created by ultcrt on 24-1-23.
//

#ifndef ULTRENDERER_ICAMERA_H
#define ULTRENDERER_ICAMERA_H

#include "hierarchy/TransformNode.h"
#include "data/Image.h"
#include "postprocessors/EmptyPostprocessor.h"

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
            ICamera(double width, double height,
                   double zMin = 0.1, double zMax = 10, ProjectionType projectionType = ProjectionType::PERSPECTIVE);


            [[nodiscard]] virtual Data::Image render(std::size_t width, std::size_t height) const = 0;

            virtual void setProjectionType(ProjectionType projectionType);
            virtual void setWidth(double width);
            virtual void setHeight(double height);
            virtual void setZMin(double zMin);
            virtual void setZMax(double zMax);
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_ICAMERA_H
