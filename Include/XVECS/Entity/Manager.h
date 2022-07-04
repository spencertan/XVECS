#pragma once

namespace XV::ECS
{

  struct Entity::Data final
  {

    Ptr<Archetype::Instance> archetype{nullptr};
    Ptr<Archetype::Pool> pool{nullptr};
    i32 pool_index{};
    Validation validation{2147483649u};
  };

  class Entity::Manager final
  {
    friend class Archetype::Pool;
    array<Data, Settings::max_entities> m_entities;
    i32 m_head{0};
    u32 m_count{0};
    u32 m_max{0};

  public:
    inline Manager() noexcept;
    inline void Initialise();
    inline Entity Allocate(Archetype::Instance &archetype, Archetype::Pool &pool, i32 pool_index) noexcept;
    inline void Delete(u32 index) noexcept;
    inline void Delete(u32 index, const Entity &entity) noexcept;
    inline const Entity::Data &Get(Entity entity) const noexcept;
    inline Entity::Data &Get(Entity entity) noexcept;
    inline void Move(i32 pool_index, const Entity &entity) noexcept;
  };

}