#pragma once

#include "Entity.h"
namespace XV::ECS
{

  struct Entity::Data final
  {

    Ptr<Archetype::Instance> archetype{nullptr};
    Ptr<Archetype::Pool::Instance> pool{nullptr};
    i32 pool_index{};
    Validation validation{2147483649u};
  };

  struct Entity::Manager final
  {

    array<Data, Settings::max_entities> entities;
    i32 head{0};
    u32 count{0};
    u32 max{0};

    inline Manager() noexcept;
    inline void Initialise();
    inline Entity Allocate(Archetype::Instance &archetype, Archetype::Pool::Instance &pool, i32 pool_index) noexcept;
    inline void Delete(u32 index) noexcept;
    inline void Delete(u32 index, const Entity &entity) noexcept;
    inline const Entity::Data &Get(Entity entity) const noexcept;
    inline Entity::Data &Get(Entity entity) noexcept;
    inline void Move(i32 pool_index, const Entity &entity) noexcept;
  };

}