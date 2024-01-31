//
// Created by ultcrt on 24-1-21.
//

#include <iostream>
#include "shaders/DepthPeelingMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        bool DepthPeelingMeshFragmentShader::operator()(const IMeshVarying &varying, const Math::Vector4D& fragCoord, Math::Vector4D &color,
                                                 double &depth) const {
            // Discard when depth is equals to last depth layer, to make last layer would not be right into zBuffer
            double lastDepth = (*pLastDepthLayer).at<Data::ColorFormat::GRAY>(static_cast<std::size_t>(fragCoord.x()), static_cast<std::size_t>(fragCoord.y()))[0];
            if (lastDepth >= depth) {
                return false;
            }

            return true;
        }
    } // Shaders
} // UltRenderer