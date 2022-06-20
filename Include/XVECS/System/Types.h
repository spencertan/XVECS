#pragma once

#include "FWD.h"
#include "../../../dep/XV_Utilities/Include/Utilities/Events/Instance.h"
namespace XV::ECS::System::Type
{

  namespace Details
  {
    struct Base
    {
      System::ID id;
      std::string_view name;
    };
  }

  enum class ID : u8
  {
    Update,
    SystemEvent,
    GlobalEvent,
    NotifyCreate,
    NotifyDestroy,
    NotifyMoveIn,
    NotifyMoveOut
  };

  struct Update : Details::Base
  {
    static constexpr auto type = ID::Update;
    static constexpr auto is_notifier = false;
  };

  template <System::Concepts::IsSystem TSystem, XV::Event::Concepts::IsEvent TEvent>
  struct ChildUpdate : Details::Base
  {
    using SystemType = TSystem;
    using EventType = TEvent;
    static constexpr auto type = ID::SystemEvent;
    static constexpr auto is_notifier = false;
  };

  template <System::Concepts::IsSystem TSystem, XV::Event::Concepts::IsEvent TEvent>
  struct SystemEvent : Details::Base
  {
    using SystemType = TSystem;
    using EventType = TEvent;
    static constexpr auto type = ID::SystemEvent;
    static constexpr auto is_notifier = false;
  };

  struct NotifyCreate : Details::Base
  {
    static constexpr auto type = ID::NotifyCreate;
    static constexpr auto is_notifier = true;
  };

  struct NotifyDestroy : Details::Base
  {
    static constexpr auto type = ID::NotifyDestroy;
    static constexpr auto is_notifier = true;
  };

  struct NotifyMoveIn : Details::Base
  {
    static constexpr auto type = ID::NotifyMoveIn;
    static constexpr auto is_notifier = true;
  };

  struct NotifyMoveOut : Details::Base
  {
    static constexpr auto type = ID::NotifyMoveOut;
    static constexpr auto is_notifier = true;
  };

  template <XV::Event::Concepts::IsEvent TEvent>
  struct GlobalEvent : Details::Base
  {
    using EventType = TEvent;
    static constexpr auto type = ID::GlobalEvent;
    static constexpr auto is_notifier = false;
  };
}