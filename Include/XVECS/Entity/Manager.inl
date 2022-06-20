#include "Manager.h"
#include <cassert>
namespace XV::ECS
{
  Entity::Manager::Manager() noexcept
  {
    Initialise();
  }

  void Entity::Manager::Initialise()
  {
    for (int i = 0, end = Settings::max_entities - 2; i < end; ++i)
    {
      entities[i].pool_index = i + 1;
      entities[i].validation.value = 2147483649u;
      entities[i].archetype = nullptr;
      entities[i].pool = nullptr;
    }
    entities[Settings::max_entities - 1].pool_index = -1;

    count = max = head = 0;
  }

  const Entity::Data &Entity::Manager::Get(Entity entity) const noexcept
  {
    assert(entity.Valid());
    auto &entry = entities[entity.GlobalIndex()];
    assert(entry.validation == entity.details.validation);
    return entry;
  }

  Entity::Data &Entity::Manager::Get(Entity entity) noexcept
  {
    return const_cast<Entity::Data &>(static_cast<const Entity::Manager &>(*this).Get(entity));
  }

  Entity Entity::Manager::Allocate(Archetype::Instance &archetype,
                                   Archetype::Pool::Instance &pool,
                                   i32 pool_index) noexcept
  {
    assert(head >= 0);
    u32 i = static_cast<u32>(head);
    auto &entry = entities[i];
    head = entry.pool_index;

    entry.archetype = &archetype;
    entry.pool = &pool;
    entry.pool_index = pool_index;
    entry.validation.Zombie(false);

    if (i > max)
      max = i;

    ++count;
    return Entity(static_cast<u32>(i), entry.validation);
  }

  void Entity::Manager::Delete(u32 index) noexcept
  {
    auto &entry = entities[index];
    entry.validation.IncrementVersion();
    entry.validation.Zombie(true);
    entry.pool_index = head;
    head = static_cast<i32>(index);
    --count;
  }

  void Entity::Manager::Delete(u32 index, const Entity &entity) noexcept
  {
    auto &entry = entities[index];
    entities[entity.GlobalIndex()].pool_index = entry.pool_index;
    entry.validation.IncrementVersion();
    entry.validation.Zombie(true);
    entry.pool_index = head;
    head = static_cast<i32>(index);
    --count;
  }

  void Entity::Manager::Move(i32 index, const Entity &entity) noexcept
  {
    entities[entity.GlobalIndex()].pool_index = index;
  }

}