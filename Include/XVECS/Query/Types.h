#pragma once

namespace XV::ECS::Query
{

  template <Component::Concepts::IsComponent... Cs>
  struct All final
  {
  };

  template <Component::Concepts::IsComponent... Cs>
  struct Any final
  {
  };

  template <Component::Concepts::IsComponent... Cs>
  struct None final
  {
  };

}

namespace XV::ECS::Query::Concepts
{

  template <typename T>
  concept IsAll = Traits::IsSpecialised<All, T>();

  template <typename T>
  concept IsAny = Traits::IsSpecialised<Any, T>();

  template <typename T>
  concept IsNone = Traits::IsSpecialised<None, T>();

}