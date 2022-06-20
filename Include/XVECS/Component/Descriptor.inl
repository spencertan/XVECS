namespace XV::ECS::Component
{
  template <Concepts::IsComponent C>
  consteval ID GenerateID()
  {
    if constexpr (std::is_same_v<Entity, C>)
      return ID();

    if constexpr (C::info.id.value)
      return C::info.id;
    else
      return ID(__FUNCSIG__);
  }
  namespace Details
  {
    template <Concepts::IsComponent C>
    consteval Descriptor CreateDescriptor() noexcept
    {
      Descriptor result_desc;

      // ID
      result_desc.id = GenerateID<C>();

      // Name
      if constexpr (C::info.name.empty())
        result_desc.name = Typename<C>();
      else
        result_desc.name = C::info.name;

      // Bit Index
      result_desc.bit_index = Descriptor::invalid_bit_index;

      // Size
      result_desc.size = static_cast<u16>(sizeof(C));

      // Type
      result_desc.type = C::info.type;

      if constexpr (Concepts::TagComponent<C>)
        return result_desc;

      // Constructor
      if constexpr (std::is_trivially_constructible_v<C>)
        result_desc.constructor = nullptr;
      else
        result_desc.constructor = [](Ptr<byte> p) noexcept
        {
          new (p) C;
        };

      // Destructor
      if constexpr (std::is_trivially_destructible_v<C>)
        result_desc.destructor = nullptr;
      else
        result_desc.destructor = [](Ptr<byte> p) noexcept
        {
          std::destroy_at(std::bit_cast<Ptr<C>>(p));
        };

      // Copy
      if constexpr (std::is_trivially_copy_assignable_v<C>)
        result_desc.copy = nullptr;
      else
        result_desc.copy = [](Ptr<byte> dest, Ptr<const byte> src) noexcept
        {
          *std::bit_cast<Ptr<C>>(dest) = *std::bit_cast<Ptr<const C>>(src);
        };

      // Move
      if constexpr (std::is_trivially_move_assignable_v<C>)
        result_desc.move = nullptr;
      else
        result_desc.move = [](Ptr<byte> dest, Ptr<byte> src) noexcept
        {
          *std::bit_cast<Ptr<C>>(dest) = std::move(*std::bit_cast<Ptr<C>>(src));
        };

      // result_desc.reflect = []( Ptr<byte> p ) noexcept
      // {
      //   return rttr::instance( *std::bit_cast<Ptr<C>>( p ) );
      // };

      return result_desc;
    }
  }

  constexpr bool CompareDescriptor(Ptr<const Descriptor> lhs, Ptr<const Descriptor> rhs) noexcept
  {
    if (lhs->type == rhs->type)
      return lhs->id < rhs->id;
    return static_cast<u32>(lhs->type) < static_cast<u32>(rhs->type);
  }

  template <Concepts::IsComponent... Cs>
  constexpr Signatures GenerateSignatures() noexcept
  {
    Signatures signature;
    (signature.set(info<Cs>.bit_index), ...);
    return signature;
  }
}