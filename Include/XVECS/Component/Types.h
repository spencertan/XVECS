#pragma once


/**
 * @brief
 * * Component Types
 */

namespace XV::ECS::Component::Type
{

  namespace Details
  {
    struct Base
    {
      Component::ID id;
      std::string_view name{};
    };
  }

  enum class ID : u8
  {
    Data,
    Tag,
    Exclusive,
    Singleton
  };

  struct Data final : Details::Base
  {
    static constexpr auto type = ID::Data;
    static constexpr auto max_size = Settings::page_size;
  };

  struct Tag final : Details::Base
  {
    static constexpr auto type = ID::Tag;
    static constexpr auto max_size = 1ull;
  };

  struct Exclusive final : Details::Base
  {
    static constexpr auto type = ID::Exclusive;
    static constexpr auto max_size = 1ull;
  };

  struct Singleton final : Details::Base
  {
    static constexpr auto type = ID::Singleton;
  };
}

/**
 * @brief
 * * Component Type Concepts
 */

namespace XV::ECS::Component::Concepts
{

  template <typename C>
  concept DataComponent = Decay<C>::info.type == Type::ID::Data &&
                          sizeof(Decay<C>) <= Type::Data::max_size;

  template <typename C>
  concept TagComponent = Decay<C>::info.type == Type::ID::Tag &&
                         sizeof(Decay<C>) <= Type::Tag::max_size;

  template <typename C>
  concept ExclusiveTagComponent = Decay<C>::info.type == Type::ID::Exclusive &&
                                  sizeof(Decay<C>) <= Type::Tag::max_size;

  template <typename C>
  concept SingletonComponent = Decay<C>::info.type == Type::ID::Singleton;

  template <typename C>
  concept IsDataComponent =
      DataComponent<C> ||
      SingletonComponent<C>;

  template <typename C>
  concept IsComponent =
      DataComponent<C> ||
      TagComponent<C> ||
      ExclusiveTagComponent<C> ||
      SingletonComponent<C>;

  template <typename List>
  concept IsComponentList = IsTypelist<List> &&
      []<IsComponent... Cs>(Typelist<Cs...>) constexpr noexcept
  {
    return true;
  }
  (List());

}

namespace XV::ECS::Concepts
{
  template <typename C>
  concept ComponentReference = XV::Concepts::IsReference<C> && Component::Concepts::IsDataComponent<C>;

  template <typename C>
  concept ComponentPointer = XV::Concepts::IsPointer<C> && Component::Concepts::IsDataComponent<C>;

}