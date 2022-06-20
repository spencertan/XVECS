#include "../../../dep/XV_Utilities/Include/Utilities/Enumerate.h"

namespace XV::ECS::Archetype
{

  inline int GetPage(const Component::Descriptor &info, i32 entity) noexcept
  {
    return ((entity * static_cast<i32>(info.size)) - 1) / Settings::page_size;
  }

  /*
   * Constructor / Destructor
   */

  void Pool::Instance(Archetype::Instance &archetype_owner, Component::DescriptorSpan data_descriptors) noexcept
  : owner(archetype_owner),
    descriptors(data_descriptors)
  {
    for (const auto &desc : descriptors)
    {
      assert(desc->size <= Settings::page_size); // Assert component size
      auto current_page = GetPage(*desc, Settings::max_entities) + 1;
      components.emplace_back(std::bit_cast<Ptr<byte>>(VirtualAlloc(nullptr, current_page * Settings::page_size, MEM_RESERVE, PAGE_NOACCESS)));
      assert(components.back()); // Assert component added
    }
  }

  Pool::~Pool() noexcept
  {
    Clear();
    for (auto &component : components)
      VirtualFree(component, 0, MEM_RELEASE);
  }

  /*
   * Modifiers
   */

  i32 Pool::Size() const noexcept
  {
    return current;
  }

  i32 Pool::Append(u32 count) noexcept
  {
    for (const auto &[i, desc] : Enumerate(descriptors))
    {
      auto next_page = GetPage(*desc, size + count);
      if (i32 current_page = GetPage(*desc, size); size == 0 || current_page != next_page)
      {
        if (size == 0)
          --current_page;
        auto page_ptr = components[i] + Settings::page_size * (current_page + 1);
        auto ptr = std::bit_cast<Ptr<byte>>(VirtualAlloc(page_ptr, (next_page - current_page) * Settings::page_size, MEM_COMMIT, PAGE_READWRITE));
        assert(ptr == page_ptr); // Check memory
      }

      if (desc->constructor)
      {
        auto ptr = &components[i][size * desc->size];
        for (auto j = count; j; --j)
        {
          desc->constructor(ptr);
          ptr += desc->size;
        }
      }
    }
    i32 index{size};
    size += count;
    current = size;
    return index;
  }

  void Pool::Delete(i32 index) noexcept
  {
    auto &entity = Component<Entity>(index);
    assert(entity.Zombie()); // Entity is alive
    entity.Version(global_delete_index);
    global_delete_index = entity.GlobalIndex();
  }

  void Pool::Clear() noexcept
  {
    // Simple lazy delete
    while (size)
      Free(size - 1, true);
  }

  i32 Pool::MoveIn(Instance &old_pool, i32 old_index, i32 new_index) noexcept
  {
    i32 src_index = 0;
    i32 dest_index = 0;
    const i32 src_count = static_cast<i32>(old_pool.descriptors.size());
    const i32 dest_count = static_cast<i32>(descriptors.size());
    while (true)
      if (old_pool.descriptors[src_index] == descriptors[dest_index])
      {
        auto &desc = *old_pool.descriptors[src_index];
        if (desc.move)
          desc.move(&components[dest_index][new_index * desc.size],
                    &old_pool.components[src_index][old_index * desc.size]);
        else
          std::memcpy(&components[dest_index][new_index * desc.size],
                      &old_pool.components[src_index][old_index * desc.size],
                      desc.size);

        if (desc.destructor)
          desc.destructor(&old_pool.components[src_index][desc.size * old_index]);

        ++src_index;
        ++dest_index;
        if (src_index >= src_count || dest_index >= dest_count)
          break;
      }
      else if (old_pool.descriptors[src_index]->id.value < descriptors[dest_index]->id.value)
      {
        auto &desc = *old_pool.descriptors[src_index];
        if (desc.destructor)
          desc.destructor(&old_pool.components[src_index][desc.size * old_index]);
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
      auto &desc = *old_pool.descriptors[src_index];
      if (desc.destructor)
        desc.destructor(&old_pool.components[src_index][desc.size * old_index]);
      ++src_index;
    }

    //
    // Put the deleted entity into the move deleted linklist
    //
    {
      auto &entity = old_pool.Component<Entity>(old_index);
      entity.Zombie(true);
      entity.Version(old_pool.move_delete_index);
      old_pool.move_delete_index = old_index;
    }

    return new_index;
  }

  i32 Pool::ComponentIndex(const Component::Descriptor &descriptor) const noexcept
  {
    for (const auto &[i, desc] : Enumerate(descriptors))
      if (&descriptor == desc) [[likely]]
        return static_cast<i32>(i);
      else if (Component::CompareDescriptors(&descriptor, desc))
        return -1;
    return -1;
  }

  i32 Pool::ComponentIndex(const Component::Descriptor &descriptor, i32 &sequence) const noexcept
  {
    const auto tmp = sequence;
    for (const auto end = static_cast<i32>(descriptors.size()); sequence < end; ++sequence)
      if (&descriptor == descriptors[sequence]) [[likely]]
        return sequence++;
      else if (Component::CompareDescriptors(&descriptor, descriptors[sequence]))
        break;
    sequence = tmp;
    return -1;
  }

