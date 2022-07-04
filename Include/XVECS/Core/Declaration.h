#pragma once

/**
 * @brief
 * * World Declaration
 */

namespace XV::ECS::World
{
  class Instance;
}

/**
 * @brief
 * * Components Declaration
 */

namespace XV::ECS::Component
{

  namespace Details
  {
    struct ComponentID
    {
    };
  }



  namespace Type
  {
    struct Data;
    struct Tag;
    struct Exclusive;
    struct Singleton;
  }

  struct Descriptor;
  class Collection;

  using ID = GUID<struct ComponentID>;
  using Descriptors= vector<Ptr<const Descriptor>>;

}

namespace XV::ECS::Component
{
  using Signatures = XV::CBitset<Settings::max_components>;
  using DescriptorSpan = span<const Ptr<const Component::Descriptor>>;
}

/**
 * @brief
 * * System Declaration
 */

namespace XV::ECS::System
{
  namespace Details
  {
    struct SystemID
    {
    };
  }

  using ID = GUID<struct SystemID>;
  class Instance;
  class Manager;

  namespace Concepts
  {
    template <typename TSystem>
    concept IsSystem = std::derived_from<TSystem, Instance>;
  }

  namespace Details
  {
    template <Concepts::IsSystem TSystem>
    struct Linker;
  }
}

/**
 * @brief
 * * Archetype Declaration
 */

namespace XV::ECS::Archetype
{
  using ID = GUID<struct ArchetypeID>;
  class Instance;
  class Manager;
  class Pool;
}

/**
 * @brief
 * * Events Declaration
 */

namespace XV::ECS::Event
{
  using ID = XV::Event::ID;
  using Global = XV::Event::Type::Global;
  using Overrides = XV::Event::Overrides;
  template <typename... Args>
  using Instance = XV::Event::Instance<Args...>;
  using Descriptor = XV::Event::Descriptor;
  using Manager = XV::Event::Manager;
}

/**
 * @brief
 * * Query Declaration
 */

namespace Xivi::ECS::Query
{
  class Instance;
}

/**
 * @brief
 * * Core Declaration
 */
namespace XV::ECS
{
  union Entity;

  using Signature = XV::CBitset<Settings::max_components>;
  using DescriptorSpan = span<const Ptr<const Component::Descriptor>>;
  using DataSpan = span<Ptr<byte>>;
  using ConstDataSpan = span<const Ptr<const byte>>;
  using EntitySpan = span<Entity>;

  inline i32 ComponentIndexFromSignature(const Signature &signature, i32 index) noexcept
  {
    const i32 y = index % 64;
    i32 x = index / 64;
    i32 count = std::popcount(signature.data()[x] & ((1ull << y) - 1));
    while (x)
      count += std::popcount(signature.data()[--x]);
    return count;
  }
}

namespace XV
{
  template <typename... Ts>
  using CList = Typelist<Ts...>;

  template <typename... Ts>
  using CListAdd = CList<Ts...>;

  template <typename... Ts>
  using CListRemove = CList<Ts...>;

  template <typename... Ts>
  using SList = Typelist<Ts...>;

  template <typename... Ts>
  using EList = Typelist<Ts...>;
}