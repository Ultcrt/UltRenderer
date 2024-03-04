//
// Created by ultcrt on 24-3-4.
//
#include <memory>
#include "data/Image.h"
#include "data/TriangleMesh.h"
#include "hierarchy/IntersectableNode.h"
#include "rendering/Scene.h"
#include "rendering/raytracing/Camera.h"

using namespace UltRenderer;
using namespace std;

inline unique_ptr<Rendering::Scene> CreateCornellBox() {
    auto pScene = std::make_unique<Rendering::Scene>();

    auto pCamera = make_shared<Rendering::Raytracing::Camera>(0.025, 0.035, 0.025);
    pCamera->transformMatrix = Math::Transform3D::FromLookAt(
            {278.0, 273.0, -800.0},
            {278.0, 273.0, -799.0},
            {0, 1, 0}
    );

    vector<Math::Vector3D> floorVertices = {
            {552.8, 0.0, 0.0},
            {0.0,   0.0, 0.0},
            {0.0,   0.0, 559.2},
            {549.6, 0.0, 559.2},
    };

    vector<Math::Vector3D> lightVertices = {
            {343.0, 548.8, 227.0},
            {343.0, 548.8, 332.0},
            {213.0, 548.8, 332.0},
            {213.0, 548.8, 227.0},
    };

    vector<Math::Vector3D> ceilVertices = {
            {556.0, 548.8, 0.0},
            {556.0, 548.8, 559.2},
            {0.0,   548.8, 559.2},
            {0.0,   548.8, 0.0},
    };

    vector<Math::Vector3D> backVertices = {
            {549.6, 0.0,   559.2},
            {0.0,   0.0,   559.2},
            {0.0,   548.8, 559.2},
            {556.0, 548.8, 559.2},
    };

    vector<Math::Vector3D> rightVertices = {
            {0.0, 0.0,   559.2},
            {0.0, 0.0,   0.0},
            {0.0, 548.8, 0.0},
            {0.0, 548.8, 559.2},
    };

    vector<Math::Vector3D> leftVertices = {
            {552.8, 0.0,   0.0},
            {549.6, 0.0,   559.2},
            {556.0, 548.8, 559.2},
            {556.0, 548.8, 0.0},
    };

    vector<Math::Vector3D> shortVertices = {
            {130.0, 165.0, 65.0},
            {82.0,  165.0, 225.0},
            {240.0, 165.0, 272.0},
            {290.0, 165.0, 114.0},

            {290.0, 0.0,   114.0},
            {290.0, 165.0, 114.0},
            {240.0, 165.0, 272.0},
            {240.0, 0.0,   272.0},

            {130.0, 0.0,   65.0},
            {130.0, 165.0, 65.0},
            {290.0, 165.0, 114.0},
            {290.0, 0.0,   114.0},

            {82.0,  0.0,   225.0},
            {82.0,  165.0, 225.0},
            {130.0, 165.0, 65.0},
            {130.0, 0.0,   65.0},

            {240.0, 0.0,   272.0},
            {240.0, 165.0, 272.0},
            {82.0,  165.0, 225.0},
            {82.0,  0.0,   225.0},
    };

    vector<Math::Vector3D> tallVertices = {
            {423.0, 330.0, 247.0},
            {265.0, 330.0, 296.0},
            {314.0, 330.0, 456.0},
            {472.0, 330.0, 406.0},

            {423.0, 0.0,   247.0},
            {423.0, 330.0, 247.0},
            {472.0, 330.0, 406.0},
            {472.0, 0.0,   406.0},

            {472.0, 0.0,   406.0},
            {472.0, 330.0, 406.0},
            {314.0, 330.0, 456.0},
            {314.0, 0.0,   456.0},

            {314.0, 0.0,   456.0},
            {314.0, 330.0, 456.0},
            {265.0, 330.0, 296.0},
            {265.0, 0.0,   296.0},

            {265.0, 0.0,   296.0},
            {265.0, 330.0, 296.0},
            {423.0, 330.0, 247.0},
            {423.0, 0.0,   247.0},
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
    auto pLight = make_shared<Data::TriangleMesh>(lightVertices, planeTriangles);
    auto pCeil = make_shared<Data::TriangleMesh>(ceilVertices, planeTriangles);
    auto pBack = make_shared<Data::TriangleMesh>(backVertices, planeTriangles);
    auto pRight = make_shared<Data::TriangleMesh>(rightVertices, planeTriangles);
    auto pLeft = make_shared<Data::TriangleMesh>(leftVertices, planeTriangles);

    auto pShort = make_shared<Data::TriangleMesh>(shortVertices, planeTriangles);
    auto pTall = make_shared<Data::TriangleMesh>(tallVertices, planeTriangles);

    pScene->addCamera(pCamera);
    pScene->addMesh(pFloor);
    pScene->addMesh(pLight);
    pScene->addMesh(pCeil);
    pScene->addMesh(pBack);
    pScene->addMesh(pRight);
    pScene->addMesh(pLeft);
    pScene->addMesh(pShort);
    pScene->addMesh(pTall);

    return pScene;
}