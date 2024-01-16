//
// Created by ultcrt on 24-1-16.
//

#ifndef ULTRENDERER_PREBAKEDAMBIENTOCCLUSION_H
#define ULTRENDERER_PREBAKEDAMBIENTOCCLUSION_H

#include "data/Image.h"
#include "data/TriangleMesh.h"
#include "rendering/Camera.h"

namespace UltRenderer {
    namespace Bakers {
        class PreBakedAmbientOcclusion {
        public:
            std::size_t samplingNum;

            Data::Image operator()(const Data::TriangleMesh& mesh) const;
        };
    } // Bakers
} // UltRenderer

#endif //ULTRENDERER_PREBAKEDAMBIENTOCCLUSION_H
