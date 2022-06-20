#pragma once

#include "../../../Dep/XV_Utilities/Include/Utilities/STL_C/UnorderedMap.h"
#include "../../../Dep/XV_Utilities/Include/Utilities/Helper/Singleton.h"

#include "Descriptor.h"

namespace XV::ECS::Component
{

  struct Collection final : Singleton<Collection>
  {
    using DescriptorMap = unordered_map<Component::ID, Ptr<const Descriptor>>;
    using DescriptorQuery = vector<Ptr<const Descriptor>>;

    template <Concepts::IsComponent C>
    inline void Register();

    inline void Lock() noexcept;
    inline bool IsLocked() const noexcept;

    inline DescriptorQuery Query(const Signatures &signature) const noexcept;

    inline u32 CollectionCount() const noexcept;

    inline const Signatures &DataSignatures() const noexcept;
    inline const Signatures &TagSignatures() const noexcept;
    inline const Signatures &ExclusiveSignatures() const noexcept;
    inline const Signatures &SingletonSignatures() const noexcept;

    inline u32 CountData(const Signatures &signature) const noexcept;
    inline u32 CountTag(const Signatures &signature) const noexcept;
    inline u32 CountExclusive(const Signatures &signature) const noexcept;
    inline u32 CountSingleton(const Signatures &signature) const noexcept;

    inline Ptr<const Descriptor> Get(const Component::ID &id) const noexcept;
    inline Descriptors GenerateDescriptors(const Signatures& signatures) const noexcept;

  private:
    friend class Singleton<Collection>;
    Collection() = default;
    ~Collection() = default;

    Descriptors m_collection{};
    DescriptorMap m_map;
    Signatures m_data_bits;
    Signatures m_tag_bits;
    Signatures m_exclusive_bits;
    Signatures m_singleton_bits;
    bool m_lock;
  };

}