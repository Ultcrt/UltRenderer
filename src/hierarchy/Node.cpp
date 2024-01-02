//
// Created by ultcrt on 24-1-2.
//

#include "hierarchy/Node.h"

namespace UltRenderer {
    namespace Hierarchy {
        void Hierarchy::Node::setScene(const std::shared_ptr<Rendering::Scene> &pScene) {
            _pScene = pScene;
        }
    } // Hierarchy
} // UltRenderer