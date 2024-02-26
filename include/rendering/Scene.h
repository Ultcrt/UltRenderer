//
// Created by ultcrt on 23-12-26.
//

#ifndef ULTRENDERER_SCENE_H
#define ULTRENDERER_SCENE_H

#include <vector>
#include "data/TriangleMesh.h"
#include "hierarchy/IntersectableNode.h"
#include "rendering/Light.h"
#include "rendering/ICamera.h"

namespace UltRenderer {
    namespace Rendering {
        // Tips: Make it possible to access shared_ptr of this
        class Scene {
        private:
            std::vector<std::shared_ptr<Data::TriangleMesh>> _meshes;
            std::vector<std::shared_ptr<Hierarchy::IntersectableNode>> _intersectables;
            std::vector<std::shared_ptr<Light>>   _lights;
            std::vector<std::shared_ptr<ICamera>>  _cameras;

        public:
            void addMesh(const std::shared_ptr<Data::TriangleMesh>& target);
            void addIntersectables(const std::shared_ptr<Hierarchy::IntersectableNode>& target);
            void addLight(const std::shared_ptr<Light>& target);
            void addCamera(const std::shared_ptr<ICamera>& target);

            [[nodiscard]] std::vector<std::shared_ptr<Data::TriangleMesh>> meshes() const;
            [[nodiscard]] std::vector<std::shared_ptr<Hierarchy::IntersectableNode>> intersectables() const;
            [[nodiscard]] std::vector<std::shared_ptr<Light>> lights() const;
            [[nodiscard]] std::vector<std::shared_ptr<ICamera>> cameras() const;
        };
    } // Rendering
} // UltRenderer

#endif //ULTRENDERER_SCENE_H
