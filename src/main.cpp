#include <iostream>
#include "data/Image.h"
#include "rendering/Rasterization.h"
#include "data/Matrix.h"
#include "data/TriangleMesh.h"

int main() {
    UltRenderer::Data::RGBAImage img(1000, 1500);

    UltRenderer::Data::TriangleMesh model("../data/african_head.obj");

    UltRenderer::Rendering::Rasterization::Line(img, 0, 0, 999, 999, {255, 0, 0, 255});


    for (const auto& tri: model.triangles) {
        for (std::size_t idx = 0; idx < 3; idx++) {
            UltRenderer::Data::Vector3D vertex0 = model.vertices[tri[idx]];
            UltRenderer::Data::Vector3D vertex1 = model.vertices[tri[(idx + 1) % 3]];
            UltRenderer::Data::Vector2S point0 = {
                    static_cast<std::size_t>((vertex0.x() + 1) * static_cast<double>(img.shape().y() - 1) / 2),
                    static_cast<std::size_t>((vertex0.y() + 1) * static_cast<double>(img.shape().x() - 1) / 2)
            };
            UltRenderer::Data::Vector2S point1 = {
                    static_cast<std::size_t>((vertex1.x() + 1) * static_cast<double>(img.shape().y() - 1) / 2),
                    static_cast<std::size_t>((vertex1.y() + 1) * static_cast<double>(img.shape().x() - 1) / 2)
            };
            UltRenderer::Rendering::Rasterization::Line(img, point0.x(), point0.y(), point1.x(), point1.y(), {255, 0, 0, 255});
        }
    }

    img.save("test.tga");

    return 0;
}
