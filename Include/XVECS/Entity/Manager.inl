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
      m_entities[i].pool_index = i + 1;
      m_entities[i].validation.value = 2147483649u;
      m_entities[i].archetype = nullptr;
      m_entities[i].pool = nullptr;
    }
    m_entities[Settings::max_entities - 1].pool_index = -1;

    m_count = m_max = m_head = 0;
  }

  const Entity::Data &Entity::Manager::Get(Entity entity) const noexcept
  {
    assert(entity.Valid());
    auto &entry = m_entities[entity.GlobalIndex()];
    assert(entry.validation == entity.details.validation);
    return entry;
  }

  Entity::Data &Entity::Manager::Get(Entity entity) noexcept
  {
    return const_cast<Entity::Data &>(static_cast<const Entity::Manager &>(*this).Get(entity));
  }

  Entity Entity::Manager::Allocate(Archetype::Instance &archetype,
                                   Archetype::Pool &pool,
                                   i32 pool_index) noexcept
  {
    assert(m_head >= 0);
    u32 i = static_cast<u32>(m_head);
    auto &entry = m_entities[i];
    m_head = entry.pool_index;

    entry.archetype = &archetype;
    entry.pool = &pool;
    entry.pool_index = pool_index;
    entry.validation.Zombie(false);

    if (i > m_max)
      m_max = i;

    ++m_count;
    return Entity(static_cast<u32>(i), entry.validation);
  }

  void Entity::Manager::Delete(u32 index) noexcept
  {
    auto &entry = m_entities[index];
    entry.validation.IncrementVersion();
    entry.validation.Zombie(true);
    entry.pool_index = m_head;
    m_head = static_cast<i32>(index);
    --m_count;
  }

  void Entity::Manager::Delete(u32 index, const Entity &entity) noexcept
  {
    auto &entry = m_entities[index];
    m_entities[entity.GlobalIndex()].pool_index = entry.pool_index;
    entry.validation.IncrementVersion();
    entry.validation.Zombie(true);
    entry.pool_index = m_head;
    m_head = static_cast<i32>(index);
    --m_count;
  }

  void Entity::Manager::Move(i32 index, const Entity &entity) noexcept
  {
    m_entities[entity.GlobalIndex()].pool_index = index;
  }

}