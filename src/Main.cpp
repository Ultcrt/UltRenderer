#include "examples/CreateCornellBox.cpp"
#include "rendering/raytracing/shaders/BackwardsPathtracingShader.h"

int main() {
    const Rendering::Raytracing::Shaders::BackwardsPathtracingShader shader;

    auto pCamera = make_shared<Rendering::Raytracing::Camera>(0, 0);

    auto pScene = CreateCornellBox(pCamera);

    auto img = pCamera->render(256, 256, shader);

    // Saving
    img.save("../img/pathtracing.tga");

    return 0;
}
