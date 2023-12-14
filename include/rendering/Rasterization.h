//
// Created by ultcrt on 23-11-21.
//

#ifndef ULTRENDERER_RASTERIZATION_H
#define ULTRENDERER_RASTERIZATION_H

#include <cmath>
#include "data/Matrix.h"
#include "data/Image.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterization {
            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image<FORMAT>& img, std::size_t x0, std::size_t y0, std::size_t x1, std::size_t y1, const UltRenderer::Data::Pixel<FORMAT>& pixel);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image<FORMAT>& img, const Data::Vector2S& p0, const Data::Vector2S& p1, const UltRenderer::Data::Pixel<FORMAT>& pixel);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Triangle(UltRenderer::Data::Image<FORMAT>& img, const Data::Vector2S& p0, const Data::Vector2S& p1, const Data::Vector2S& p2, const  UltRenderer::Data::Pixel<FORMAT>& pixel);

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image<FORMAT> &img, std::size_t x0, std::size_t y0, std::size_t x1,
                                std::size_t y1, const UltRenderer::Data::Pixel<FORMAT> &pixel) {
                const auto dX = static_cast<std::size_t>(std::abs(static_cast<double>(x0) - static_cast<double>(x1)));
                const auto dY = static_cast<std::size_t>(std::abs(static_cast<double>(y0) - static_cast<double>(y1)));

                // Set longer axis as sampling direction
                bool xIsLonger = dX >= dY;

                std::size_t samples, longOrigin, shortOrigin;
                std::size_t step;
                bool shortIsIncreasing;
                if (xIsLonger) {
                    samples = dX;
                    if (x0 <= x1) {
                        longOrigin = x0;
                        shortOrigin = y0;
                        shortIsIncreasing = y0 < y1;
                    }
                    else {
                        longOrigin = x1;
                        shortOrigin = y1;
                        shortIsIncreasing = y1 < y0;
                    }
                    step = dY;
                }
                else {
                    samples = dY;
                    if (y0 <= y1) {
                        longOrigin = y0;
                        shortOrigin = x0;
                        shortIsIncreasing = x0 < x1;
                    }
                    else {
                        longOrigin = y1;
                        shortOrigin = x1;
                        shortIsIncreasing = x1 < x0;
                    }
                    step = dX;
                }

                long long error = 0;
                std::size_t shortPos = shortOrigin;
                for (std::size_t sampleIdx = 0; sampleIdx < samples; sampleIdx++) {
                    const std::size_t longPos = longOrigin + sampleIdx;

                    if (2 * error > static_cast<long long>(samples)) {
                        shortPos += shortIsIncreasing ? 1 : -1;
                        error -= static_cast<long long>(samples);
                    }

                    if (xIsLonger) {
                        img.set(longPos, shortPos, pixel);
                    }
                    else {
                        img.set(shortPos, longPos, pixel);
                    }

                    error += static_cast<long long>(step);
                }
            }

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Line(UltRenderer::Data::Image<FORMAT> &img, const Data::Vector2S &p0,
                                     const Data::Vector2S &p1, const UltRenderer::Data::Pixel<FORMAT> &pixel) {
                Line<FORMAT>(img, p0.x(), p0.y(), p1.x(), p1.y(), pixel);
            }

            template<UltRenderer::Data::ImageFormat FORMAT>
            void Triangle(UltRenderer::Data::Image<FORMAT> &img, const Data::Vector2S &p0,
                                         const Data::Vector2S &p1, const Data::Vector2S &p2,
                                         const UltRenderer::Data::Pixel<FORMAT> &pixel) {
                std::vector<Data::Vector2S> points{p0, p1, p2};

                // Sort three point with y, only compare x when ys are equal
                std::sort(points.begin(), points.end(), [](const Data::Vector2S& a, const Data::Vector2S& b){
                    if (a.y() == b.y()) {
                        return a.x() < b.x();
                    }
                    else {
                        return a.y() < b.y();
                    }
                });

                // Draw border of triangle
                Line<FORMAT>(img, points[0], points[1], pixel);
                Line<FORMAT>(img, points[1], points[2], pixel);
                Line<FORMAT>(img, points[2], points[0], pixel);

                // Filling
                if (points[0].y() < points[1].y()) {
                    double step01 = (static_cast<double>(points[0].x()) - static_cast<double>(points[1].x())) / (static_cast<double>(points[0].y()) - static_cast<double>(points[1].y()));
                    double step02 = (static_cast<double>(points[0].x()) - static_cast<double>(points[2].x())) / (static_cast<double>(points[0].y()) - static_cast<double>(points[2].y()));
                    for (std::size_t idx = 1; idx < points[1].y() - points[0].y(); idx++) {
                        const std::size_t yIdx = points[0].y() + idx;
                        const std::size_t xIdx01 = std::lround(static_cast<double>(points[0].x()) + step01 * static_cast<double>(idx));
                        const std::size_t xIdx02 = std::lround(static_cast<double>(points[0].x()) + step02 * static_cast<double>(idx));
                        Line<FORMAT>(img, xIdx01, yIdx, xIdx02, yIdx, pixel);
                    }

                    if (points[1].y() < points[2].y()) {
                        double step12 = (static_cast<double>(points[1].x()) - static_cast<double>(points[2].x())) / (static_cast<double>(points[1].y()) - static_cast<double>(points[2].y()));
                        for (std::size_t idx = 0; idx < points[2].y() - points[1].y(); idx++) {
                            const std::size_t yIdx = points[1].y() + idx;
                            const std::size_t xIdx12 = std::lround(static_cast<double>(points[1].x()) + step12 * static_cast<double>(idx));
                            const std::size_t xIdx02 = std::lround(static_cast<double>(points[0].x()) + step02 * static_cast<double>(yIdx - points[0].y()));
                            Line<FORMAT>(img, xIdx12, yIdx, xIdx02, yIdx, pixel);
                        }
                    }
                }
            }
        }
    } // UltRenderer
} // Rendering

#endif //ULTRENDERER_RASTERIZATION_H
