#pragma once

#include "../../../dep/XV_Utilities/Include/Utilities/STL_C/Vector.h"
#include "../../../dep/XV_Utilities/Include/Utilities/STL_C/Tuple.h"
#include "../../../dep/XV_Utilities/Include/Utilities/Containers/PinnedVector.h"

#include "FWD.h"
#include "../Component/Descriptor.h"
#include "../Entity/Entity.h"
namespace XV::ECS::Archetype
{
  static constexpr u32 invalid_index = 0xffffffffu >> 1;

  struct Pool final
  {
    /*
     * Constructor / Destructor
     */

    inline Pool(Archetype::Instance &owner, Component::DescriptorSpan descriptors) noexcept;
    inline ~Pool() noexcept;

    /*
     * Modifiers
     */

    inline i32 Size() const noexcept;
    inline i32 Append(u32 count) noexcept;
    inline void Delete(i32) noexcept;
    inline void Clear() noexcept;
    inline i32 MoveIn(Instance &old_pool, i32 old_index, i32 new_index) noexcept;

    /*
     * Component Accessor
     */
    inline i32 ComponentIndex(const Component::Descriptor &desc) const noexcept;
    inline i32 ComponentIndex(const Component::Descriptor &desc, i32 &sequence) const noexcept;
    template <Component::Concepts::IsComponent C>
    inline Decay<C> &Component(i32 index) noexcept;
    template <Component::Concepts::IsComponent C>
    inline Decay<C> &Component(i32 index, i32 &sequence) noexcept;
    inline Ptr<byte> Component(i32 index, const Component::Descriptor &desc) noexcept;
    template <Component::Concepts::IsComponent... Cs>
    inline tuple<Cs &...> Components(i32 index) noexcept;

    inline vector<Ptr<byte>> Components(i32 index, const Component::Signatures &signatures) noexcept;

    /*
     * Structural Changes
     */

    inline void UpdateStructuralChanges(Entity::Manager &em) noexcept;

    using ComponentVector = PinnedVector<Ptr<byte>>;
    Archetype::Instance &owner;
    ComponentVector components{};
    Component::DescriptorSpan descriptors{};
    Ptr<Pool> pending_changes{nullptr};
    i32 current{};
    i32 size{};
    i32 global_delete_index{invalid_index};
    i32 move_delete_index{invalid_index};

  private:
    inline bool Free(i32 index, bool destructor) noexcept;
  };
}