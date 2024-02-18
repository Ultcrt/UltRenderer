#include "data/Image.h"
#include "math/Matrix.h"
#include "data/TriangleMesh.h"
#include <memory>
#include <chrono>
#include "rendering/rasterizing/Camera.h"
#include "rendering/Light.h"
#include "rendering/Scene.h"
#include "rendering/rasterizing/shaders/FlatMeshShader.h"
#include "rendering/rasterizing/shaders/BlinnPhongReflectionMeshShader.h"
#include "postprocessors/ScreenSpaceAmbientOcclusion.h"
#include "rendering/raytracing/Camera.h"
#include "math/Ray.h"
#include "rendering/Material.h"
#include "rendering/raytracing/shaders/BackwardsPathtracingShader.h"

using namespace UltRenderer;

int main() {
    // Shaders
    Rendering::Rasterizing::Shaders::BlinnPhongReflectionMeshInterpolator it;
    Rendering::Rasterizing::Shaders::BlinnPhongReflectionMeshVertexShader vs;
    Rendering::Rasterizing::Shaders::BlinnPhongReflectionMeshFragmentShader fs;
    Rendering::Raytracing::Shaders::BackwardsPathtracingShader rs;

    // Postprocessors
    Postprocessors::ScreenSpaceAmbientOcclusion ssao;

    // Obj material
    auto pMat = std::make_shared<Rendering::Material>();
    auto pTexture = std::make_shared<Data::Image>("../data/diablo3_pose_diffuse.tga", Data::FilterType::LINEAR);
    auto pNormalMap = std::make_shared<Data::Image>("../data/diablo3_pose_nm_tangent.tga", Data::FilterType::LINEAR);
    auto pSpecular = std::make_shared<Data::Image>("../data/diablo3_pose_spec.tga", Data::FilterType::LINEAR);
    auto pGlowMap = std::make_shared<Data::Image>("../data/diablo3_pose_glow.tga", Data::FilterType::LINEAR);
    pMat->pGlowMap = pGlowMap;
    pMat->pTexture = pTexture;
    pMat->pSpecularMap = pSpecular;
    pMat->pNormalMap = pNormalMap;
    pMat->normalMapType = UltRenderer::Data::NormalMapType::DARBOUX;
    pMat->refractiveIndex = 1.333;
    pMat->reflectionCoefficient = 0.9;
    pMat->refractionCoefficient = 0.9;

    // Obj
    auto pMesh = std::make_shared<Data::TriangleMesh>("../data/diablo3_pose.obj");
    pMesh->pMaterial = pMat;

    // Floor material
    auto pFloorMat = std::make_shared<Rendering::Material>();
    auto pFloorTexture = std::make_shared<Data::Image>("../data/floor_diffuse.tga", Data::FilterType::LINEAR);
    auto pFloorNormalMap = std::make_shared<Data::Image>("../data/floor_nm_tangent.tga", Data::FilterType::LINEAR);
    pFloorMat->pNormalMap = pFloorNormalMap;
    pFloorMat->pTexture = pFloorTexture;
    pFloorMat->normalMapType = Data::NormalMapType::DARBOUX;

    // Floor
    auto pFloorMesh = std::make_shared<Data::TriangleMesh>("../data/floor.obj");
    pFloorMesh->pMaterial = pFloorMat;

    // Camera
    auto pRasterizingCamera = std::make_shared<Rendering::Rasterizing::Camera>(2, 2, 3);
    auto pRaytracingCamera = std::make_shared<Rendering::Raytracing::Camera>(2, 2, 3);

    pRasterizingCamera->transformMatrix = Math::Transform3D::FromLookAt({2, 3, 5}, {0, 0, 0}, Math::Vector3D::Y());
    pRaytracingCamera->transformMatrix = Math::Transform3D::FromLookAt({2, 3, 5}, {0, 0, 0}, Math::Vector3D::Y());

    // Light
    auto pLight0 = std::make_shared<Rendering::Light>(Math::Vector3D{0, -1, -1}.normalized(), 1);
    auto pLight1 = std::make_shared<Rendering::Light>(Math::Vector3D{-1, -1, -1}.normalized(), 1);

    // Scene
    Rendering::Scene scene;

    scene.addMesh(pMesh);
    scene.addMesh(pFloorMesh);
    scene.addCamera(pRasterizingCamera);
    scene.addCamera(pRaytracingCamera);
    scene.addLight(pLight0);
    scene.addLight(pLight1);

    // Rendering
    auto start = std::chrono::high_resolution_clock::now();
    auto imgRasterizing = pRasterizingCamera->render(800, 800, vs, fs, it);
    auto imgRaytracing = pRaytracingCamera->render(800, 800);
    auto imgPathtracing = pRaytracingCamera->render(128, 128, rs);
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(finish-start).count() << "s\n";


    // Saving
    imgRasterizing.save("rasterizing.tga");
    imgRaytracing.save("raytracing.tga");
    imgPathtracing.save("pathtracing.tga");

    return 0;
}
