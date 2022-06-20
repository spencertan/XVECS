#pragma once

#include "FWD.h"
#include "../../../dep/XV_Utilities/Include/Utilities/Events/Manager.h"
#include "../Entity/Entity.h"
#include "../Component/Descriptor.h"
#include "Pool.h"
namespace XV::ECS::Archetype
{

  inline Archetype::ID GenerateID(const Component::Signatures& signatures) noexcept;

  struct Instance final
  {

    struct Events
    {
      Event::Instance<Entity &> on_entity_created;
      Event::Instance<Entity &> on_entity_destroyed;
      Event::Instance<Entity &> on_entity_moved_in;
      Event::Instance<Entity &> on_entity_moved_out;
    };

    /*
     * Constructor / Destructor
     */

    inline Instance(Manager &manager, Archetype::ID id, const Component::Signatures& signatures) noexcept;
    inline ~Instance() = default;
\
    /*
     * ID / Signatures
     */

    inline Archetype::ID ID() const noexcept;
    inline const Component::Signatures &Signatures() const noexcept;
    inline const Component::Signatures &ExclusiveSignatures() const noexcept;

    template <Component::Concepts::IsComponent C>
    inline bool HaveComponent() const noexcept;
    inline bool HaveComponent(const Component::Descriptor &descriptor) const noexcept;

    template <Component::Concepts::IsComponent... Cs>
    inline bool HaveComponents() const noexcept;
    inline bool HaveComponents(Component::DescriptorSpan descriptors) const noexcept;

private:
    Manager& m_manager;
    Events m_events;
    Archetype::ID m_id;
    Component::Descriptors m_descriptors;
    Component::Signatures m_signatures;
    Component::Signatures m_exclusive;
    u32 m_data_count;
    u32 m_tag_count;
    u32 m_exclusive_count;
    Pool m_pool;


  };

}