//
// Created by ultcrt on 24-1-16.
//

#include <vector>
#include "bakers/PreBakedAmbientOcclusion.h"
#include "rendering/Scene.h"
#include "math/Geometry.h"
#include "shaders/DepthMeshShader.h"
#include "shaders/PreBakedAmbientOcclusionMeshShader.h"
#include "rendering/Helper.h"

namespace UltRenderer {
    namespace Bakers {
        Data::Image PreBakedAmbientOcclusion::operator()(const Data::TriangleMesh &mesh) const {
            Data::Image bakedTexture(width, height, Data::ImageFormat::GRAY);

            std::vector<Math::Vector3D> sampledPoints = Math::Geometry::SampleFromUnitSphere(samplingNum);

            Shaders::PreBakedAmbientOcclusionMeshInterpolator interpolator;
            Shaders::PreBakedAmbientOcclusionMeshVertexShader vertexShader;

            for (const auto samplePoint: sampledPoints) {
                Data::Image localBakedTexture(width, height, Data::ImageFormat::GRAY);
                Shaders::PreBakedAmbientOcclusionMeshFragmentShader fragmentShader(localBakedTexture);

                // Baking is done under mesh local coordinate, so model matrix is identity matrix
                Math::Transform3D model = Math::Matrix4D::Identity();
                Math::Transform3D view;
                Math::Transform3D projection;
                Math::Transform3D viewport;
                Data::Image depthImage(width, height, Data::Pixel<Data::ImageFormat::GRAY>{1});
                Rendering::RenderDepthImageOfMesh(mesh, -samplePoint, depthImage, &view, &projection, &viewport);

                vertexShader.pModel = &model;
                vertexShader.pView = &view;
                vertexShader.pProjection = &projection;
                vertexShader.modelViewMatrix = view * model;
                vertexShader.modelViewProjectionMatrix = projection * vertexShader.modelViewMatrix;
                vertexShader.pVertices = &mesh.vertices;
                vertexShader.pUvs = &mesh.vertexTextures;

                fragmentShader.pShadowMap = &depthImage;

                Data::Image fBuffer(width, height, Data::ImageFormat::RGBA);
                Data::Image zBuffer(width, height, Data::Pixel<Data::ImageFormat::GRAY>{1});
                Rendering::Pipeline::Execute<Shaders::IMeshVarying>(fBuffer, zBuffer, viewport, mesh.vertices.size(), mesh.triangles, {}, {}, vertexShader, fragmentShader, interpolator);

                // TODO: Should support some operation in Image
                for (std::size_t x = 0; x < width; x++) {
                    for (std::size_t y = 0; y < width; y++) {
                        double val = bakedTexture.at<Data::ImageFormat::GRAY>(x, y)[0];
                        bakedTexture.at<Data::ImageFormat::GRAY>(x, y)[0] = val + localBakedTexture.at<Data::ImageFormat::GRAY>(x, y)[0] / samplingNum;
                    }
                }
            }

            return bakedTexture;
        }
    } // Bakers
} // UltRenderer