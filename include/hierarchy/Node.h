//
// Created by ultcrt on 24-1-2.
//

#ifndef ULTRENDERER_NODE_H
#define ULTRENDERER_NODE_H

#include <memory>

namespace UltRenderer {
    // Tips: Forward declaration to avoid circular dependency
    namespace Rendering {
        class Scene;
    }

    namespace Hierarchy {
        // Class can be inserted into hierarchy
        class Node {
        protected:
            std::shared_ptr<Rendering::Scene> _pScene;

        public:
            void setScene(const std::shared_ptr<Rendering::Scene>& pScene);
        };
    } // Hierarchy
} // UltRenderer

#endif //ULTRENDERER_NODE_H
