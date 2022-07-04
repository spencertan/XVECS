namespace XV::ECS::Archetype
{

  inline int GetPage(const Component::Descriptor &info, i32 entity) noexcept
  {
    return ((entity * static_cast<i32>(info.size)) - 1) / Settings::page_size;
  }

  /*
   * Constructor / Destructor
   */

  Pool::Pool(Archetype::Instance &archetype_owner, Component::DescriptorSpan data_descriptors) noexcept
  : m_owner(archetype_owner),
    m_descriptors(data_descriptors)
  {
    for (const auto &desc : m_descriptors)
    {
      assert(desc->size <= Settings::page_size); // Assert component size
      auto current_page = GetPage(*desc, Settings::max_entities) + 1;
      m_components.emplace_back(std::bit_cast<Ptr<byte>>(VirtualAlloc(nullptr, current_page * Settings::page_size, MEM_RESERVE, PAGE_NOACCESS)));
      assert(m_components.back()); // Assert component added
    }
  }

  Pool::~Pool() noexcept
  {
    Clear();
    for (auto &component : m_components)
      VirtualFree(component, 0, MEM_RELEASE);
  }

  /*
   * Modifiers
   */

  i32 Pool::Size() const noexcept
  {
    return m_current;
  }

  i32 Pool::Append(u32 count) noexcept
  {
    for (const auto &[i, desc] : Enumerate(m_descriptors))
    {
      auto next_page = GetPage(*desc, m_size + count);
      if (i32 current_page = GetPage(*desc, m_size); m_size == 0 || current_page != next_page)
      {
        if (m_size == 0)
          --current_page;
        auto page_ptr = m_components[i] + Settings::page_size * (current_page + 1);
        auto ptr = std::bit_cast<Ptr<byte>>(VirtualAlloc(page_ptr, (next_page - current_page) * Settings::page_size, MEM_COMMIT, PAGE_READWRITE));
        assert(ptr == page_ptr); // Check memory
      }

      if (desc->constructor)
      {
        auto ptr = &m_components[i][m_size * desc->size];
        for (auto j = count; j; --j)
        {
          desc->constructor(ptr);
          ptr += desc->size;
        }
      }
    }
    i32 index{m_size};
    m_size += count;
    m_current = m_size;
    return index;
  }

  void Pool::Delete(i32 index) noexcept
  {
    auto &entity = GetComponent<Entity>(index);
    assert(entity.Zombie()); // Entity is alive
    entity.Version(m_global_delete_index);
    m_global_delete_index = entity.GlobalIndex();
  }

  void Pool::Clear() noexcept
  {
    // Simple lazy delete
    while (m_size)
      Free(m_size - 1, true);
  }

  i32 Pool::MoveIn(Pool &old_pool, i32 old_index, i32 new_index) noexcept
  {
    i32 src_index = 0;
    i32 dest_index = 0;
    const i32 src_count = static_cast<i32>(old_pool.m_descriptors.size());
    const i32 dest_count = static_cast<i32>(m_descriptors.size());
    while (true)
      if (old_pool.m_descriptors[src_index] == m_descriptors[dest_index])
      {
        auto &desc = *old_pool.m_descriptors[src_index];
        if (desc.move)
          desc.move(&m_components[dest_index][new_index * desc.size],
                    &old_pool.m_components[src_index][old_index * desc.size]);
        else
          std::memcpy(&m_components[dest_index][new_index * desc.size],
                      &old_pool.m_components[src_index][old_index * desc.size],
                      desc.size);

        if (desc.destructor)
          desc.destructor(&old_pool.m_components[src_index][desc.size * old_index]);

        ++src_index;
        ++dest_index;
        if (src_index >= src_count || dest_index >= dest_count)
          break;
      }
      else if (old_pool.m_descriptors[src_index]->id.value < m_descriptors[dest_index]->id.value)
      {
        auto &desc = *old_pool.m_descriptors[src_index];
        if (desc.destructor)
          desc.destructor(&old_pool.m_components[src_index][desc.size * old_index]);
        if (++src_index >= src_count)
          break;
      }
      else if (++dest_index >= dest_count)
        break;

    //
    // Destruct any pending component
    //
    while (src_index < src_count)
    {
      auto &desc = *old_pool.m_descriptors[src_index];
      if (desc.destructor)
        desc.destructor(&old_pool.m_components[src_index][desc.size * old_index]);
      ++src_index;
    }

    //
    // Put the deleted entity into the move deleted linklist
    //
    {
      auto &entity = old_pool.GetComponent<Entity>(old_index);
      entity.Zombie(true);
      entity.Version(old_pool.m_move_delete_index);
      old_pool.m_move_delete_index = old_index;
    }

    return new_index;
  }

  i32 Pool::GetComponentIndex(const Component::Descriptor &descriptor) const noexcept
  {
    for (const auto &[i, desc] : Enumerate(m_descriptors))
      if (&descriptor == desc) [[likely]]
        return static_cast<i32>(i);
      else if (Component::CompareDescriptors(&descriptor, desc))
        return -1;
    return -1;
  }

  i32 Pool::GetComponentIndex(const Component::Descriptor &descriptor, i32 &sequence) const noexcept
  {
    const auto tmp = sequence;
    for (const auto end = static_cast<i32>(m_descriptors.size()); sequence < end; ++sequence)
      if (&descriptor == m_descriptors[sequence]) [[likely]]
        return sequence++;
      else if (Component::CompareDescriptors(&descriptor, m_descriptors[sequence]))
        break;
    sequence = tmp;
    return -1;
  }

  template <Component::Concepts::IsComponent C>
  Decay<C> &Pool::GetComponent(i32 index) noexcept
  {
    using RealC = Decay<C>;
    if constexpr (std::is_same_v<Entity, RealC>)
      return *std::bit_cast<RealC *>(&m_components[0][index * sizeof(RealC)]);
    else
    {
      const auto i = ComponentIndex(Component::info<RealC>);
      return *std::bit_cast<RealC *>(&m_components[i][index * sizeof(RealC)]);
    }
  }

  template <Component::Concepts::IsComponent C>
  Decay<C> &Pool::GetComponent(i32 index, i32 &sequence) noexcept
  {
    using RealC = Raw<C>;
    if constexpr (std::is_same_v<Entity, RealC>)
    {
      assert(sequence == 0);
      ++sequence;
      return *std::bit_cast<RealC *>(&m_components[0][index * sizeof(RealC)]);
    }
    else
    {
      const auto i = ComponentIndex(Component::info<RealC>, sequence);
      return *std::bit_cast<RealC *>(&m_components[i][index * sizeof(RealC)]);
    }
  }

  Ptr<byte> Pool::GetComponent(i32 index, const Component::Descriptor &descriptor) noexcept
  {
    const auto i = GetComponentIndex(descriptor);
    return &m_components[i][index * descriptor.size];
  }

  template <Component::Concepts::IsComponent... Cs>
  tuplet::tuple<Cs &...> Pool::GetComponents(i32 index) noexcept
  {
    return tuplet::tie(Component<Cs>(index)...);
  }

  vector<Ptr<byte>> Pool::GetComponents(i32 index, const Component::Signatures &signatures) noexcept
  {
    vector<Ptr<byte>> vec;
    for (const auto &[i, desc] : Enumerate(m_descriptors))
      if (signatures[desc->bit_index])
        vec.push_back(&m_components[i][index * desc->size]);
    return vec;
  }

  /*
   * Structural Changes
   */

  void Pool::UpdateStructuralChanges(Entity::Manager &em) noexcept
  {
    const auto old_size = m_size;

    while (m_global_delete_index != invalid_index)
    {
      auto &entry = em.m_entities[m_global_delete_index];
      auto &entity = *std::bit_cast<Ptr<Entity>>(&m_components[0][sizeof(Entity) * entry.pool_index]);
      const auto next_global_delete_index = entity.Version();
      assert(entity.Zombie()); // Entity must be dead

      if (Free(entry.pool_index, true))
        em.Delete(m_global_delete_index, entity);
      else
        em.Delete(m_global_delete_index);
      m_global_delete_index = next_global_delete_index;
    }

    while (m_move_delete_index != invalid_index)
    {
      auto &entity = *std::bit_cast<Ptr<Entity>>(&m_components[0][sizeof(Entity) * m_move_delete_index]);
      const auto next_global_delete_index = entity.Version();
      if (Free(m_move_delete_index, false))
        em.Move(m_move_delete_index, entity);
      m_move_delete_index = next_global_delete_index;
    }

    if (m_size < old_size)
      for (const auto &[i, desc] : Enumerate(m_descriptors))
      {
        const auto page = GetPage(*desc, old_size);
        if (auto current = GetPage(*desc, m_size); current != page || m_size == 0)
        {
          if (m_size == 0)
            --current;
          auto ptr = &m_components[i][Settings::page_size * (current + 1)];
          auto page_count = page - current;
          assert(page_count > 0); // Ensure allocated
          auto b = VirtualFree(ptr, static_cast<u32>(page_count) * Settings::page_size, MEM_DECOMMIT);
          assert(b); // Failed free
        }
      }
    m_current = m_size;
  }

  /*
   * Internals
   */

  bool Pool::Free(i32 index, bool destructor) noexcept
  {
    assert(index >= 0); // Check valid index
    assert(m_size);       // Assert if empty
    if (index >= m_size)
      return false;

    do
    {
      m_current = --m_size;
      if (!std::bit_cast<Ptr<Entity>>(&m_components[0][sizeof(Entity) * m_size])->Zombie())
      {
        if (m_size == index)
        {
          if (destructor)
            for (const auto &[i, desc] : Enumerate(m_descriptors))
            {
              auto data = m_components[i];
              if (desc->destructor)
                desc->destructor(&data[index * desc->size]);
            }
          return false;
        }
        m_current = ++m_size;
        break;
      }

      if (destructor)
        for (const auto &[i, desc] : Enumerate(m_descriptors))
        {
          auto data = m_components[i];
          if (desc->destructor)
            desc->destructor(&data[index * desc->size]);
        }
    } while (m_size);

    if (index < m_size)
    {
      m_current = --m_size;
      for (const auto &[i, desc] : Enumerate(m_descriptors))
      {
        auto data = m_components[i];
        if (desc->move)
        {
          desc->move(&data[index * desc->size], &data[m_size * desc->size]);
          if (destructor && desc->destructor)
            desc->destructor(&data[m_size * desc->size]);
        }
        else
        {
          if (destructor && desc->destructor)
            desc->destructor(&data[index * desc->size]);
          std::memcpy(&data[index * desc->size], &data[m_size * desc->size], desc->size);
        }
      }
      return true;
    }
    return false;
  }
}