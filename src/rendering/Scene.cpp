//
// Created by ultcrt on 23-12-26.
//

#include "rendering/Scene.h"

namespace UltRenderer {
    namespace Rendering {
        void Scene::addMesh(const std::shared_ptr<Data::TriangleMesh> &target) {
            target->setScene(this);
            _meshes.emplace_back(target);
            // Mesh is also intersectable
            _intersectables.emplace_back(target);
        }

        void Scene::addIntersectables(const std::shared_ptr<Hierarchy::IntersectableNode> &target) {
            target->setScene(this);
            _intersectables.emplace_back(target);
        }

        void Scene::addNonAreaLight(const std::shared_ptr<Light::INonAreaLight> &target) {
            target->setScene(this);
            _nonAreaLights.emplace_back(target);
        }

        void Scene::addAreaLight(const std::shared_ptr<Light::IAreaLight> &target) {
            target->setScene(this);
            _areaLights.emplace_back(target);
            // Area light is also intersectable
            _intersectables.emplace_back(target);
        }

        void Scene::addCamera(const std::shared_ptr<ICamera> &target) {
            target->setScene(this);
            _cameras.emplace_back(target);
        }

        std::vector<std::shared_ptr<Data::TriangleMesh>> Scene::meshes() const {
            return _meshes;
        }

        std::vector<std::shared_ptr<Light::INonAreaLight>> Scene::nonAreaLights() const {
            return _nonAreaLights;
        }

        std::vector<std::shared_ptr<Light::IAreaLight>> Scene::areaLights() const {
            return _areaLights;
        }

        std::vector<std::shared_ptr<ICamera>> Scene::cameras() const {
            return _cameras;
        }

        std::vector<std::shared_ptr<Hierarchy::IntersectableNode>> Scene::intersectables() const {
            return _intersectables;
        }
    } // Rendering
} // UltRenderer