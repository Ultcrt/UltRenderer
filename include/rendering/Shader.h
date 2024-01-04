//
// Created by ultcrt on 24-1-4.
//

#ifndef ULTRENDERER_SHADER_H
#define ULTRENDERER_SHADER_H

#include <vector>
#include <memory>
#include "math/Matrix.h"
#include "math/Transform.h"
#include "utils/Proxy.h"
#include "data/Image.h"

namespace UltRenderer {
    namespace Rendering {
        // A struct used to pass values from vertex shader to fragment shader in the same primitive, just like varying
        struct Varying {
            Math::Vector4D position;    // Works like gl_Position
        };

        // Base vertex shader
        class VertexShader {
        public:
            virtual std::unique_ptr<Varying> vertex(const Math::Vector3D& vertex) = 0;
        };

        // Base fragment shader
        class FragmentShader {
        public:
            virtual std::unique_ptr<Varying> vertex(Varying* pVarying) = 0;
        };
    } // Rendering
} // UltRender

#endif //ULTRENDERER_SHADER_H
