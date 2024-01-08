//
// Created by ultcrt on 24-1-8.
//

#include "shaders/PhongShader.h"

namespace UltRenderer {
    namespace Shaders {
        PhongVarying
        PhongInterpolator::operator()(const std::array<PhongVarying, 2> &varyings, const Math::Vector2D &weights) const {
            PhongVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1];

            return res;
        }

        PhongVarying
        PhongInterpolator::operator()(const std::array<PhongVarying, 3> &varyings, const Math::Vector3D &weights) const {
            PhongVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];
            res.normal = varyings[0].normal * weights[0] + varyings[1].normal * weights[1] + varyings[2].normal * weights[2];

            return res;
        }

        PhongVertexShader::PhongVertexShader(const std::vector<Math::Vector3D>& vs, const std::vector<Math::Vector3D>& ns, const std::vector<Math::Vector3D>& uvs,
                                           const Math::Transform3D& model, const Math::Transform3D& view, const Math::Transform3D& projection):
                vertices(vs), normals(ns), uvs(uvs), model(model), view(view), projection(projection) {}

        // TODO: Should apply here?
        PhongVarying PhongVertexShader::operator()(std::size_t vIdx) const {
            PhongVarying res;

            res.position = projection * view * model * vertices[vIdx].toHomogeneousCoordinates(1);

            res.uv = uvs[vIdx];

            res.normal = normals[vIdx];

            return res;
        }

        PhongFragmentShader::PhongFragmentShader(const Data::Image &t, const Math::Vector3D& l): texture(t), light(l) {}

        bool PhongFragmentShader::operator()(const UltRenderer::Shaders::PhongVarying &varying, Math::Vector4D &color,
                                            double &depth) const {
            std::size_t width = texture.width();
            std::size_t height = texture.height();

            Math::Vector3D rgb;
            switch (texture.type()) {
                case Data::ImageFormat::RGBA:
                    rgb = texture.at<Data::ImageFormat::RGBA>(std::lround(static_cast<double>(width) * varying.uv[0]), std::lround(static_cast<double>(height) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
                case Data::ImageFormat::RGB:
                    rgb = texture.at<Data::ImageFormat::RGB>(std::lround(static_cast<double>(width) * varying.uv[0]), std::lround(static_cast<double>(height) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
                case Data::ImageFormat::GRAY:
                    rgb = texture.at<Data::ImageFormat::GRAY>(std::lround(static_cast<double>(width) * varying.uv[0]), std::lround(static_cast<double>(height) * varying.uv[1])).convertTo<Data::ImageFormat::RGB>();
                    break;
            }

            color = (varying.normal.dot(-light) * rgb).toHomogeneousCoordinates(1);

            return true;
        }
    } // Shaders
} // UltRenderer