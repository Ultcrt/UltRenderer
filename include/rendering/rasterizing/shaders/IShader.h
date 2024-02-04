//
// Created by ultcrt on 24-1-4.
//

#ifndef ULTRENDERER_ISHADER_H
#define ULTRENDERER_ISHADER_H

#include <array>
#include <memory>
#include <concepts>
#include "math/Matrix.h"
#include "math/Transform.h"
#include "utils/Proxy.h"
#include "data/Image.h"

namespace UltRenderer {
    namespace Shaders {
        /**
         * A struct used to pass values from vertex shader to fragment shader in the same primitive, just like varying. User should derive this class to add more varying variables
         */
        struct IVarying {
        };

        /**
         * Base interpolator, user can derive or full specialize this class to implement interpolator for varying variables
         * @tparam V A user defined struct would be interpolated by this class, need to be derived from IVarying, works like varying in GLSL.
         */
        template <std::derived_from<IVarying> V>
        // Tips: Use concepts (c++20) to make V must be derived from IVarying class
        class IInterpolator {
        public:
            /**
             * Callback function of interpolating triangle primitive
             * @param varyings Comes from primitive assembly procedure, is a group of the vertex shader output in the same primitive.
             * @param weights The weights used to interpolate in triangle primitive
             * @return The interpolated V struct
             */
            virtual V operator()(const std::array<V, 3>& varyings, const Math::Vector3D& weights) const = 0;

            /**
             * Callback function of interpolating line primitive
             * @param varyings Comes from primitive assembly procedure, is a group of the vertex shader output in the same primitive.
             * @param weights The weights used to interpolate in line primitive
             * @return The interpolated V struct
             */
            virtual V operator()(const std::array<V, 2>& varyings, const Math::Vector2D& weights) const = 0;
        };

        /***
         * Base vertex shader, user can derive or full specialize this class to implement their own vertex shader
         * @tparam V A user defined struct returned by vertex shader, need to be derived from IVarying, works like varying in GLSL.
         */
        template <std::derived_from<IVarying> V>
        class IVertexShader {
        public:
             /**
              * Callback function of vertex shader
              * @param vIdx The index of current vertex, user can use it to get attribute variables (User should set vertex buffer as member of this class)
              * @param position Works like gl_Position
              * @return A user defined struct derived from IVarying
              */
            virtual V operator()(std::size_t vIdx, Math::Vector4D& position) const = 0;
        };

        /**
         * Base fragment shader, user can derive or full specialize this class to implement their own fragment shader
         * @tparam V A user defined struct accepted by fragment shader, need to be derived from IVarying, works like varying in GLSL.
         */
        template <std::derived_from<IVarying> V>
        class IFragmentShader {
        public:
            /**
             * Callback function of fragment shader
             * @param varying The interpolated variables from rasterization procedure, works like varying in GLSL.
             * @param fragCoord The gl_FragCoord like variable
             * @param color The reference of color that need processing
             * @return Return true means color is processed, false means discarded
             */
            virtual bool operator()(const V& varying, const Math::Vector4D& fragCoord, Math::Vector4D& color, double& depth) const = 0;
        };
    } // Rendering
} // UltRender

#endif //ULTRENDERER_ISHADER_H
