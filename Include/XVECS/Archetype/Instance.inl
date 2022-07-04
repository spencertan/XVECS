namespace XV::ECS::Archetype
{

  /*
  * Private Method
  */ 

  template <ECS::Concepts::ComponentReference... Cs>
  auto Instance::GetComponentPointerArray(const i32 index, tuple<Cs...> *) noexcept
  {
    assert(((m_pool.ComponentIndex(Component::info<Cs>) >= 0) && ...));
    using DataTuple = tuple<Cs...>;
    using SortedTuple = SortComponentsTuple<DataTuple>;

    array<Ptr<byte>, sizeof...(Cs)> cached_pointers;
    [&]<typename... SortedCs>(tuple<SortedCs...> *) constexpr noexcept
    {
      i32 sequence = 0;
      ((cached_pointers[Traits::Tuple::Index<SortedCs, DataTuple>] = &m_pool.m_components[m_pool.ComponentIndex(Component::info<SortedCs>, sequence)][sizeof(Decay<SortedCs>) * index]), ...);
    }
    (Traits::Tuple::null<SortedTuple>);
    return cached_pointers;
  }

  template <typename... Cs>
  requires(ECS::Concepts::ComponentPointer<Cs> || ...) auto Instance::GetComponentPointerArray(const i32 index, tuple<Cs...> *) noexcept
  {
    using DataTuple = tuple<Cs...>;
    using SortedTuple = SortComponentsTuple<DataTuple>;

    array<Ptr<byte>, sizeof...(Cs)> cached_pointers;
    [&]<typename... SortedCs>(tuple<SortedCs...> *) constexpr noexcept
    {
      i32 sequence = 0;
      ((cached_pointers[Traits::Tuple::Index<SortedCs, DataTuple>] = [&]<typename C>(tuple<C> *) constexpr noexcept {
         const auto i = m_pool.ComponentIndex(Component::info<C>, sequence);
         if constexpr (std::is_pointer_v<C>)
           return (i < 0) ? nullptr : &m_pool.m_components[i][sizeof(Decay<C>) * index];
         else
           return &m_pool.m_components[i][sizeof(Decay<C>) * index];
       }(Traits::Tuple::null<tuple<SortedCs>>)),
       ...);
    }
    (Traits::Tuple::null<SortedTuple>);
    return cached_pointers;
  }

  template <Concepts::BoolFuncRefOnly Func, typename Data>
  bool Instance::Invoke(Func &&func, Data &cached_pointers) noexcept
  {
    using ArgsTuple = Traits::Function::Type<Func>::ArgsTuple;
    return [&]<typename... Cs>(tuple<Cs...> *) constexpr noexcept
    {
      if (func(reinterpret_cast<Cs>(*cached_pointers[Traits::Tuple::Index<Cs, ArgsTuple>]), ...))
        return true;
      ((cached_pointers[Traits::Tuple::Index<Cs, ArgsTuple>] += sizeof(Decay<Cs>)), ...);
      return false;
    }
    (Traits::Tuple::null<ArgsTuple>);
  }

  template <Concepts::VoidFuncRefOnly Func, typename Data>
  void Instance::Invoke(Func &&func, Data &cached_pointers) noexcept
  {
    using ArgsTuple = Traits::Function::Type<Func>::ArgsTuple;
    return [&]<typename... Cs>(tuple<Cs...> *) constexpr noexcept
    {
      func(reinterpret_cast<Cs>(*cached_pointers[Traits::Tuple::Index<Cs, ArgsTuple>]), ...);
      ((cached_pointers[Traits::Tuple::Index<Cs, ArgsTuple>] += sizeof(Decay<Cs>)), ...);
    }
    (Traits::Tuple::null<ArgsTuple>);
  }

  template <Concepts::BoolFuncRefPtr Func, typename Data>
  bool Instance::Invoke(Func &&func, Data &cached_pointers) noexcept
  {
    using ArgsTuple = Traits::Function::Type<Func>::ArgsTuple;
    return [&]<typename... Cs>(tuple<Cs...>*) constexpr noexcept
    {
      return func([&] <typename C>(tuple<C>*) constexpr noexcept -> C
      {
        auto& ptr = cached_pointers[Traits::Tuple::Index<C, ArgsTuple>];
        if constexpr(std::is_pointer_v<C>)
          if (!ptr) return std::bit_cast<C>(nullptr);
        
        auto c_ptr = ptr;
        ptr += sizeof(Decay<C>);

        if constexpr ( std::is_pointer_v<C> ) return std::bit_cast<C>( c_ptr );
        else return reinterpret_cast<C>( *c_ptr );
      }(Traits::Tuple::make_null<C>),...);
    }(Traits::Tuple::null<ArgsTuple>);
  }

  template <Concepts::VoidFuncRefPtr Func, typename Data>
  void Instance::Invoke(Func &&func, Data &cached_pointers) noexcept
  {
    using ArgsTuple = Traits::Function::Type<Func>::ArgsTuple;
    [&]<typename... Cs>(tuple<Cs...>*) constexpr noexcept
    {
      func([&] <typename C>(tuple<C>*) constexpr noexcept -> C
      {
        auto& ptr = cached_pointers[Traits::Tuple::Index<C, ArgsTuple>];
        if constexpr(std::is_pointer_v<C>)
          if (!ptr) return std::bit_cast<C>(nullptr);
        
        auto c_ptr = ptr;
        ptr += sizeof(Decay<C>);

        if constexpr ( std::is_pointer_v<C> ) return std::bit_cast<C>( c_ptr );
        else return reinterpret_cast<C>( *c_ptr );
      }(Traits::Tuple::make_null<C>),...);
    }(Traits::Tuple::null<ArgsTuple>);
  }


  Archetype::ID GenerateID(const Component::Signatures &signatures) noexcept
  {
    return Hash::XXH3_64(signatures);
  }

  Instance::Instance(Manager &manager, Archetype::ID id, const Component::Signatures &signatures)
      : m_manager(manager),
        m_id(id),
        m_descriptors(Component::Collection::Instance().GenerateDescriptors(signatures)),
        m_signatures(signatures),
        m_exclusive(signatures & Component::Collection::Instance().ExclusiveSignatures()),
        m_data_count(Component::Collection::Instance().CountData(signatures)),
        m_tag_count(Component::Collection::Instance().CountTag(signatures)),
        m_exclusive_count(Component::Collection::Instance().CountExclusive(signatures)),
        m_pool(*this, {m_descriptors.data(), m_data_count})
  {
  }

  Archetype::ID Instance::GetID() const noexcept
  {
    return m_id;
  }
  const Component::Signatures &Instance::GetSignatures() const noexcept
  {
    return m_signatures;
  }
  const Component::Signatures &Instance::GetExclusiveSignatures() const noexcept
  {
    return m_exclusive;
  }

  template <Component::Concepts::IsComponent C>
  bool Instance::HaveComponent() const noexcept
  {
    return m_signatures[Component::info<C>.bit_index];
  }
  bool Instance::HaveComponent(const Component::Descriptor &descriptor) const noexcept
  {
    return m_signatures[descriptor.bit_index];
  }

  template <Component::Concepts::IsComponent... Cs>
  bool Instance::HaveComponents() const noexcept
  {
    return (m_signature[Component::info<Cs>.bit_index] && ...);
  }
  bool Instance::HaveComponents(Component::DescriptorSpan descriptors) const noexcept
  {
    for (auto &desc : descriptors)
      if (!m_signatures[desc->bit_index])
        return false;
    return true;
  }

  Entity Instance::CreateEntity(DescriptorSpan descriptors, DataSpan datas) noexcept
  {
    return CreateEntities(1, descriptors, datas).front();
  }

  template <ECS::Concepts::SetterCallback Func>
  Entity Instance::CreateEntity(Func &&func) noexcept
  {
    return CreateEntities(1, std::forward<Func &&>(func)).front();
  }

  vector<Entity> Instance::CreateEntities(u32 count, DescriptorSpan descriptors, DataSpan datas) noexcept
  {
    assert(HaveComponents(descriptors));
    assert(!datas.empty());
    auto index = m_pool.Append(count);
    vector<Entity> entities;
    auto create = [&]<bool Notify>(std::bool_constant<Notify>)
    {
      for (auto i = 0; i < count; ++i)
      {
        auto current_index = index + i;
        auto &entity = m_pool.GetComponent<Entity>(current_index) = m_manager.GetWorld().GetEntityManager().Allocate(*this, m_pool, current_index);
        entities.push_back(entity);
        for (auto i = 0; i < descriptors.size(); ++i)
        {
          auto &desc = *descriptors[i];
          auto c_index = m_pool.GetComponentIndex(desc);
          assert(c_index >= 0);

          if (datas[i])
          {
            if (desc.move)
              desc.move(&m_pool.m_components[c_index][current_index * desc.size], datas[i]);
            else
              std::memmove(&m_pool.m_components[c_index][current_index * desc.size], datas[i], desc.size);
          }
        }

        if constexpr (Notify)
          m_events.on_entity_created.Notify(entity);
      }
    };

    if (m_events.on_entity_created.delegates.size())
      create(std::bool_constant<true>{});
    else
      create(std::bool_constant<false>{});

    return entities;
  }

  template <ECS::Concepts::SetterCallback Func>
  vector<Entity> Instance::CreateEntities(u32 count, Func &&func) noexcept
  {

    auto index = m_pool.Append(count);
    auto create = [&]<bool Notify>(std::bool_constant<Notify>)
    {
      if constexpr (std::is_same_v<EmptyLambda, Func>)
      {
        for (auto i = 0u; i < count; ++i)
        {
          auto current_index = index + i;
          auto &entity = m_pool.Component<Entity>(current_index) = m_manager.m_world.m_entity_manager.Allocate(*this, m_pool, current_index);
          entities.push_back(entity);
          if constexpr (Notify)
            m_events.on_entity_created.Notify();
        }
      }
      else
      {
        ArgsTuple = Traits::Function::Type<Func>::ArgsTuple;
        auto cached_pointers = GetComponentPointerArray(index, Traits::Tuple::null<ArgsTuple>);

        for (auto i = 0u; i < count; ++i)
        {
          auto current_index = index + i;
          auto &entity = 
            *std::bit_cast<Ptr<Entity>>(cached_pointers[Traits::Tuple::Index<Entity,ArgsTuple]) = 
               m_manager.m_world.m_entity_manager.Allocate(*this, m_pool, current_index);
          entities.push_back(entity);
          Invoke(func,cached_pointers);

          if constexpr (Notify)
            m_events.on_entity_created.Notify();
        }
      }
    };

    if (m_events.on_entity_created.delegates.size())
      create(std::bool_constant<true>{});
    else
      create(std::bool_constant<false>{});

    return entities;
  }





}