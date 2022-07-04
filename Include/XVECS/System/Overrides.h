#pragma once

#include "../Query/Types.h"
#include "Types.h"

namespace XV::ECS::System
{
  class Overrides
  {
    static constexpr auto info = Type::Update{};
    using All = Query::All<>;
    using Any = Query::Any<>;
    using None = Query::None<>;

    using Events = tuplet::tuple<>;

  public:
    inline Overrides(World::Instance &world) noexcept : world(world)
    {
    }

    void OnCreate() noexcept
    {
    }
    void OnStart() noexcept
    {
    }
    void OnFrameStart() noexcept
    {
    }
    void OnPreUpdate() noexcept
    {
    }
    void OnUpdate() noexcept
    {
    }
    void OnPostUpdate() noexcept
    {
    }
    void OnPostStructuralChanges() noexcept
    {
    }
    void OnFrameEnd() noexcept
    {
    }
    void OnEnd() noexcept
    {
    }
    void OnDestroy() noexcept
    {
    }
    void OnPause() noexcept
    {
      m_paused = true;
    }
    void OnResume() noexcept
    {
      m_paused = false;
    }
    void OnEvent(...) noexcept
    {
    }
    void OnNotify(Entity &) noexcept
    {
    }

  protected:
    World::Instance &m_world;
    bool m_paused{true};
  };
}