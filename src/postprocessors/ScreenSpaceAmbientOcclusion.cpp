//
// Created by ultcrt on 24-1-15.
//

#include <iostream>
#include "postprocessors/ScreenSpaceAmbientOcclusion.h"

namespace UltRenderer {
    namespace Postprocessors {
        void ScreenSpaceAmbientOcclusion::operator()(Data::Image &fBuffer, Data::Image &zBuffer) const {
            const std::size_t width = fBuffer.width();
            const std::size_t height = fBuffer.height();

            for (std::size_t x = 0; x < width; x++) {
                for (std::size_t y = 0; y < height; y++) {
                    const double depth = zBuffer.at<Data::ImageFormat::GRAY>(x, y)[0];

                    if (depth > 0) {
                        double approxSolidAngle = 0;
                        for (std::size_t idx = 0; idx < 8; idx++) {
                            const double angle = M_PI * 2 / 8 * static_cast<double>(idx);
                            approxSolidAngle += ComputeMaxSlopeAngle(
                                    zBuffer, {x, y}, {std::cos(angle), std::sin(angle)}
                            );
                        }
                        approxSolidAngle = 1 - approxSolidAngle / (M_PI / 2 * 8);

                        // Increase contrast
                        approxSolidAngle = std::pow(approxSolidAngle, 100.);

                        // Update image
                        Math::Vector3D color = fBuffer.at<Data::ImageFormat::RGB>(x, y);
                        fBuffer.at<Data::ImageFormat::RGB>(x, y) = color * approxSolidAngle;
                    }
                }
            }
        }

        double ScreenSpaceAmbientOcclusion::ComputeMaxSlopeAngle(const Data::Image &zBuffer, const Math::Vector2S &pos,
                                                                 const Math::Vector2D &dir, std::size_t maxExtension) {
            const Math::Vector2D posDouble = static_cast<Math::Vector2D>(pos);
            const std::size_t width = zBuffer.width();
            const std::size_t height = zBuffer.height();

            double maxAngle = 0;
            for (std::size_t scaling = 0; scaling < maxExtension; scaling++) {
                Math::Vector2D end = {
                        std::round(posDouble.x() + dir.x() * static_cast<double>(scaling)),
                        std::round(posDouble.y() + dir.y() * static_cast<double>(scaling)),
                };

                if (end.x() >= static_cast<double>(width) || end.x() < 0 || end.y() >= static_cast<double>(height) || end.y() < 0) {
                    break;
                }

                const double dist = (end - posDouble).norm();

                // Skip when end and pos are in the same pixel
                if (dist >= 1) {
                    const double posDepth = zBuffer.at<Data::ImageFormat::GRAY>(pos)[0];
                    const double endDepth = zBuffer.at<Data::ImageFormat::GRAY>(static_cast<Math::Vector2S>(end))[0];
                    const double deltaDepth = endDepth - posDepth;

                    const double slopeAngle = deltaDepth / dist;

                    maxAngle = std::max(maxAngle, std::atan(slopeAngle));
                }
            }

            return maxAngle;
        }
    } // Postprocessors
} // UltRenderer