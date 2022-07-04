#pragma once

namespace XV::ECS::Archetype
{

  class Manager final
  {

    using ArchetypeMap = unordered_map<Archetype::ID, Ptr<Archetype::Instance>>;
    using ArchetypeList = PinnedVector<UniquePtr<Archetype::Instance>>;
    using SignatureAndExclusive = std::pair<Signature, Signature>;
    using ArchetypeSignatureList = PinnedVector<SignatureAndExclusive>;

    struct Events
    {
      Event::Instance<Archetype::Instance &> on_new_archetype;
    } m_events;

    World::Instance &m_world;
    ArchetypeMap m_archetype_map;
    ArchetypeList m_archetypes;
    ArchetypeSignatureList m_archestypes_signatures;
    Ptr<Pool> m_pool_structural_pending_list{nullptr};

    inline Archetype::Instance& Create(ID id, const Signature& signature) noexcept;
    inline void AddToStructuralPendingList(Pool& pool) noexcept;

  public:
    inline Manager() = delete;
    inline Manager(World::Instance &world) noexcept;

    inline World::Instance& GetWorld() noexcept;

    inline Ptr<Archetype::Instance> GetArchetype(ID id) noexcept;
    inline Archetype::Instance& GetOrCreateArchetype(Signature& signature) noexcept;
    inline void UpdateStructuralChanges() noexcept;
  };

}