#pragma once

#include "../Core/Concepts.h"
#include "Types.h"

namespace XV::ECS::Query
{

  struct Instance final
  {
    Component::Signatures all;
    Component::Signatures any;
    Component::Signatures none;

    Instance() noexcept = default;
    Instance(const Instance &rhs) = default;
    Instance &operator=(const Instance &rhs) = default;

    inline bool Compare(const Component::Signatures &signature) const noexcept;

    inline bool Compare(const Component::Signatures &signature, const Component::Signatures &exclusive) const noexcept;

    template <Concepts::IsAll AllComponents,
              Concepts::IsAny AnyComponents,
              Concepts::IsNone NoneComponents>
    inline Instance &Set() noexcept;

    inline Instance &Set(Instance q) noexcept;

    template <Component::Concepts::IsComponent... Cs>
    inline Instance &AllOf() noexcept;

    template <Component::Concepts::IsComponent... Cs>
    inline Instance &AnyOf() noexcept;

    template <Component::Concepts::IsComponent... Cs>
    inline Instance &NoneOf() noexcept;

    template <Traits::Concepts::CallableFunction F>
    inline void SetQueryFromFunction() noexcept;

    template <Traits::Concepts::CallableFunction F>
    inline void SetQueryFromFunction(F &&) noexcept;

    inline u64 GenerateID() const noexcept;

  private:
    template <ECS::Concepts::ComponentReference C>
    inline void FunctionSet() noexcept;

    template <ECS::Concepts::ComponentPointer C>
    requires(!XV::ECS::Component::Concepts::SingletonComponent<C>) 
    inline void FunctionSet() noexcept;
  };

}