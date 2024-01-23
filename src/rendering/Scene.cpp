//
// Created by ultcrt on 23-12-26.
//

#include "rendering/Scene.h"

namespace UltRenderer {
    namespace Rendering {
        void Scene::addMesh(const std::shared_ptr<Data::TriangleMesh> &target) {
            target->setScene(this);
            _meshes.emplace_back(target);
        }

        void Scene::addLight(const std::shared_ptr<Light> &target) {
            target->setScene(this);
            _lights.emplace_back(target);
        }

        void Scene::addCamera(const std::shared_ptr<ICamera> &target) {
            target->setScene(this);
            _cameras.emplace_back(target);
        }

        std::vector<std::shared_ptr<Data::TriangleMesh>> Scene::meshes() const {
            return _meshes;
        }

        std::vector<std::shared_ptr<Light>> Scene::lights() const {
            return _lights;
        }

        std::vector<std::shared_ptr<ICamera>> Scene::cameras() const {
            return _cameras;
        }
    } // Rendering
} // UltRenderer