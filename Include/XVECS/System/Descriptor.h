#pragma once

#include "Types.h"
#include "../Query/Instance.h"
#include "../Archetype/FWD.h"
namespace XV::ECS::System
{

  struct Descriptor
  {
    using NotifierRegistration = FuncPtr<void(Archetype::Instance &, Instance &) noexcept>;
    using Destructor = FuncPtr<void(System::Instance &) noexcept>;

    ID id;
    mutable ECS::Query::Instance query;
    NotifierRegistration notifier_registration;
    Destructor destructor;
    Type::ID type;
    std::string_view name;
  };

  namespace Details
  {
    template <Concepts::IsSystem TSystem>
    consteval Descriptor CreateDescriptor() noexcept;

    template <typename TSystem>
    inline constexpr auto desc = CreateDescriptor<TSystem>();
  }

  template <typename TSystem>
  inline constexpr auto &info = Details::desc<Decay<TSystem>>;

}