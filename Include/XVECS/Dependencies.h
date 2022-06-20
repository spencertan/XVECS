#pragma once

#ifndef XV_UTILITIES_INCLUDE
// Types
#include "../../Dep/XV_Utilities/Include/Utilities/Core.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Types.h"
#include "../../Dep/XV_Utilities/Include/Utilities/GUID.h"

// Containers
#include "../../Dep/XV_Utilities/Include/Utilities/STL_C/Span.h"
#include "../../Dep/XV_Utilities/Include/Utilities/STL_C/Array.h"
#include "../../Dep/XV_Utilities/Include/Utilities/STL_C/Vector.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Containers/CBitset.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Containers/PinnedVector.h"

// Traits
#include "../../Dep/XV_Utilities/Include/Utilities/Traits/Specialised.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Traits/Function.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Traits/Tuple.h"

// Helper 
#include "../../Dep/XV_Utilities/Include/Utilities/Helper/NonCopyable.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Helper/Singleton.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Hash/XXH3.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Enumerate.h"
#include "../../Dep/XV_Utilities/Include/Utilities/Typename.h"

// Event
#include "../../Dep/XV_Utilities/Include/Utilities/Events/Manager.h"

#else

// Types
#include <Utilities/Core.h>
#include <Utilities/Types.h>
#include <Utilities/GUID.h>

// Containers
#include <Utilities/STL_C/Span.h>
#include <Utilities/STL_C/Array.h>
#include <Utilities/STL_C/Vector.h>
#include <Utilities/Containers/CBitset.h>
#include <Utilities/Containers/PinnedVector.h>

// Traits
#include <Utilities/Traits/Specialised.h>
#include <Utilities/Traits/Function.h>
#include <Utilities/Traits/Tuple.h>

// Helper 
#include <Utilities/Helper/NonCopyable.h>
#include <Utilities/Helper/Singleton.h>
#include <Utilities/Hash/XXH3.h>
#include <Utilities/Enumerate.h>
#include <Utilities/Typename.h>

// Event
#include <Utilities/Events/Manager.h>

#endif
