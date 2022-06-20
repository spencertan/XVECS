#include "Descriptor.h"

namespace XV::ECS::System::Details
{
  template <Concepts::IsSystem TSystem>
  consteval Descriptor CreateDescriptor() noexcept
  {
    Descriptor descriptor;

    using RealSystem = Details::Wrapper<TSystem>;

    if constexpr (TSystem::info.id.Valid())
      descriptor.id = TSystem::info.id;
    else
      descriptor.id = ID(Typename<TSystem>());

    if constexpr (!TSystem::info.is_notifier)
      descriptor.notifier_registration = nullptr;
    else
    {
      descriptor.notifier_registration = [](Archetype::Instance &archetype, System::Instance &system) noexcept
      {
        if constexpr (TSystem::info.type == Type::ID::NotifyCreate)
          archetype.events.on_entity_created.Register<&RealSystem::Notify>(static_cast<RealSystem &>(system));
        else if constexpr (TSystem::info.type == Type::ID::NotifyDestroy)
          archetype.events.on_entity_destroyed.Register<&RealSystem::Notify>(static_cast<RealSystem &>(system));
        else if constexpr (TSystem::info.type == Type::ID::NotifyMoveIn)
          archetype.events.on_entity_moved_in.Register<&RealSystem::Notify>(static_cast<RealSystem &>(system));
        else if constexpr (TSystem::info.type == Type::ID::NotifyMoveOut)
          archetype.events.on_entity_moved_out.Register<&RealSystem::Notify>(static_cast<RealSystem &>(system));
      };
    }

    descriptor.destructor = [](System::Instance &system) noexcept
    {
      std::destroy_at(&static_cast<Details::Wrapper<TSystem> &>(system));
    };

    if constexpr (TSystem::info.name)
      descriptor.name = Typename<TSystem>();
    else
      descriptor.name = TSystem::info.name;

    descriptor.type = TSystem::info.type;

    return descriptor;
  }
}
