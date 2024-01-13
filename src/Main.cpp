#include "data/Image.h"
#include "math/Matrix.h"
#include "data/TriangleMesh.h"
#include <memory>
#include "rendering/Camera.h"
#include "rendering/Light.h"
#include "rendering/Scene.h"
#include "shaders/FlatMeshShader.h"
#include "shaders/BlinnPhongReflectionMeshShader.h"

using namespace UltRenderer;

int main() {
    Shaders::BlinnPhongReflectionMeshInterpolator it;
    Shaders::BlinnPhongReflectionMeshVertexShader vs;
    Shaders::BlinnPhongReflectionMeshFragmentShader fs;

    fs.diffuseCoefficient = 0.5;
    fs.specularCoefficient = 0.4;
    fs.ambientCoefficient = 0.1;

    fs.specularColor = {1, 1, 1};
    fs.ambientColor = {0.2, 0.1, 0};
    
    auto pTexture = std::make_shared<Data::Image>("../data/african_head_diffuse.tga");
    auto pNormalMap = std::make_shared<Data::Image>("../data/african_head_nm_tangent.tga");
    auto pSpecular = std::make_shared<Data::Image>("../data/african_head_spec.tga");
    auto pMesh = std::make_shared<Data::TriangleMesh>("../data/african_head.obj");
    auto pCamera = std::make_shared<Rendering::Camera>(2, 2, 4);
    auto pLight = std::make_shared<Rendering::Light>(Math::Vector3D{0, 0, -1}, 2);

    // Use 3.14 and a large z, will cause head not at center
    pCamera->transformMatrix = Math::Transform3D({1, 1, 1}, {0, M_PI / 8, -M_PI / 10}, {2.1, 1.8, 5});

    pMesh->pTexture = pTexture;
    pMesh->pNormalMap = pNormalMap;
    pMesh->pSpecular = pSpecular;
    pMesh->normalMapType = Data::NormalMapType::DARBOUX;

    Rendering::Scene scene;

    scene.addCamera(pCamera);
    scene.addMesh(pMesh);
    scene.addLight(pLight);

    auto img = pCamera->render(1920, 1920, vs, fs, it);

    img.save("test.tga");

    return 0;
}
