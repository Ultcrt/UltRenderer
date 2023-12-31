//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_SCENE_H
#define ULTRENDERER_SCENE_H

#include <vector>
#include "data/TriangleMesh.h"
#include "rendering/Light.h"
#include "rendering/ICamera.h"

namespace UltRenderer {
    namespace Rendering {
        // Tips: Make it possible to access shared_ptr of this
        class Scene {
        private:
            std::vector<std::shared_ptr<Data::TriangleMesh>> _meshes;
            std::vector<std::shared_ptr<Rendering::Light>>   _lights;
            std::vector<std::shared_ptr<Rendering::ICamera>>  _cameras;

        public:
            void addMesh(const std::shared_ptr<Data::TriangleMesh>& target);
            void addLight(const std::shared_ptr<Rendering::Light>& target);
            void addCamera(const std::shared_ptr<Rendering::ICamera>& target);

            [[nodiscard]] std::vector<std::shared_ptr<Data::TriangleMesh>> meshes();
            [[nodiscard]] std::vector<std::shared_ptr<Rendering::Light>>   lights();
            [[nodiscard]] std::vector<std::shared_ptr<Rendering::ICamera>>  cameras();
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_SCENE_H
