#pragma once

/**
 * @brief
 * * World Declaration
 */

namespace Xivi::ECS::World
{
  struct Instance;
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

  using ID = GUID<struct ComponentID>;

  namespace Descriptor
  {
    struct Instance;
    struct Manager;
  }

  namespace Type
  {
    struct Tag;
    struct Data;
  }
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
  struct Instance;
  struct Manager;

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
  struct Instance;
  struct Manager;
}

/**
 * @brief
 * * Pool Declaration
 */

namespace XV::ECS::Pool
{
  using ID = GUID<struct PoolID>;
  struct Instance;
  struct Index
  {
    i32 m_value{0};
    auto operator<=>(const Index &) const = default;
  };
}

/**
 * @brief
 * * Family Declaration
 */

namespace XV::ECS::Pool::Family
{
  using ID = GUID<struct FamilyID>;
  struct Instance;
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
  struct Instance;
}

/**
 * @brief
 * * Core Declaration
 */
namespace XV::ECS
{
  struct Entity;
  struct ReferenceCount;

  using Signature = XV::CBitset<Settings::max_components>;
  using DescriptorSpan = span<const Ptr<const Component::Descriptor::Instance>>;
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