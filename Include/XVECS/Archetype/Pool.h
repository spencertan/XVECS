#pragma once

namespace XV::ECS::Archetype
{
  static constexpr u32 invalid_index = 0xffffffffu >> 1;

  class Pool final
  {

  public:
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
    inline i32 MoveIn(Pool &old_pool, i32 old_index, i32 new_index) noexcept;

    /*
     * Component Accessor
     */
    inline i32 GetComponentIndex(const Component::Descriptor &desc) const noexcept;
    inline i32 GetComponentIndex(const Component::Descriptor &desc, i32 &sequence) const noexcept;
    template <Component::Concepts::IsComponent C>
    inline Decay<C> &GetComponent(i32 index) noexcept;
    template <Component::Concepts::IsComponent C>
    inline Decay<C> &GetComponent(i32 index, i32 &sequence) noexcept;
    inline Ptr<byte> GetComponent(i32 index, const Component::Descriptor &desc) noexcept;
    template <Component::Concepts::IsComponent... Cs>
    inline tuple<Cs &...> GetComponents(i32 index) noexcept;

    inline vector<Ptr<byte>> GetComponents(i32 index, const Component::Signatures &signatures) noexcept;

    /*
     * Structural Changes
     */

    inline void UpdateStructuralChanges(Entity::Manager &em) noexcept;

    using ComponentVector = PinnedVector<Ptr<byte>>;

  private:
    inline bool Free(i32 index, bool destructor) noexcept;
    friend class Archetype::Instance;
    friend class Archetype::Manager;
    Archetype::Instance &m_owner;
    Ptr<Pool> m_pending_changes;
    ComponentVector m_components{};
    Component::DescriptorSpan m_descriptors{};
    i32 m_current{};
    i32 m_size{};
    i32 m_global_delete_index{invalid_index};
    i32 m_move_delete_index{invalid_index};
  };
}