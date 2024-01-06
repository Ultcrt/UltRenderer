//
// Created by ultcrt on 24-1-4.
//

#ifndef ULTRENDERER_SHADER_H
#define ULTRENDERER_SHADER_H

#include <array>
#include <memory>
#include <concepts>
#include "math/Matrix.h"
#include "math/Transform.h"
#include "utils/Proxy.h"
#include "data/Image.h"

namespace UltRenderer {
    namespace Rendering {
        /**
         * A struct used to pass values from vertex shader to fragment shader in the same primitive, just like varying. User can derive this class to add more varying variables
         */
        struct Varying {
            Math::Vector4D position;    // Works like gl_Position
        };

        /***
         * Base vertex shader, user can derive or full specialize this class to implement their own vertex shader
         * @tparam V A user defined struct returned by vertex shader, need to be derived from Varying, works like varying in GLSL.
         */
        template <std::derived_from<Varying> V>
        // Tips: Use concepts (c++20) to make VARYING must be derived from Varying class
        class VertexShader {
        public:
            /**
             * Callback function of vertex shader
             * @param vertex The vertex need processing
             * @return A user defined struct derived from Varying
             */
            virtual V operator()(const Math::Vector3D& vertex) = 0 const;
        };

        /**
         * Base fragment shader, user can derive or full specialize this class to implement their own fragment shader
         * @tparam V A user defined struct accepted by fragment shader, need to be derived from Varying, works like varying in GLSL.
         */
        template <std::derived_from<Varying> V>
        class FragmentShader {
        public:
            /**
             * Callback function of fragment shader
             * @param varying The interpolated variables from rasterization procedure, works like varying in GLSL.
             * @param color The reference of color that need processing
             * @return Return true means color is processed, false means discarded
             */
            virtual bool operator()(const V& varying, Math::Vector4D& color) = 0 const;
        };
    } // Rendering
} // UltRender

#endif //ULTRENDERER_SHADER_H
