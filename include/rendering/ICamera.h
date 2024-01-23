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
        class ICamera: public Hierarchy::TransformNode {
        public:
            [[nodiscard]] virtual Data::Image render(std::size_t width, std::size_t height) const = 0;
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_ICAMERA_H
