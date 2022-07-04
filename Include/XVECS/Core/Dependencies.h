#pragma once

#ifndef XVUTILITIES_INCLUDE
// Types
#include "../../Dep/XVUtilities/Include/XVUtilities/Core.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Types.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/GUID.h"

// Containers
#include "../../Dep/XVUtilities/Include/XVUtilities/STL_C/Span.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/STL_C/Array.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/STL_C/Vector.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Containers/CBitset.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Containers/PinnedVector.h"

// Traits
#include "../../Dep/XVUtilities/Include/XVUtilities/Traits/Specialised.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Traits/Function.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Traits/Tuple.h"

// Helper 
#include "../../Dep/XVUtilities/Include/XVUtilities/Helper/NonCopyable.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Helper/Singleton.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Hash/XXH3.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Enumerate.h"
#include "../../Dep/XVUtilities/Include/XVUtilities/Typename.h"

// Event
#include "../../Dep/XVUtilities/Include/XVUtilities/Events/Manager.h"

#else

// Types
#include <XVUtilities/Core.h>
#include <XVUtilities/Types.h>
#include <XVUtilities/GUID.h>

// Containers
#include <XVUtilities/STL_C/Span.h>
#include <XVUtilities/STL_C/Array.h>
#include <XVUtilities/STL_C/Vector.h>
#include <XVUtilities/Containers/CBitset.h>
#include <XVUtilities/Containers/PinnedVector.h>

// Traits
#include <XVUtilities/Traits/Specialised.h>
#include <XVUtilities/Traits/Function.h>
#include <XVUtilities/Traits/Tuple.h>

// Helper 
#include <XVUtilities/Helper/NonCopyable.h>
#include <XVUtilities/Helper/Singleton.h>
#include <XVUtilities/Hash/XXH3.h>
#include <XVUtilities/Enumerate.h>
#include <XVUtilities/Typename.h>

// Event
#include <XVUtilities/Events/Manager.h>

#endif

#include <cassert>