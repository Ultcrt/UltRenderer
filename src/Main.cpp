#include <iostream>
#include <numeric>
#include "data/Image.h"
#include "rendering/Rasterize.h"
#include "data/Matrix.h"
#include "data/TriangleMesh.h"

int main() {
    UltRenderer::Data::Image img(1920, 1920, UltRenderer::Data::Pixel<UltRenderer::Data::ImageFormat::RGBA>{0., 0., 0., 0.});

    UltRenderer::Data::TriangleMesh model("../data/african_head.obj");

    UltRenderer::Data::Vector3D light = {0, 0, -1};

    std::vector<double> zBuffer(img.height() * img.width(), std::numeric_limits<double>::lowest());

    for (const auto& tri: model.triangles) {
        UltRenderer::Data::Vector3D vertex0 = model.vertices[tri[0]];
        UltRenderer::Data::Vector3D vertex1 = model.vertices[tri[1]];
        UltRenderer::Data::Vector3D vertex2 = model.vertices[tri[2]];
        UltRenderer::Data::Vector2S point0 = {
                static_cast<std::size_t>((vertex0.x() + 1) * static_cast<double>(img.shape().x() - 1) / 2),
                static_cast<std::size_t>((vertex0.y() + 1) * static_cast<double>(img.shape().y() - 1) / 2)
        };
        UltRenderer::Data::Vector2S point1 = {
                static_cast<std::size_t>((vertex1.x() + 1) * static_cast<double>(img.shape().x() - 1) / 2),
                static_cast<std::size_t>((vertex1.y() + 1) * static_cast<double>(img.shape().y() - 1) / 2)
        };
        UltRenderer::Data::Vector2S point2 = {
                static_cast<std::size_t>((vertex2.x() + 1) * static_cast<double>(img.shape().x() - 1) / 2),
                static_cast<std::size_t>((vertex2.y() + 1) * static_cast<double>(img.shape().y() - 1) / 2)
        };

        UltRenderer::Data::Vector3D triangleNormal = ((vertex0 - vertex2).cross(vertex0 - vertex1)).normalized();
        double ratio = triangleNormal.dot(light);

        if (ratio > 0) {
            UltRenderer::Rendering::Rasterize::Triangle<UltRenderer::Data::ImageFormat::RGBA>(img, {point0, point1, point2}, {vertex0.z(), vertex1.z(), vertex2.z()}, {ratio, ratio, ratio, 1}, zBuffer);
        }
    }

    img.flip(UltRenderer::Data::ImageDirection::HORIZONTAL);

    img.save("test.tga");

    return 0;
}
