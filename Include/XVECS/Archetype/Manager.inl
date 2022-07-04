namespace XV::ECS::Archetype
{

  Archetype::Instance &Manager::Create(ID id, const Signature &signature) noexcept
  {
    m_archetypes.push_back(MakeUnique<Archetype::Instance>(*this, id, signature));
    auto &archetype = *m_archetypes.back().get();
    m_archetype_map.emplace(id, &archetype);
    m_archestypes_signatures.push_back({signature, archetype.m_exclusive});
    m_events.on_new_archetype.Notify(archetype);
    return archetype;
  }

  void Manager::AddToStructuralPendingList(Pool &pool) noexcept
  {
    if (!pool.m_pending_changes)
    {
      pool.m_pending_changes = m_pool_structural_pending_list;
      m_pool_structural_pending_list = &pool
    }
  }

  Manager::Manager(World::Instance &world) noexcept
      : m_world(world)
  {
  }

  Ptr<Archetype::Instance> Manager::GetArchetype(ID id) noexcept
  {
    auto result = m_archetype_map.find(id);
    if (result != m_archetype_map.end())
      return result->second;
  }

  Archetype::Instance &Manager::GetOrCreateArchetype(Signature &signature) noexcept
  {
    assert(signature[Component::info<Entity>.bit_index]);
    auto id = GenerateID(signature);
    if (auto result = m_archetype_map.find(id); result != m_archetype_map.end())
      return *result->second;
    return Create(id, signature);
  }

  void Manager::UpdateStructuralChanges() noexcept
  {
    auto ptr = m_pool_structural_pending_list;
    while (ptr)
    {
      auto next = ptr->m_pending_changes;
      ptr->UpdateStructuralChanges();
      ptr->m_pending_changes = nullptr;
      ptr = next;
    }
    m_pool_structural_pending_list = nullptr;
  }

}