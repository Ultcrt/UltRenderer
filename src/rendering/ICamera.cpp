//
// Created by ultcrt on 24-1-23.
//

#include "rendering/ICamera.h"

namespace UltRenderer {
    namespace Rendering {
        void ICamera::setProjectionType(ProjectionType projectionType) {
            _projectionType = projectionType;
        }

        void ICamera::setWidth(double width) {
            _width = width;
        }

        void ICamera::setHeight(double height) {
            _height = height;
        }

        void ICamera::setZMin(double zMin) {
            _zMin = zMin;
        }

        void ICamera::setZMax(double zMax) {
            _zMax = zMax;
        }

        ICamera::ICamera(double width, double height, double zMin, double zMax, ProjectionType projectionType): _width(width), _height(height), _zMin(zMin), _zMax(zMax), _projectionType(projectionType) {}
    } // Rendering
} // UltRenderer