#include <iostream>
#include "data/Image.h"
#include "rendering/Rasterization.h"
#include "data/Matrix.h"

int main() {
    UltRenderer::Data::RGBAImage img(1000, 1000);

    UltRenderer::Rendering::Rasterization::Line(img, 630, 100, 10, 389, {255, 0, 0, 255});
    UltRenderer::Rendering::Rasterization::Line(img, 10, 789, 120, 100, {255, 255, 0, 255});
    UltRenderer::Rendering::Rasterization::Line(img, 133, 56, 12, 750, {255, 255, 0, 255});
    img.save("test.tga");

    UltRenderer::Data::Matrix<int, 3, 2> tmp0({1, 2, 3, 4, 5, 6});
    UltRenderer::Data::Matrix<int, 2, 3> tmp1({1, 2, 3, 4, 5, 6});
    auto tmp3 = tmp1 * tmp0;

    tmp3(0, 1) = 23;

    std::cout << tmp3(0, 1) << std::endl;

    return 0;
}
