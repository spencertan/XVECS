#pragma once

#include "Dependencies.h"
#include "../Component/Types.h"

namespace XV::ECS::Concepts
{

  template <typename Func>
  concept SystemCallback =
      (std::is_same_v<typename Traits::Function::Type<Func>::ReturnType, void> ||
       std::is_same_v<typename Traits::Function::Type<Func>::ReturnType, bool>)&&
       []<Component::Concepts::IsDataComponent... Cs>(tuple<Cs...> *) constexpr noexcept
  {
    return true;
  }
  (Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple>);

  template <typename Func>
  concept SetterCallback =
      Traits::Concepts::IsFunctionReturnType<Func, void> &&
      []<Component::Concepts::IsDataComponent... Cs>(tuple<Cs...> *) constexpr noexcept
  {

    static_assert(((std::is_same_v<Cs, std::remove_const_t<Cs>>) && ...), "No const in parameters allowed");
    static_assert(((std::is_reference_v<Cs>)&&...), "Only references is allowed");
    return true;
  }
  (Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple>);

  template <typename Func>
  concept RefOnlyParameters = 
  []<Component::Concepts::IsDataComponent... Cs>(tuple<Cs...> *) constexpr noexcept
  {
    static_assert(((std::is_reference_v<Cs>) && ...), "Only references is allowed");
    return true;
  }
  (Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple>);

  template <typename Func>
  concept RefPtrParameters = 
  []<Component::Concepts::IsDataComponent... Cs>(tuple<Cs...> *) constexpr noexcept
  {
    static_assert(((std::is_pointer_v<Cs>) || ...), "At least one pointer");
    return true;
  }
  (Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple>);

  template <typename Func>
  concept VoidFuncRefOnly =
      Traits::Concepts::IsFunctionReturnType<Func, void> &&
      RefOnlyParameters<Func>;

  template <typename Func>
  concept VoidFuncRefPtr =
      Traits::Concepts::IsFunctionReturnType<Func, void> &&
      RefPtrParameters<Func>;

  template <typename Func>
  concept BoolFuncRefOnly =
      Traits::Concepts::IsFunctionReturnType<Func, bool> &&
      RefOnlyParameters<Func>;

  template <typename Func>
  concept BoolFuncRefPtr =
      Traits::Concepts::IsFunctionReturnType<Func, bool> &&
      RefPtrParameters<Func>;

}