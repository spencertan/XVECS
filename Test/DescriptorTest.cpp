#include "../Include/XVECS/Component/Descriptor.h"


struct Entity
{
  static constexpr auto info = XV::ECS::Component::Type::Data{{.name = "Entity"}};
  XV::u64 value;
};


#include "../Include/XVECS/Component/Descriptor.inl"

struct C1
{
  static constexpr auto info = XV::ECS::Component::Type::Data{{.name = "C1"}};
  XV::u32 value;
  XV::u32 value2;
};

#include <iostream>

int main()
{

std::cout << XV::ECS::Component::info<C1>.name << std::endl;


}