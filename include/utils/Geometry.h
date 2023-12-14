//
// Created by ultcrt on 23-12-14.
//

#ifndef ULTRENDERER_GEOMETRY_H
#define ULTRENDERER_GEOMETRY_H

#include <utility>
#include <vector>
#include "data/Matrix.h"

namespace UltRenderer {
    namespace Utils {
        namespace Geometry {
            template<typename T, std::size_t N>
            std::pair<Data::Matrix<T, N, 1>, Data::Matrix<T, N, 1>> GetAABB(const std::vector<Data::Matrix<T, N, 1>>& points);

            template<typename T, std::size_t N>
            std::pair<Data::Matrix<T, N, 1>, Data::Matrix<T, N, 1>>
            GetAABB(const std::vector<Data::Matrix<T, N, 1>> &points) {
                Data::Matrix<T, N, 1> minVec;
                Data::Matrix<T, N, 1> maxVec;

                if (!points.empty()) {
                    minVec = points[0];
                    maxVec = points[1];

                    for (auto iter = points.begin() + 1; iter != points.end(); iter++) {
                        for (std::size_t channelIdx = 0; channelIdx < N; channelIdx++) {
                            if (minVec[channelIdx] > (*iter)[channelIdx]) {
                                minVec[channelIdx] = (*iter)[channelIdx];
                            }
                            else if (maxVec[channelIdx] < (*iter)[channelIdx]) {
                                maxVec[channelIdx] = (*iter)[channelIdx];
                            }
                        }
                    }
                }

                return {minVec, maxVec};
            }
        }
    } // Utils
} // UltRender

#endif //ULTRENDERER_GEOMETRY_H
