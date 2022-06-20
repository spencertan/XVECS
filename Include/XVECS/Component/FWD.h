#pragma once

#include "../../../Dep/XV_Utilities/Include/Utilities/GUID.h"
#include "../../../Dep/XV_Utilities/Include/Utilities/Containers/CBitset.h"
#include "../Core/Settings.h"

namespace XV::ECS::Component
{

  namespace Details
  {
    struct ComponentID
    {
    };
  }

  using ID = GUID<struct ComponentID>;

  struct Descriptor;
  struct Collection;

  namespace Type
  {
    struct Data;
    struct Tag;
    struct Exclusive;
    struct Singleton;
  }
}

namespace XV::ECS::Component
{
  using Signatures = XV::CBitset<Settings::max_components>;
  using DescriptorSpan = span<const Ptr<const Component::Descriptor>>;
}