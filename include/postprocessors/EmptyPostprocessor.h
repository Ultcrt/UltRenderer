//
// Created by ultcrt on 24-1-15.
//

#ifndef ULTRENDERER_EMPTYPOSTPROCESSOR_H
#define ULTRENDERER_EMPTYPOSTPROCESSOR_H

#include "IPostprocessor.h"

namespace UltRenderer {
    namespace Postprocessors {
        class EmptyPostprocessor: public IPostprocessor{
            void operator()(Data::Image& fBuffer, Data::Image& zBuffer) override;
        };

    } // Postprocessors
} // UltRenderer

#endif //ULTRENDERER_EMPTYPOSTPROCESSOR_H
