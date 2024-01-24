#include "data/Image.h"
#include "math/Matrix.h"
#include "data/TriangleMesh.h"
#include <memory>
#include <chrono>
#include "rendering/rasterizing/Camera.h"
#include "rendering/Light.h"
#include "rendering/Scene.h"
#include "shaders/FlatMeshShader.h"
#include "shaders/BlinnPhongReflectionMeshShader.h"
#include "postprocessors/ScreenSpaceAmbientOcclusion.h"
#include "rendering/raytracing/Camera.h"
#include "math/Ray.h"

using namespace UltRenderer;

int main() {
    Shaders::BlinnPhongReflectionMeshInterpolator it;
    Shaders::BlinnPhongReflectionMeshVertexShader vs;
    Shaders::BlinnPhongReflectionMeshFragmentShader fs;
    Postprocessors::ScreenSpaceAmbientOcclusion ssao;

    fs.diffuseCoefficient = 0.5;
    fs.specularCoefficient = 0.4;
    fs.ambientCoefficient = 0.1;

    fs.specularColor = {1, 1, 1};
    fs.ambientColor = {0.2, 0.1, 0};

    fs.shadowIntensity = 0.3;
    fs.glowIntensity = 3.0;

    // Obj
    auto pTexture = std::make_shared<Data::Image>("../data/diablo3_pose_diffuse.tga", Data::FilterType::LINEAR);
    auto pNormalMap = std::make_shared<Data::Image>("../data/diablo3_pose_nm_tangent.tga", Data::FilterType::LINEAR);
    auto pSpecular = std::make_shared<Data::Image>("../data/diablo3_pose_spec.tga", Data::FilterType::LINEAR);
    auto pGlowMap = std::make_shared<Data::Image>("../data/diablo3_pose_glow.tga", Data::FilterType::LINEAR);
    auto pMesh = std::make_shared<Data::TriangleMesh>("../data/diablo3_pose.obj");

    pMesh->pTexture = pTexture;
    pMesh->pNormalMap = pNormalMap;
    pMesh->pSpecularMap = pSpecular;
    pMesh->normalMapType = Data::NormalMapType::DARBOUX;
    pMesh->pGlowMap = pGlowMap;

    // Floor
    auto pFloorTexture = std::make_shared<Data::Image>("../data/floor_diffuse.tga", Data::FilterType::LINEAR);
    auto pFloorNormalMap = std::make_shared<Data::Image>("../data/floor_nm_tangent.tga", Data::FilterType::LINEAR);
    auto pFloorMesh = std::make_shared<Data::TriangleMesh>("../data/floor.obj");

    pFloorMesh->setTexture(pFloorTexture);
    pFloorMesh->pNormalMap = pFloorNormalMap;
    pFloorMesh->normalMapType = Data::NormalMapType::DARBOUX;

    // Camera
    auto pRasterizingCamera = std::make_shared<Rendering::Rasterizing::Camera>(2, 2, 4);
    auto pRaytracingCamera = std::make_shared<Rendering::Raytracing::Camera>(2, 2, 4);

    pRasterizingCamera->transformMatrix = Math::Transform3D::FromLookAt({2, 3, 5}, {0, 0, 0}, Math::Vector3D::Y());
    pRaytracingCamera->transformMatrix = Math::Transform3D::FromLookAt({2, 3, 5}, {0, 0, 0}, Math::Vector3D::Y());

    // Light
    auto pLight = std::make_shared<Rendering::Light>(Math::Vector3D{0, 0, -1}, 2);

    // Scene
    Rendering::Scene scene;

    scene.addMesh(pMesh);
    scene.addMesh(pFloorMesh);
    scene.addCamera(pRasterizingCamera);
    scene.addCamera(pRaytracingCamera);
    scene.addLight(pLight);

    // Rendering
    auto start = std::chrono::high_resolution_clock::now();
//    auto imgRasterizing = pRasterizingCamera->render(128, 128, vs, fs, it);
    auto imgRaytracing = pRaytracingCamera->render(800, 800);
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(finish-start).count() << "s\n";


    // Saving
//    imgRasterizing.save("rasterizing.tga");
    imgRaytracing.save("raytracing.tga");

    return 0;
}
