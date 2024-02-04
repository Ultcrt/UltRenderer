//
// Created by ultcrt on 24-1-10.
//

#include <iostream>
#include "shaders/DepthMeshShader.h"

namespace UltRenderer {
    namespace Shaders {
        IMeshVarying
        DepthMeshInterpolator::operator()(const std::array<IMeshVarying, 3> &varyings,
                                                  const Math::Vector3D &weights) const {
            IMeshVarying res;

            return res;
        }

        IMeshVarying
        DepthMeshInterpolator::operator()(const std::array<IMeshVarying, 2> &varyings,
                                                  const Math::Vector2D &weights) const {
            IMeshVarying res;

            return res;
        }

        IMeshVarying DepthMeshVertexShader::operator()(std::size_t vIdx, Math::Vector4D& position) const {
            IMeshVarying res;

            position = modelViewProjectionMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            return res;
        }

        bool DepthMeshFragmentShader::operator()(const IMeshVarying &varying, const Math::Vector4D& fragCoord, Math::Vector4D &color,
                                                         double &depth) const {
            // Fragment shader do not need to do anything, only need depth buffer
            return true;
        }
    } // Shaders
} // UltRenderer