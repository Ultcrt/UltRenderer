//
// Created by ultcrt on 24-1-15.
//

#ifndef ULTRENDERER_IPOSTPROCESSOR_H
#define ULTRENDERER_IPOSTPROCESSOR_H

#include "data/Image.h"

namespace UltRenderer {
    namespace Postprocessors {
        class IPostprocessor {
        public:
            virtual void operator()(Data::Image& fBuffer, Data::Image& zBuffer) const = 0;
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_IPOSTPROCESSOR_H
