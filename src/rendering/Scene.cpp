//
// Created by ultcrt on 23-12-26.
//

#include "rendering/Scene.h"
#include "rendering/Camera.h"

namespace UltRenderer {
    namespace Rendering {
        void Scene::addMesh(const std::shared_ptr<Data::TriangleMesh> &target) {
            target->setScene(this);
            _meshes.emplace_back(target);
        }

        void Scene::addLight(const std::shared_ptr<Rendering::Light> &target) {
            target->setScene(this);
            _lights.emplace_back(target);
        }

        void Scene::addCamera(const std::shared_ptr<Rendering::ICamera> &target) {
            target->setScene(this);
            _cameras.emplace_back(target);
        }

        std::vector<std::shared_ptr<Data::TriangleMesh>> Scene::meshes() {
            return _meshes;
        }

        std::vector<std::shared_ptr<Rendering::Light>> Scene::lights() {
            return _lights;
        }

        std::vector<std::shared_ptr<Rendering::ICamera>> Scene::cameras() {
            return _cameras;
        }


    } // Rendering
} // UltRenderer