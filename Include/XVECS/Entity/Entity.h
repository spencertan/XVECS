#pragma once
#include <bit>
#include "../Component/Types.h"
#include "../Archetype/FWD.h"
namespace XV::ECS
{

  union Entity final
  {

    /**
     * @brief
     * * Entity Validation Field
     */

    union Validation final
    {
      u32 value;
      struct Details
      {
        u32 version : 20,
            r : 11,
            zombie : 1;
      } details;

      u32 Version() const noexcept
      {
        return details.version;
      }

      void IncrementVersion() noexcept
      {
        ++details.version;
      }

      void Version(u32 value) noexcept
      {
        details.version = value;
      }

      bool Zombie() const noexcept
      {
        return details.zombie;
      }

      void Zombie(bool b) noexcept
      {
        details.zombie = b;
      }

      bool operator==(const Validation &rhs) const noexcept
      {
        return value == rhs.value;
      }
    };
    static_assert(sizeof(Validation) == sizeof(u32));

    /**
     * @brief
     * * Entity Data / Manager Declaration
     */

    struct Data;
    struct Manager;

    /**
     * @brief
     * * Entity Implementation
     */

    static constexpr auto info = Component::Type::Data{{.name = "Entity"}};
    static constexpr u64 invalid_entity = ~0ull;

    u64 value;
    struct Details
    {
      u32 global_index{~static_cast<u32>(0)};
      Validation validation{~static_cast<u32>(0)};
    } details;

    Entity(u32 index, Validation validation) noexcept
        : details{index, validation.value}
    {
    }

    u32 GlobalIndex() const noexcept
    {
      return details.global_index;
    }

    u32 GlobalIndex(u32 value) noexcept
    {
      details.global_index = value;
    }

    u32 Version() const noexcept
    {
      return details.validation.Version();
    }

    void Version(u32 value) noexcept
    {
      details.validation.Version(value);
    }

    bool Zombie() const noexcept
    {
      return details.validation.Zombie();
    }

    void Zombie(bool b) noexcept
    {
      details.validation.Zombie(b);
    }

    bool Valid() const noexcept
    {
      return value != invalid_entity;
    }

    bool operator==(const Entity &entity) const noexcept
    {
      return value == entity.value;
    }
  };
  static_assert(sizeof(Entity) == sizeof(u64));

}

namespace std
{

  template <>
  struct hash<XV::ECS::Entity>
  {
    std::size_t operator()(const XV::ECS::Entity &e) const
    {
      return XV::Hash::XXH3_64(e);
    }
  };

}