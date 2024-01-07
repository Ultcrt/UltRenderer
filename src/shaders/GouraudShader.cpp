//
// Created by ultcrt on 24-1-7.
//

#include <iostream>
#include "shaders/GouraudShader.h"

namespace UltRenderer {
    namespace Shaders {
        GouraudVarying
        GouraudInterpolator::operator()(const std::array<GouraudVarying, 2> &varyings, const Math::Vector2D &weights) const {
            GouraudVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1];

            return res;
        }

        GouraudVarying
        GouraudInterpolator::operator()(const std::array<GouraudVarying, 3> &varyings, const Math::Vector3D &weights) const {
            GouraudVarying res;

            res.position = varyings[0].position * weights[0] + varyings[1].position * weights[1] + varyings[2].position * weights[2];
            res.uv = varyings[0].uv * weights[0] + varyings[1].uv * weights[1] + varyings[2].uv * weights[2];
            res.intensity = varyings[0].intensity * weights[0] + varyings[1].intensity * weights[1] + varyings[2].intensity * weights[2];

            return res;
        }

        GouraudVertexShader::GouraudVertexShader(
                const Math::Vector3D &l, const std::vector<Math::Vector3D>& vs, const std::vector<Math::Vector3D>& ns, const std::vector<Math::Vector3D>& uvs,
                const Math::Transform3D& model, const Math::Transform3D& view, const Math::Transform3D& projection):
                light(l), vertices(vs), normals(ns), uvs(uvs), model(model), view(view), projection(projection) {}

        GouraudVarying GouraudVertexShader::operator()(std::size_t vIdx) const {
            GouraudVarying res;

            res.intensity = (-light).dot(normals[vIdx]);
            res.uv = uvs[vIdx];

            res.position = projection * view * model * vertices[vIdx].toHomogeneousCoordinates(1);

            return res;
        }

        GouraudFragmentShader::GouraudFragmentShader(const Data::Image &t): texture(t) {}

        bool GouraudFragmentShader::operator()(const UltRenderer::Shaders::GouraudVarying &varying, Math::Vector4D &color,
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

            color = (varying.intensity * rgb).toHomogeneousCoordinates(1);

            return true;
        }
    } // Shaders
} // UltRenderer