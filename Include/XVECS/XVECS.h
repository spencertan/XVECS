#pragma once

/**
 * @brief 
 * * Files Declaration
 * TODO: Clean up header includes in the future
 */
#include "Dependencies.h"
#include "../Core/Settings.h"
#include "../Core/Declaration.h"
#include "Component/Declaration.h"
#include "Entity/Declaration.h"
#include "Query/Declaration.h"
#include "Archetype/Declaration.h"
#include "World/Instance.h"

/**
 * @brief 
 * * Inline File Declaration
 */
#ifdef XV_ECS_IMPL
#include "Component/Descriptor.inl"
#include "Component/Collection.inl"
#include "Entity/Manager.inl"
#include "Query/Instance.inl"
#include "Archetype/Pool.inl"
#include "Archetype/Instance.inl"
#include "Archetype/Manager.inl"
#endif






