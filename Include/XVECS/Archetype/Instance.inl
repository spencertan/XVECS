#include "Instance.h"
namespace XV::ECS::Archetype
{

  Archetype::ID GenerateID(const Component::Signatures &signatures) noexcept
  {
    return Hash::XXH3_64(signatures);
  }

  Instance::Instance(Manager &manager, Archetype::ID id, const Component::Signatures &signatures)
      : m_manager(manager),
        m_id(id),
        m_descriptors(Component::Collection::Instance().GenerateDescriptors(signatures)),
        m_signatures(signatures),
        m_exclusive(signatures & Component::Collection::Instance().ExclusiveSignature()),
        m_data_count(Component::Collection::Instance().CountData(signatures)),
        m_tag_count(Component::Collection::Instance().CountTag(signatures)),
        m_exclusive_count(Component::Collection::Instance().CountExclusive(signatures)),
        m_pool(*this, {m_descriptors.data(), m_data_count})
  {
  }

  Archetype::ID Instance::ID() const noexcept
  {
    return m_id;
  }
  const Component::Signatures &Instance::Signatures() const noexcept
  {
    return m_signatures;
  }
  const Component::Signatures &Instance::ExclusiveSignatures() const noexcept
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

}