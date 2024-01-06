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
         * Base vertex shader, user can derive this class to implement their own vertex shader
         * @tparam VARYING A user defined struct returned by vertex shader, need deriving from Varying, works like varying in GLSL.
         */
        template <std::derived_from<Varying> VARYING>
        // Tips: Use concepts (c++20) to make VARYING must be derived from Varying class
        class VertexShader {
        public:
            /**
             * Callback function of vertex shader
             * @param vertex The vertex need processing
             * @return A user defined struct derived from Varying
             */
            virtual VARYING operator()(const Math::Vector3D& vertex) = 0;
        };

        /**
         * Base fragment shader, user can derive this class to implement their own fragment shader
         * @tparam VARYING A user defined struct accepted by fragment shader, need deriving from Varying, works like varying in GLSL.
         * @tparam VERTEX_NUM_IN_PRIMITIVE The number of vertex in a primitive
         */
        template <std::derived_from<Varying> VARYING, std::size_t VERTEX_NUM_IN_PRIMITIVE>
        class FragmentShader {
        public:
            /**
             * Callback function of fragment shader
             * @param varyings Comes from primitive assembly procedure, is a group of the vertex shader output in the same primitive, works like varying in GLSL.
             * @param weights The Weights used to interpolate in primitive
             * @param color The reference of color that need processing
             * @return Return true means color is processed, false means discarded
             */
            virtual bool operator()(const std::array<VARYING, VERTEX_NUM_IN_PRIMITIVE>& varyings,
                                    const Math::Matrix<double, VERTEX_NUM_IN_PRIMITIVE, 1>& weights,
                                    Math::Vector4D& color) = 0;
        };
    } // Rendering
} // UltRender

#endif //ULTRENDERER_SHADER_H
