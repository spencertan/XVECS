#pragma once

namespace XV::ECS::Component
{

  /**
   * @brief
   * * Component Descriptor
   */

  struct Descriptor final
  {

    static constexpr auto invalid_bit_index = (u16)~0;
    static constexpr auto processing_bit_index = invalid_bit_index - 1;

    using Constructor = FuncPtr<void(Ptr<byte>) noexcept>;
    using Destructor = FuncPtr<void(Ptr<byte>) noexcept>;
    using Copy = FuncPtr<void(Ptr<byte>, Ptr<const byte>) noexcept>;
    using Move = FuncPtr<void(Ptr<byte>, Ptr<byte>) noexcept>;
    // using Reflect = FuncPtr<rttr::instance( Ptr<byte> )noexcept>;

    Constructor constructor{nullptr};
    Destructor destructor{nullptr};
    Copy copy{nullptr};
    Move move{nullptr};
    // Reflect m_reflect { nullptr };

    ID id;
    std::string_view name;
    mutable u16 bit_index;
    u16 size;
    Type::ID type;
  };

  /**
   * @brief
   * * Descriptor Create/Retrieval
   */

  namespace Details
  {
    template <Concepts::IsComponent C>
    inline consteval Descriptor CreateDescriptor() noexcept;

    template <typename C>
    inline constexpr auto desc = CreateDescriptor<C>();
  }

  template <typename C>
  inline constexpr auto &info = Details::desc<Decay<C>>;

  /**
   * @brief
   * * Descriptor Helper
   */

  inline constexpr bool CompareDescriptors(Ptr<const Descriptor> lhs, Ptr<const Descriptor> rhs) noexcept;

  template <Concepts::IsComponent... Cs>
  inline constexpr Signatures GenerateSignatures() noexcept;

  namespace Details
  {
    template <bool B>
    struct Condition 
    {
      template <typename C1, typename> using Type = C1;
    };

    template<>
    struct Condition<false>
    {
      template<typename, typename C2> using Type = C2;
    };

    template <typename C1, typename C2>
    using Smaller = Condition<CompareDescriptors(Component::info<C1>,Component::info<C2>)>;
  }

template <typename Tuple>
using DataOnlyTuple =
std::invoke_result_t<decltype( []<typename... Ts>( tuplet::tuple<Ts...> * )
  -> Traits::Tuple::CatType<std::conditional_t<
                               Component::info<Ts>.m_type == Component::Type::ID::Data, tuplet::tuple<Ts>, tuplet::tuple<>
  >...>
{} ), Tuple * > ;

  template <typename Tuple>
  using SortComponentsTuple = Traits::Tuple::SortType<Details::Smaller, Tuple>;
}

namespace XV::ECS::Component
{
  using Descriptors= vector<Ptr<const Descriptor>>;
}