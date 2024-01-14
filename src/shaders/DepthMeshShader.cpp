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
            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];

            return res;
        }

        IMeshVarying
        DepthMeshInterpolator::operator()(const std::array<IMeshVarying, 2> &varyings,
                                                  const Math::Vector2D &weights) const {
            IMeshVarying res;
            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] ;

            return res;
        }

        IMeshVarying DepthMeshVertexShader::operator()(std::size_t vIdx) const {
            IMeshVarying res;

            res.position = modelViewProjectionMatrix * (*pVertices)[vIdx].toHomogeneousCoordinates(1);

            return res;
        }

        bool DepthMeshFragmentShader::operator()(const IMeshVarying &varying, Math::Vector4D &color,
                                                         double &depth) const {
            // Apply intensity here
            color.x() = depth;
            color.w() = 1;

            return true;
        }
    } // Shaders
} // UltRenderer