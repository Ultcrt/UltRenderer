//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_SCENE_H
#define ULTRENDERER_SCENE_H

#include <vector>
#include "data/TriangleMesh.h"
#include "rendering/Light.h"
#include "rendering/rasterizing/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        namespace Rasterizing {
            class Camera;
        }

        // Tips: Make it possible to access shared_ptr of this
        class Scene {
        private:
            std::vector<std::shared_ptr<Data::TriangleMesh>> _meshes;
            std::vector<std::shared_ptr<Light>>   _lights;
            std::vector<std::shared_ptr<Rasterizing::Camera>>  _cameras;

        public:
            void addMesh(const std::shared_ptr<Data::TriangleMesh>& target);
            void addLight(const std::shared_ptr<Light>& target);
            void addCamera(const std::shared_ptr<Rasterizing::Camera>& target);

            [[nodiscard]] std::vector<std::shared_ptr<Data::TriangleMesh>> meshes();
            [[nodiscard]] std::vector<std::shared_ptr<Light>>   lights();
            [[nodiscard]] std::vector<std::shared_ptr<Rasterizing::Camera>>  cameras();
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_SCENE_H
