//
// Created by ultcrt on 24-1-31.
//

#ifndef ULTRENDERER_COLOR_H
#define ULTRENDERER_COLOR_H

#include <vector>
#include "utils/Proxy.h"

namespace UltRenderer {
    namespace Data {
        /*----------Declaration----------*/
        enum class ColorFormat: std::size_t {
            GRAY = 1, RGB = 3, RGBA = 4
        };

        template<ColorFormat FORMAT>
        using ColorProxy = Utils::MatrixProxy<double, static_cast<std::size_t>(FORMAT), 1>;

        // Why not "using Color = Math::VectorXD<static_cast<std::size_t>(FORMAT)>;" ?
        // Because compiler cannot infer template parameter from above line. Also, there are more functionality
        template<ColorFormat FORMAT>
        class Color: public Math::VectorXD<static_cast<std::size_t>(FORMAT)> {
        public:
            // Tips: Inherit all constructor of base class
            using Math::VectorXD<static_cast<std::size_t>(FORMAT)>::VectorXD;

            Color(const Math::VectorXD<static_cast<std::size_t>(FORMAT)>& target);

            template<ColorFormat TARGET>
            Color<TARGET> to() const;
        };

        /*----------Definition----------*/
        template<ColorFormat FORMAT>
        template<ColorFormat TARGET>
        Color<TARGET> Color<FORMAT>::to() const {
            if constexpr (FORMAT == TARGET) {
                return *this;
            }
            else if constexpr (FORMAT == ColorFormat::GRAY && TARGET == ColorFormat::RGB) {
                return Color<TARGET>{1, 1, 1} * this->x();
            }
            else if constexpr (FORMAT == ColorFormat::GRAY && TARGET == ColorFormat::RGBA) {
                return Color<TARGET>{this->x(), this->x(), this->x(), 1};
            }
            else if constexpr (FORMAT == ColorFormat::RGB && TARGET == ColorFormat::GRAY) {
                return Color<TARGET>{0.299 * this->x() + 0.587 * this->y() + 0.114 * this->z()};
            }
            else if constexpr (FORMAT == ColorFormat::RGB && TARGET == ColorFormat::RGBA) {
                return Color<TARGET>{this->x(), this->y(), this->z(), 1};
            }
            else if constexpr (FORMAT == ColorFormat::RGBA && TARGET == ColorFormat::GRAY) {
                Color<ColorFormat::RGB> rgb = to<ColorFormat::RGB>();
                return rgb.to<TARGET>();
            }
            else if constexpr (FORMAT == ColorFormat::RGBA && TARGET == ColorFormat::RGB) {
                return Color<TARGET>{this->x(), this->y(), this->z()};
            }
        }

        template<ColorFormat FORMAT>
        Color<FORMAT>::Color(const Math::VectorXD<static_cast<std::size_t>(FORMAT)>& target): Math::VectorXD<static_cast<std::size_t>(FORMAT)>(target) {}
    } // Data
} // UltRenderer

#endif //ULTRENDERER_COLOR_H
