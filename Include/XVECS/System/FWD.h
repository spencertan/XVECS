#pragma once

#include "../../../Dep/XV_Utilities/Include/Utilities/GUID.h"

namespace XV::ECS::System
{
  namespace Details
  {
    struct SystemID
    {
    };
  }

  using ID = GUID<struct SystemID>;
  struct Instance;
  struct Manager;

  namespace Concepts
  {
    template <typename TSystem>
    concept IsSystem = std::derived_from<TSystem, Instance>;
  }

  namespace Details
  {
    template <Concepts::IsSystem TSystem>
    struct Wrapper;
  }
}