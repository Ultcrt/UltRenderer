//
// Created by ultcrt on 24-1-17.
//

#include "rendering/rasterizing/Helper.h"
#include "shaders/IMeshShader.h"
#include "math/Geometry.h"
#include "rendering/rasterizing/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            void RenderDepthImageOfMeshes(const std::vector<std::shared_ptr<Data::TriangleMesh>>& pMeshes, const Math::Vector3D& dir, Data::Image& depthImage,
                                          Math::Transform3D* outModelView, Math::Transform3D* outProjection, Math::Transform3D* outViewport) {
                std::vector<Math::Vector3D> represents;

                for (const auto& pMesh: pMeshes) {
                    const auto& minMax = Math::Geometry::GetMinMax(pMesh->vertices);
                    represents.emplace_back(minMax.first);
                    represents.emplace_back(minMax.second);
                }

                const auto boundingInfo = Math::Geometry::GetAABB(represents);


                const auto nDir = dir.normalized();

                const auto origin = boundingInfo.origin();
                const auto radius = boundingInfo.radius();

                const auto w = 2.1 * radius;
                const auto h = 2.1 * radius;
                const auto zMin = -1.05 * radius;
                const auto zMax = 1.05 * radius;

                // dir.dot(Math::Vector3D::Y()): make sure z axis is not parallel to up vector
                const Math::Transform3D& view = Math::Transform3D::FromLookAt(
                        origin,
                        nDir,
                        nDir.dot(Math::Vector3D::Y()) == 1 ? Math::Vector3D::X() : Math::Vector3D::Y()
                ).inverse();

                // User only pass direction here, so only orthogonal projection is needed
                const auto projection = Camera::ComputeProjectionMatrix(w, h, zMin, zMax, ProjectionType::ORTHOGONAL);
                const auto viewport = Camera::ComputeViewportMatrix(depthImage.width(), depthImage.height());

                Data::Image fBuffer(depthImage.width(), depthImage.height(), Data::ColorFormat::RGBA);
                for (const auto& pMesh: pMeshes) {
                    const auto& mesh = *pMesh;

                    const Math::Transform3D& model = mesh.transformMatrix;

                    Shaders::DepthMeshInterpolator interpolator;
                    Shaders::DepthMeshVertexShader vertexShader;
                    Shaders::DepthMeshFragmentShader fragmentShader;

                    vertexShader.pModel = &model;
                    vertexShader.pView = &view;
                    vertexShader.pProjection = &projection;
                    vertexShader.modelViewMatrix = view * model;
                    vertexShader.modelViewProjectionMatrix = projection * vertexShader.modelViewMatrix;
                    vertexShader.pVertices = &mesh.vertices;

                    Pipeline::Execute<Shaders::IMeshVarying>(
                            fBuffer, depthImage,
                            viewport,
                            mesh.vertices.size(),
                            mesh.triangles, {}, {}, vertexShader, fragmentShader, interpolator
                    );
                }

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
        } // Rasterizing

    } // Rendering
} // UltRenderer