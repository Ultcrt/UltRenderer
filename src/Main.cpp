#include "data/Image.h"
#include "math/Matrix.h"
#include "data/TriangleMesh.h"
#include <memory>
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

    auto pTexture = std::make_shared<Data::Image>("../data/diablo3_pose_diffuse.tga", Data::FilterType::LINEAR);
    auto pNormalMap = std::make_shared<Data::Image>("../data/diablo3_pose_nm_tangent.tga", Data::FilterType::LINEAR);
    auto pSpecular = std::make_shared<Data::Image>("../data/diablo3_pose_spec.tga", Data::FilterType::LINEAR);
    auto pGlowMap = std::make_shared<Data::Image>("../data/diablo3_pose_glow.tga", Data::FilterType::LINEAR);
    auto pMesh = std::make_shared<Data::TriangleMesh>("../data/diablo3_pose.obj");
    auto pRasterizingCamera = std::make_shared<Rendering::Rasterizing::Camera>(2, 2, 4);
    auto pRaytracingCamera = std::make_shared<Rendering::Raytracing::Camera>(2, 2, 4);
    auto pLight = std::make_shared<Rendering::Light>(Math::Vector3D{0, 0, -1}, 2);

    // Use 3.14 and a large z, will cause head not at center
    pRasterizingCamera->transformMatrix = Math::Transform3D::FromLookAt({2, 3, 5}, {0, 0, 0}, Math::Vector3D::Y());
    pRaytracingCamera->transformMatrix = Math::Transform3D::FromLookAt({2, 3, 5}, {0, 0, 0}, Math::Vector3D::Y());

    pMesh->pTexture = pTexture;
    pMesh->pNormalMap = pNormalMap;
    pMesh->pSpecular = pSpecular;
    pMesh->normalMapType = Data::NormalMapType::DARBOUX;
    pMesh->pGlowMap = pGlowMap;

    Rendering::Scene scene;

    scene.addCamera(pRasterizingCamera);
    scene.addCamera(pRaytracingCamera);
    scene.addMesh(pMesh);
    scene.addLight(pLight);

    auto imgRasterizing = pRasterizingCamera->render(128, 128, vs, fs, it);
    auto imgRaytracing = pRaytracingCamera->render(128, 128);

    imgRasterizing.save("rasterizing.tga");
    imgRaytracing.save("raytracing.tga");

    return 0;
}
