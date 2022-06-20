#pragma once

#include "../Query/Types.h"
#include "Types.h"

namespace XV::ECS::System
{
  struct Overrides
{
  static constexpr auto info = Type::Update {};
  using All = Query::All<>;
  using Any = Query::Any<>;
  using None = Query::None<>;

  using Events = tuplet::tuple<>;

  inline Overrides( World::Instance &world ) noexcept : world( world )
  {}

  void OnCreate() noexcept
  {}
  void OnStart() noexcept
  {}
  void OnFrameStart() noexcept
  {}
  void OnPreUpdate() noexcept
  {}
  void OnUpdate() noexcept
  {}
  void OnPostUpdate() noexcept
  {}
  void OnPostStructuralChanges() noexcept
  {}
  void OnFrameEnd() noexcept
  {}
  void OnEnd() noexcept
  {}
  void OnDestroy() noexcept
  {}
  void OnPause() noexcept
  {
    paused = true;
  }
  void OnResume() noexcept
  {
    paused = false;
  }
  void OnEvent( ... ) noexcept
  {}
  void OnNotify( Entity & ) noexcept
  {}

  World::Instance &world;
  bool paused { true };

};
}