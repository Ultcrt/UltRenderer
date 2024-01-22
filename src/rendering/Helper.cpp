//
// Created by ultcrt on 24-1-17.
//

#include "rendering/Helper.h"
#include "shaders/IMeshShader.h"
#include "math/Geometry.h"
#include "rendering/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        void RenderDepthImageOfMesh(const Data::TriangleMesh& mesh, const Math::Vector3D& dir, Data::Image& depthImage,
                                    Math::Transform3D* outModelView, Math::Transform3D* outProjection, Math::Transform3D* outViewport) {
            const auto nDir = dir.normalized();

            auto boundingInfo = Math::Geometry::GetAABB(mesh.vertices);

            const auto origin = boundingInfo.origin();
            const auto radius = boundingInfo.radius();

            const auto w = 2.1 * radius;
            const auto h = 2.1 * radius;
            const auto zMin = -1.05 * radius;
            const auto zMax = 1.05 * radius;

            // dir is in mesh local coordinate, so model matrix is identity matrix
            const Math::Transform3D& model = Math::Matrix4D::Identity();
            // dir.dot(Math::Vector3D::Y()): make sure z axis is not parallel to up vector
            const Math::Transform3D& view = Math::Transform3D::FromLookAt(
                origin,
                nDir,
                nDir.dot(Math::Vector3D::Y()) == 1 ? Math::Vector3D::X() : Math::Vector3D::Y()
            ).inverse();

            // User only pass direction here, so only orthogonal projection is needed
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

            if (outModelView != nullptr) {
                // Model matrix is identical
                *outModelView = view;
            }

            if (outProjection != nullptr) {
                *outProjection = projection;
            }

            if (outViewport != nullptr) {
                *outViewport = viewport;
            }
        }
    } // Rendering
} // UltRenderer