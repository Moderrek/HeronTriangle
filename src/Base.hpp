#ifndef BASE_HPP_INCLUDED
#define BASE_HPP_INCLUDED 1

#include <memory>

#ifdef HERON_DEBUG

  #if defined(HERON_PLATFORM_WINDOWS)
    #define HERON_DEBUGBREAK() __debugbreak()
  #elif defined(HERON_PLATFORM_LINUX)
    #include <signal.h>
    #define HERON_DEBUGBREAK() raise(SIGTRAP)
  #else
    #error "Platform doesn't have debug break support!"
  #endif

#else

#define HERON_DEBUGBREAK()

#endif

#ifdef HERON_DEBUG

#define ASSERT(x) if (!(x)) HERON_DEBUGBREAK()

#else

#define ASSERT(x)

#endif

#endif // BASE_HPP_INCLUDED