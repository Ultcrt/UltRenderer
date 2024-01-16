//
// Created by ultcrt on 24-1-16.
//

#include <vector>
#include "bakers/PreBakedAmbientOcclusion.h"
#include "rendering/Scene.h"
#include "math/Geometry.h"

namespace UltRenderer {
    namespace Bakers {
        Data::Image PreBakedAmbientOcclusion::operator()(const Data::TriangleMesh &mesh) const {
            std::vector<Math::Vector3D> sampledPoints = Math::Geometry::SampleFromUnitSphere(samplingNum);

            auto [origin, radius] = Math::Geometry::ComputeApproximateBoundingSphere(mesh.vertices);

            // Create camera based on bounding sphere to make sure camera can see the whole mesh
            Rendering::Camera camera(2.2 * radius, 2.2 * radius, -1.1 * radius, 1.1 * radius, Rendering::ProjectionType::ORTHOGONAL);

            Rendering::Scene scene;

            scene.addCamera(std::make_shared<Rendering::Camera>(camera));
            scene.addMesh(std::make_shared<Data::TriangleMesh>(mesh));

            for (const Math::Vector3D& sampledPoint: sampledPoints) {
                camera.transformMatrix = Math::Transform3D::FromLookAt({0, 0, 0}, -sampledPoint, {0, 1, 0});


            }
        }
    } // Bakers
} // UltRenderer