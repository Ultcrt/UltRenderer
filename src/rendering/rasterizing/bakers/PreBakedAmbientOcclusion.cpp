//
// Created by ultcrt on 24-1-16.
//

#include <vector>
#include "rendering/rasterizing/bakers/PreBakedAmbientOcclusion.h"
#include "utils/Random.h"
#include "rendering/rasterizing/shaders/DepthMeshShader.h"
#include "rendering/rasterizing/shaders/PreBakedAmbientOcclusionMeshShader.h"
#include "rendering/rasterizing/Helper.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            namespace Bakers {
                Data::Image PreBakedAmbientOcclusion::operator()(const std::shared_ptr<Data::TriangleMesh> &pMesh) const {
                    Data::Image bakedTexture(width, height, Data::ColorFormat::GRAY);

                    std::vector<Math::Vector3D> sampledPoints = Utils::Random::SampleFromUnitSphere(samplingNum);

                    Shaders::PreBakedAmbientOcclusionMeshInterpolator interpolator;
                    Shaders::PreBakedAmbientOcclusionMeshVertexShader vertexShader;

                    for (const auto samplePoint: sampledPoints) {
                        Data::Image localBakedTexture(width, height, Data::ColorFormat::GRAY);
                        Shaders::PreBakedAmbientOcclusionMeshFragmentShader fragmentShader(localBakedTexture);

                        // Baking is done under pMesh local coordinate, so model matrix is identity matrix
                        Math::Transform3D model = Math::Matrix4D::Identity();
                        Math::Transform3D view;
                        Math::Transform3D projection;
                        Math::Transform3D viewport;
                        Data::Image depthImage(width, height, Data::Color<Data::ColorFormat::GRAY>{1});
                        RenderDepthImageOfMeshes({pMesh}, -samplePoint, depthImage, &view, &projection, &viewport);

                        vertexShader.pModel = &model;
                        vertexShader.pView = &view;
                        vertexShader.pProjection = &projection;
                        vertexShader.modelViewMatrix = view * model;
                        vertexShader.modelViewProjectionMatrix = projection * vertexShader.modelViewMatrix;
                        vertexShader.pVertices = &pMesh->vertices;
                        vertexShader.pUvs = &pMesh->vertexTextures;

                        fragmentShader.pShadowMap = &depthImage;

                        Data::Image fBuffer(width, height, Data::ColorFormat::RGBA);
                        Data::Image zBuffer(width, height, Data::Color<Data::ColorFormat::GRAY>{1});
                        Pipeline::Execute<Shaders::IMeshVarying>(fBuffer, zBuffer, viewport, pMesh->vertices.size(), pMesh->triangles, {}, {}, vertexShader, fragmentShader, interpolator);

                        // TODO: Should support some operation in Image
                        for (std::size_t x = 0; x < width; x++) {
                            for (std::size_t y = 0; y < width; y++) {
                                double val = bakedTexture.at<Data::ColorFormat::GRAY>(x, y)[0];
                                bakedTexture.at<Data::ColorFormat::GRAY>(x, y)[0] = val + localBakedTexture.at<Data::ColorFormat::GRAY>(x, y)[0] / static_cast<double>(samplingNum);
                            }
                        }
                    }

                    return bakedTexture;
                }
            } // Bakers
        } // Rasterizing
    } // Rendering
} // UltRenderer