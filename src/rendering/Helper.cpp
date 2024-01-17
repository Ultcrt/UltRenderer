//
// Created by ultcrt on 24-1-17.
//

#include "rendering/Helper.h"
#include "shaders/IMeshShader.h"
#include "math/Geometry.h"
#include "rendering/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        Math::Transform3D RenderDepthImageOfMesh(const Data::TriangleMesh& mesh, const Math::Vector3D& dir, Data::Image& depthImage) {
            const auto nDir = dir.normalized();

            auto [origin, radius] = Math::Geometry::ComputeApproximateBoundingSphere(mesh.vertices);

            const auto w = 2.1 * radius;
            const auto h = 2.1 * radius;
            const auto zMin = -1.05 * radius;
            const auto zMax = 1.05 * radius;

            // User only pass direction here, so only orthogonal projection is needed
            const Math::Transform3D& model = mesh.transformMatrix;
            // dir.dot(Math::Vector3D::Y()): make sure z axis is not parallel to up vector
            const Math::Transform3D& view = Math::Transform3D::FromLookAt(
                origin,
                nDir,
                nDir.dot(Math::Vector3D::Y()) == 1 ? Math::Vector3D::X() : Math::Vector3D::Y()
            ).inverse();

            const auto projection = Rendering::Camera::ComputeProjectionMatrix(w, h, zMin, zMax, ProjectionType::ORTHOGONAL);
            const auto viewport = Rendering::Camera::ComputeViewportMatrix(depthImage.width(), depthImage.height());

            Data::Image fBuffer(depthImage.width(), depthImage.height(), Data::ImageFormat::RGBA);

            Shaders::DepthMeshInterpolator interpolator;
            Shaders::DepthMeshVertexShader vertexShader;
            Shaders::DepthMeshFragmentShader fragmentShader;

            vertexShader.pModel = &model;
            vertexShader.pView = &view;
            vertexShader.pProjection = &projection;
            vertexShader.modelViewMatrix = view * model;
            vertexShader.modelViewProjectionMatrix = projection * vertexShader.modelViewMatrix;
            vertexShader.pVertices = &mesh.vertices;

            Rendering::Pipeline::Execute<Shaders::IMeshVarying>(
                fBuffer, depthImage,
                viewport,
                mesh.vertices.size(),
                mesh.triangles, {}, {}, vertexShader, fragmentShader, interpolator
            );

            return viewport * vertexShader.modelViewProjectionMatrix;
        }
    } // Rendering
} // UltRenderer