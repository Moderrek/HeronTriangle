#ifndef PLATFROM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED 1

#ifdef __linux__

#define PLATFORM_LINUX 1

#elif _WIN32

#define PLATFORM_WINDOWS 1

#else

#error Unsupported platform

#endif


#endif // PLATFROM_HPP_INCLUDED