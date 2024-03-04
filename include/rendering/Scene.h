//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_SCENE_H
#define ULTRENDERER_SCENE_H

#include <vector>
#include "data/TriangleMesh.h"
#include "hierarchy/IntersectableNode.h"
#include "rendering/light/INonAreaLight.h"
#include "rendering/light/IAreaLight.h"
#include "rendering/ICamera.h"

namespace UltRenderer {
    namespace Rendering {
        // Tips: Make it possible to access shared_ptr of this
        class Scene {
        private:
            std::vector<std::shared_ptr<Data::TriangleMesh>> _meshes;
            std::vector<std::shared_ptr<Hierarchy::IntersectableNode>> _intersectables;
            std::vector<std::shared_ptr<Light::INonAreaLight>>   _nonAreaLights;
            std::vector<std::shared_ptr<Light::IAreaLight>>   _areaLights;
            std::vector<std::shared_ptr<ICamera>>  _cameras;

        public:
            void addMesh(const std::shared_ptr<Data::TriangleMesh>& target);
            void addIntersectables(const std::shared_ptr<Hierarchy::IntersectableNode>& target);
            void addNonAreaLight(const std::shared_ptr<Light::INonAreaLight>& target);
            void addAreaLight(const std::shared_ptr<Light::IAreaLight>& target);
            void addCamera(const std::shared_ptr<ICamera>& target);

            [[nodiscard]] std::vector<std::shared_ptr<Data::TriangleMesh>> meshes() const;
            [[nodiscard]] std::vector<std::shared_ptr<Hierarchy::IntersectableNode>> intersectables() const;
            [[nodiscard]] std::vector<std::shared_ptr<Light::INonAreaLight>> nonAreaLights() const;
            [[nodiscard]] std::vector<std::shared_ptr<Light::IAreaLight>> areaLights() const;
            [[nodiscard]] std::vector<std::shared_ptr<ICamera>> cameras() const;
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_SCENE_H
