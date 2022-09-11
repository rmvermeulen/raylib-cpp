#pragma once
#include <variant>

#include "../entity.h"

namespace schmup::state {
    namespace actions {
        struct AddEntity {
            Entity entity;
        };
        struct RemoveEntity {
            int id;
        };
        struct UpdateEntity {
            Entity entity;
        };
    } // namespace actions
    typedef std::variant<actions::AddEntity, actions::RemoveEntity,
                         actions::UpdateEntity>
        ActionType;
} // namespace schmup::state