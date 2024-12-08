#ifndef PLATFROM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED 1

#ifdef __linux__

#define HERON_PLATFORM_LINUX 1

#elif _WIN32

#define HERON_PLATFORM_WINDOWS 1

#else

#error Unsupported platform

#endif


#endif // PLATFROM_HPP_INCLUDED