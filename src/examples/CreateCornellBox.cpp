//
// Created by ultcrt on 24-3-4.
//
#include <memory>
#include "data/Image.h"
#include "data/TriangleMesh.h"
#include "hierarchy/IntersectableNode.h"
#include "rendering/Scene.h"
#include "rendering/raytracing/Camera.h"
#include "rendering/material/LambertianBRDFMaterial.h"
#include "rendering/light/DirectionalLight.h"
#include "rendering/light/PointLight.h"
#include "rendering/light/PlaneLight.h"

using namespace UltRenderer;
using namespace std;

inline unique_ptr<Rendering::Scene> CreateCornellBox(const std::shared_ptr<Rendering::ICamera>& pCamera) {
    auto pScene = std::make_unique<Rendering::Scene>();

    pCamera->setWidth(0.025);
    pCamera->setHeight(0.025);
    pCamera->setZMin(0.035);
    pCamera->transformMatrix = Math::Transform3D::FromLookAt(
            {2.780, 2.730, -8.000},
            {2.780, 2.730, -7.990},
            {0, 1, 0}
    );

    vector<Math::Vector3D> floorVertices = {
            {5.528, 0.0, 0.0},
            {0.0,   0.0, 0.0},
            {0.0,   0.0, 5.592},
            {5.496, 0.0, 5.592},
    };

    vector<Math::Vector3D> ceilVertices = {
            {5.560, 5.488, 0.0},
            {5.560, 5.488, 5.592},
            {0.0,   5.488, 5.592},
            {0.0,   5.488, 0.0},
    };

    vector<Math::Vector3D> backVertices = {
            {5.496, 0.0,   5.592},
            {0.0,   0.0,   5.592},
            {0.0,   5.488, 5.592},
            {5.560, 5.488, 5.592},
    };

    vector<Math::Vector3D> rightVertices = {
            {0.0, 0.0,   5.592},
            {0.0, 0.0,   0.0},
            {0.0, 5.488, 0.0},
            {0.0, 5.488, 5.592},
    };

    vector<Math::Vector3D> leftVertices = {
            {5.528, 0.0,   0.0},
            {5.496, 0.0,   5.592},
            {5.560, 5.488, 5.592},
            {5.560, 5.488, 0.0},
    };

    vector<Math::Vector3D> shortVertices = {
            {1.300, 1.650, 0.650},
            {0.820, 1.650, 2.250},
            {2.400, 1.650, 2.720},
            {2.900, 1.650, 1.140},

            {2.900, 0.0,   1.140},
            {2.900, 1.650, 1.140},
            {2.400, 1.650, 2.720},
            {2.400, 0.0,   2.720},

            {1.300, 0.0,   0.650},
            {1.300, 1.650, 0.650},
            {2.900, 1.650, 1.140},
            {2.900, 0.0,   1.140},

            {0.820,  0.0,   2.250},
            {0.820,  1.650, 2.250},
            {1.300,  1.650, 0.650},
            {1.300,  0.0,   0.650},

            {2.400,  0.0,   2.720},
            {2.400,  1.650, 2.720},
            {0.820,  1.650, 2.250},
            {0.820,  0.0,   2.250},
    };

    vector<Math::Vector3D> tallVertices = {
            {4.230, 3.300, 2.470},
            {2.650, 3.300, 2.960},
            {3.140, 3.300, 4.560},
            {4.720, 3.300, 4.060},

            {4.230, 0.0,   2.470},
            {4.230, 3.300, 2.470},
            {4.720, 3.300, 4.060},
            {4.720, 0.0,   4.060},

            {4.720, 0.0,   4.060},
            {4.720, 3.300, 4.060},
            {3.140, 3.300, 4.560},
            {3.140, 0.0,   4.560},

            {3.140, 0.0,   4.560},
            {3.140, 3.300, 4.560},
            {2.650, 3.300, 2.960},
            {2.650, 0.0,   2.960},

            {2.650, 0.0,   2.960},
            {2.650, 3.300, 2.960},
            {4.230, 3.300, 2.470},
            {4.230, 0.0,   2.470},
    };

    vector<Math::Vector3S> planeTriangles = {
            {0, 1, 2},
            {2, 3, 0},
    };

    vector<Math::Vector3S> blockTriangles = {
            {0, 1, 2},
            {2, 3, 0},

            {4,  5,  6},
            {6,  7,  4},

            {8,  9, 10},
            {10, 11, 8},

            {12, 13, 14},
            {14, 15, 12},

            {16, 17, 18},
            {18, 19, 16},
    };

    auto pFloor = make_shared<Data::TriangleMesh>(floorVertices, planeTriangles);
    auto pCeil = make_shared<Data::TriangleMesh>(ceilVertices, planeTriangles);
    auto pBack = make_shared<Data::TriangleMesh>(backVertices, planeTriangles);
    auto pRight = make_shared<Data::TriangleMesh>(rightVertices, planeTriangles);
    auto pLeft = make_shared<Data::TriangleMesh>(leftVertices, planeTriangles);

    auto pShort = make_shared<Data::TriangleMesh>(shortVertices, blockTriangles);
    auto pTall = make_shared<Data::TriangleMesh>(tallVertices, blockTriangles);

    pFloor->pMaterial = std::make_shared<Rendering::Material::LambertianBRDFMaterial>();
    pFloor->pMaterial->diffuseColor = {1, 1, 1};

    pCeil->pMaterial = std::make_shared<Rendering::Material::LambertianBRDFMaterial>();
    pCeil->pMaterial->diffuseColor = {1, 1, 1};

    pBack->pMaterial = std::make_shared<Rendering::Material::LambertianBRDFMaterial>();
    pBack->pMaterial->diffuseColor = {1, 1, 1};

    pRight->pMaterial = std::make_shared<Rendering::Material::LambertianBRDFMaterial>();
    pRight->pMaterial->diffuseColor = {0, 1, 0};

    pLeft->pMaterial = std::make_shared<Rendering::Material::LambertianBRDFMaterial>();
    pLeft->pMaterial->diffuseColor = {1, 0, 0};

    pShort->pMaterial = std::make_shared<Rendering::Material::LambertianBRDFMaterial>();
    pShort->pMaterial->diffuseColor = {1, 1, 1};

    pTall->pMaterial = std::make_shared<Rendering::Material::LambertianBRDFMaterial>();
    pTall->pMaterial->diffuseColor = {1, 1, 1};


    pScene->addCamera(pCamera);
    pScene->addMesh(pFloor);
    pScene->addMesh(pCeil);
    pScene->addMesh(pBack);
    pScene->addMesh(pRight);
    pScene->addMesh(pLeft);
    pScene->addMesh(pShort);
    pScene->addMesh(pTall);

    auto pLight = make_shared<Rendering::Light::PlaneLight>(1.30, 1.05, 100);
    pLight->transformMatrix = {
            1., 0., 0., 2.780,
            0., -1., 0., 5.488,
            0., 0., 1., 2.795,
            0., 0., 0., 1.,
    };
    pScene->addAreaLight(pLight);

    return pScene;
}