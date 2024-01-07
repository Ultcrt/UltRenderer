#include <iostream>
#include <numeric>
#include "data/Image.h"
#include "rendering/Rasterize.h"
#include "math/Matrix.h"
#include "data/TriangleMesh.h"
#include <memory>
#include "rendering/Camera.h"
#include "rendering/Light.h"
#include "rendering/Scene.h"

int main() {
    auto pTexture = std::make_shared<UltRenderer::Data::Image>("../data/african_head_diffuse.tga");
    auto pMesh = std::make_shared<UltRenderer::Data::TriangleMesh>("../data/african_head.obj");
    auto pCamera = std::make_shared<UltRenderer::Rendering::Camera>(2, 2, 0.1, 10);
    auto pLight = std::make_shared<UltRenderer::Rendering::Light>(UltRenderer::Math::Vector3D{0, 0, -1});

    // Use 3.14 and a large z, will cause head not at center
    pCamera->transformMatrix = UltRenderer::Math::Transform3D({1, 1, 1}, {0, 0, 0}, {0, 0, 1});

    pMesh->pTexture = pTexture;

    UltRenderer::Rendering::Scene scene;

    scene.addCamera(pCamera);
    scene.addMesh(pMesh);
    scene.addLight(pLight);

    auto img = pCamera->render<UltRenderer::Data::ImageFormat::RGB>(1920, 1920);

    img.save("test.tga");

    return 0;
}
