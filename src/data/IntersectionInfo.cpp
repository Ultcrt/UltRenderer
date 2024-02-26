//
// Created by ultcrt on 24-1-22.
//

#include "data/IntersectionInfo.h"

namespace UltRenderer {
    namespace Data {
        IntersectionInfo &IntersectionInfo::operator=(const IntersectionInfo &target) {
            this->isIntersected = target.isIntersected;
            this->pNode = target.pNode;
            this->length = target.length;
            this->uv = target.uv;
            this->normal = target.normal;

            return *this;
        }
    } // Data
} // UltRenderer