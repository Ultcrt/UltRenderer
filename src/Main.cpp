#include "examples/CreateCornellBox.cpp"

int main() {
    auto pScene = CreateCornellBox();

    auto pCamera = pScene->cameras()[0];

    auto img = pCamera->render(128, 128);

    // Saving
    img.save("pathtracing.tga");

    return 0;
}