  template <Component::Concepts::IsComponent C>
  Decay<C> &Pool::Component(i32 index) noexcept
  {
    using RealC = Decay<C>;
    if constexpr (std::is_same_v<Entity, RealC>)
      return *std::bit_cast<RealC *>(&components[0][index * sizeof(RealC)]);
    else
    {
      const auto i = ComponentIndex(Component::info<RealC>);
      return *std::bit_cast<RealC *>(&components[i][index * sizeof(RealC)]);
    }
  }

  template <Component::Concepts::IsComponent C>
  Decay<C> &Pool::Component(i32 index, i32 &sequence) noexcept
  {
    using RealC = Raw<C>;
    if constexpr (std::is_same_v<Entity, RealC>)
    {
      assert(sequence == 0);
      ++sequence;
      return *std::bit_cast<RealC *>(&components[0][index * sizeof(RealC)]);
    }
    else
    {
      const auto i = ComponentIndex(Component::info<RealC>, sequence);
      return *std::bit_cast<RealC *>(&components[i][index * sizeof(RealC)]);
    }
  }

  Ptr<byte> Pool::Component(i32 index, const Component::Descriptor &descriptor) noexcept
  {
    const auto i = ComponentIndex(descriptor);
    return &components[i][index * descriptor.size];
  }

  template <Component::Concepts::IsComponent... Cs>
  tuplet::tuple<Cs &...> Pool::Components(i32 index) noexcept
  {
    return tuplet::tie(Component<Cs>(index)...);
  }

  vector<Ptr<byte>> Pool::Components(i32 index, const Component::Signatures &signatures) noexcept
  {
    vector<Ptr<byte>> vec;
    for (const auto &[i, desc] : Enumerate(descriptors))
      if (signatures[desc->bit_index])
        vec.push_back(&components[i][index * desc->size]);
    return vec;
  }

  /*
   * Structural Changes
   */

  void Pool::UpdateStructuralChanges(Entity::Manager &em) noexcept
  {
    const auto old_size = size;

    while (global_delete_index != invalid_index)
    {
      auto &entry = em.entities[global_delete_index];
      auto &entity = *std::bit_cast<Ptr<Entity>>(&components[0][sizeof(Entity) * entry.pool_index]);
      const auto next_global_delete_index = entity.Version();
      assert(entity.Zombie()); // Entity must be dead

      if (Free(entry.pool_index, true))
        em.Delete(global_delete_index, entity);
      else
        em.Delete(global_delete_index);
      global_delete_index = next_global_delete_index;
    }

    while (move_delete_index != invalid_index)
    {
      auto &entity = *std::bit_cast<Ptr<Entity>>(&components[0][sizeof(Entity) * move_delete_index]);
      const auto next_global_delete_index = entity.Version();
      if (Free(move_delete_index, false))
        em.Move(move_delete_index, entity);
      move_delete_index = next_global_delete_index;
    }

    if (size < old_size)
      for (const auto &[i, desc] : Enumerate(descriptors))
      {
        const auto page = GetPage(*desc, old_size);
        if (auto current = GetPage(*desc, size); current != page || size == 0)
        {
          if (size == 0)
            --current;
          auto ptr = &components[i][Settings::page_size * (current + 1)];
          auto page_count = page - current;
          assert(page_count > 0); // Ensure allocated
          auto b = VirtualFree(ptr, static_cast<u32>(page_count) * Settings::page_size, MEM_DECOMMIT);
          assert(b); // Failed free
        }
      }
    current = size;
  }

  /*
   * Internals
   */

  bool Pool::Free(i32 index, bool destructor) noexcept
  {
    assert(index >= 0); // Check valid index
    assert(size);       // Assert if empty
    if (index >= size)
      return false;

    do
    {
      current = --size;
      if (!std::bit_cast<Ptr<Entity>>(&components[0][sizeof(Entity) * size])->Zombie())
      {
        if (size == index)
        {
          if (destructor)
            for (const auto &[i, desc] : Enumerate(descriptors))
            {
              auto data = components[i];
              if (desc->destructor)
                desc->destructor(&data[index * desc->size]);
            }
          return false;
        }
        current = ++size;
        break;
      }

      if (destructor)
        for (const auto &[i, desc] : Enumerate(descriptors))
        {
          auto data = components[i];
          if (desc->destructor)
            desc->destructor(&data[index * desc->size]);
        }
    } while (size);

    if (index < size)
    {
      current = --size;
      for (const auto &[i, desc] : Enumerate(descriptors))
      {
        auto data = components[i];
        if (desc->move)
        {
          desc->move(&data[index * desc->size], &data[size * desc->size]);
          if (destructor && desc->destructor)
            desc->destructor(&data[size * desc->size]);
        }
        else
        {
          if (destructor && desc->destructor)
            desc->destructor(&data[index * desc->size]);
          std::memcpy(&data[index * desc->size], &data[size * desc->size], desc->size);
        }
      }
      return true;
    }
    return false;
  }
}