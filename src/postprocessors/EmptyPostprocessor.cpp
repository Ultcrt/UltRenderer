//
// Created by ultcrt on 24-1-15.
//

#include "postprocessors/EmptyPostprocessor.h"

namespace UltRenderer {
    namespace Postprocessors {
        void EmptyPostprocessor::operator()(Data::Image &fBuffer, Data::Image &zBuffer) const {
        }
    } // Postprocessors
} // UltRenderer