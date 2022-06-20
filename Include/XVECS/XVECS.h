#pragma once

/**
 * @brief 
 * * Dependencies Declaration
 */

#include "Dependencies.h"


/**
 * @brief 
 * * Alias Declaration
 */

namespace XV
{
  template <typename... Ts>
  using CList = Typelist<Ts...>;

  template <typename... Ts>
  using CListAdd = CList<Ts...>;

  template <typename... Ts>
  using CListRemove = CList<Ts...>;

  template <typename... Ts>
  using SList = Typelist<Ts...>;

  template <typename... Ts>
  using EList = Typelist<Ts...>;
}

/**
 * @brief 
 * * Files Declaration
 */

#include "Core/Settings.h"
#include "Declaration.h"











