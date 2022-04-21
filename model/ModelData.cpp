#include "ModelData.hpp"

namespace kreogl {
    ModelData::~ModelData() noexcept {
        if (release != nullptr)
            release();
    }
}