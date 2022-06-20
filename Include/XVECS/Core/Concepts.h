#pragma once

#include "Component/Types.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Traits/Function.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Traits/Tuple.h"

namespace XV::ECS::Concepts
{

  template <typename C>
  concept ComponentReference = XV::Concepts::IsReference<C> && Component::Concepts::IsDataComponent<C>;

  template <typename C>
  concept ComponentPointer = XV::Concepts::IsPointer<C> && Component::Concepts::IsDataComponent<C>;

  template <typename Func>
  concept SystemCallback = (std::is_same_v<typename Traits::Function::Type<Func>::ReturnType, void> ||
                            std::is_same_v<typename Traits::Function::Type<Func>::ReturnType, bool>)&&[]<Component::Concepts::IsDataComponent... Cs>(tuple<Cs...> *) constexpr noexcept
  {
    return true;
  }
  (Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple>);

  template <typename Func>
  concept SetterCallback = Traits::Concepts::IsFunctionReturnType<Func, void> &&
      []<Component::Concepts::IsDataComponent... Cs>(tuplet::tuple<Cs...> *) constexpr noexcept
  {

    static_assert(((std::is_same_v<const Entity&, Cs> || (std::is_same_v<Cs, std::remove_const_t<Cs>>)) && ...), "No const in parameters allowed");
    static_assert(((std::is_reference_v<Cs>)&&...), "Only references is allowed");
    return true;
  }
  (Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple>);

}