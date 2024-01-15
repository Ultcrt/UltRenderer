//
// Created by ultcrt on 24-1-15.
//

#ifndef ULTRENDERER_SCREENSPACEAMBIENTOCCLUSION_H
#define ULTRENDERER_SCREENSPACEAMBIENTOCCLUSION_H

#include "IPostprocessor.h"

namespace UltRenderer {
    namespace Postprocessors {

        class ScreenSpaceAmbientOcclusion: public IPostprocessor {
        public:
            void operator()(Data::Image& fBuffer, Data::Image& zBuffer) override;

        private:
            static double ComputeMaxSlopeAngle(
                    const Data::Image& zBuffer, const Math::Vector2S& pos, const Math::Vector2D& dir, std::size_t maxExtension = 100);
        };

    } // Postprocessors
} // UltRenderer

#endif //ULTRENDERER_SCREENSPACEAMBIENTOCCLUSION_H
