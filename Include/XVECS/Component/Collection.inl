namespace XV::ECS::Component
{

  template <Concepts::IsComponent C>
  void Collection::Register() noexcept
  {
    if (m_lock)
      return;

    if (info<C>.bit_index == Descriptor::invalid_bit_index)
    {
      info<C>.bit_index = Descriptor::processing_bit_index;
      m_collection.emplace_back(&info<C>);
    }
  }

  void Collection::Lock() noexcept
  {
    if (m_lock)
      return;

    m_lock = true;

    std::sort(m_collection.begin(), m_collection.end(), CompareDescriptors);

    for (const auto &[i, desc] : Enumerate(m_collection))
    {
      assert(desc->bit_index == Descriptor::processing_bit_index);

      desc->bit_index = i;
      m_map.emplace(std::pair{desc->id, desc});

      switch (desc->type)
      {

      case Type::ID::Data:
        m_data_bits.set(i);
        break;
      case Type::ID::Exclusive:
        m_exclusive_bits.set(i);
        [[fallthrough]] case Type::ID::Tag : m_tag_bits.set(i);
        break;
      case Type::ID::Singleton:
        m_singleton_bits.set(i);
        break;
      }
    }
  }

  bool Collection::IsLocked() const noexcept
  {
    return m_lock;
  }

  Collection::DescriptorQuery Collection::Query(const Signatures &signature) const noexcept
  {
    DescriptorQuery result;
    for (const auto &desc : m_collection)
      if (signature[desc->bit_index])
        result.push_back(desc);
    return result;
  }

  u32 Collection::CollectionCount() const noexcept
  {
    return m_collection.size();
  }

  const Signatures &Collection::DataSignatures() const noexcept
  {
    return m_data_bits;
  }

  const Signatures &Collection::TagSignatures() const noexcept
  {
    return m_tag_bits;
  }

  const Signatures &Collection::ExclusiveSignatures() const noexcept
  {
    return m_exclusive_bits;
  }

  const Signatures &Collection::SingletonSignatures() const noexcept
  {
    return m_singleton_bits;
  }

  u32 Collection::CountData(const Signatures &signature) const noexcept
  {
    auto sig = signature & m_data_bits;
    return sig.count();
  }

  u32 Collection::CountTag(const Signatures &signature) const noexcept
  {
    auto sig = signature & m_tag_bits;
    return sig.count();
  }

  u32 Collection::CountExclusive(const Signatures &signature) const noexcept
  {
    auto sig = signature & m_exclusive_bits;
    return sig.count();
  }

  u32 Collection::CountSingleton(const Signatures &signature) const noexcept
  {
    auto sig = signature & m_singleton_bits;
    return sig.count();
  }

  Ptr<const Descriptor> Collection::GetDescriptor(const Component::ID &id) const noexcept
  {
    auto it = m_map.find(id);
    if (it != m_map.end())
      return it->second;
    return nullptr;
  }

  Descriptors Collection::GenerateDescriptors(const Signatures& signatures) const noexcept
  {
    Descriptors result;
    for (auto& desc : m_collection)
      if (signatures[desc->bit_index])
        result.push_back(desc);
  return result;
  }


}