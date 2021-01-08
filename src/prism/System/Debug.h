#include <assert.h>
#include "Log.h"

#ifdef PRISM_DEBUG
#define PR_ASSERT(x, ...) { if(!(x)) { PR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); assert(x); } }
#else
#define PR_ASSERT(x, ...)
#endif