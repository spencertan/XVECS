#pragma once
#include "../Core/Dependencies.h"
#include "../Core/Settings.h"
#include "../Core/Declaration.h"
#include "../Component/Types.h"
#include "../Component/Descriptor.h"
#include "../Component/Collection.h"
#include "../Core/Concepts.h"
#include "../Entity/Entity.h"
#include "Pool.h"
#include "Instance.h"
namespace XV::ECS::Archetype
{

  inline Archetype::ID GenerateID(const Component::Signatures &signatures) noexcept;

  class Instance final
  {

    friend class Manager;

    struct Events
    {
      Event::Instance<Entity &> on_entity_created;
      Event::Instance<Entity &> on_entity_destroyed;
      Event::Instance<Entity &> on_entity_moved_in;
      Event::Instance<Entity &> on_entity_moved_out;
    } m_events;

    Manager &m_manager;
    Archetype::ID m_id;
    Component::Descriptors m_descriptors;
    Component::Signatures m_signatures;
    Component::Signatures m_exclusive;
    u32 m_data_count;
    u32 m_tag_count;
    u32 m_exclusive_count;
    Pool m_pool;

    template <Concepts::ComponentReference... Cs>
    inline auto GetComponentPointerArray(const i32 index, tuple<Cs...> *) noexcept;

    template <typename... Cs>
    requires(Concepts::ComponentPointer<Cs> || ...) inline auto GetComponentPointerArray(const i32 index, tuple<Cs...> *) noexcept;

    template <Concepts::BoolFuncRefOnly Func, typename Data>
    inline bool Invoke(Func &&func, Data &cached_pointers) noexcept;

    template <Concepts::VoidFuncRefOnly Func, typename Data>
    inline void Invoke(Func &&func, Data &cached_pointers) noexcept;

    template <Concepts::BoolFuncRefPtr Func, typename Data>
    inline bool Invoke(Func &&func, Data &cached_pointers) noexcept;

    template <Concepts::VoidFuncRefPtr Func, typename Data>
    inline void Invoke(Func &&func, Data &cached_pointers) noexcept;

  public:
    /*
     * Constructor / Destructor
     */

    inline Instance(Manager &manager, Archetype::ID id, const Component::Signatures &signatures) noexcept;
    inline ~Instance() = default;

    /*
     * ID / Signatures
     */

    inline Archetype::ID GetID() const noexcept;
    inline const Component::Signatures &GetSignatures() const noexcept;
    inline const Component::Signatures &GetExclusiveSignatures() const noexcept;

    template <Component::Concepts::IsComponent C>
    inline bool HaveComponent() const noexcept;
    inline bool HaveComponent(const Component::Descriptor &descriptor) const noexcept;

    template <Component::Concepts::IsComponent... Cs>
    inline bool HaveComponents() const noexcept;
    inline bool HaveComponents(Component::DescriptorSpan descriptors) const noexcept;

    /**
     * * Entities API
     */
    inline Entity CreateEntity(DescriptorSpan descriptors, DataSpan datas) noexcept;

    template <ECS::Concepts::SetterCallback Func = EmptyLambda>
    inline Entity CreateEntity(Func &&func = EmptyLambda{}) noexcept;

    inline vector<Entity> CreateEntities(u32 count, DescriptorSpan descriptors, DataSpan datas) noexcept;

    template <ECS::Concepts::SetterCallback Func = EmptyLambda>
    inline vector<Entity> CreateEntities(u32 count, Func &&func = EmptyLambda{}) noexcept;

    inline void DestroyEntity(Entity &entity) noexcept;

    template <ECS::Concepts::SetterCallback Func = EmptyLambda>
    inline void MoveEntity(Entity &entity, Instance &target_archetype, Func &&func = EmptyLambda{}) noexcept;
  };

}