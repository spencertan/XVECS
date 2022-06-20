#pragma once

namespace XV::ECS::Settings
{
  constexpr auto page_size = 4096;
  constexpr auto max_entities = 1 << 16;
  constexpr auto max_components = 64;
}