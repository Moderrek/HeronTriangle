#pragma once

/*
* Scoped Timer 
* by Hegemon Studio
* 
* used for profiling
* speed measure
* 
* define HG_TIMER_OFF
* if you want to disable
* timer e.g. for release
*/

#include <iostream>
#include <chrono>

#ifdef HG_TIMER_OFF

#define HG_SCOPED_TIMER(name)

#else

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)

#define HG_SCOPED_TIMER(name)  ScopedTimer CONCATENATE(timer_, __LINE__){name}

#endif

class ScopedTimer {
public:
  ScopedTimer(const char* name);
  ScopedTimer(const std::string& name);
  ScopedTimer();

  ~ScopedTimer();

  long long stop();

private:
  const char* m_name;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start_point;
  bool m_stopped;
};

ScopedTimer::ScopedTimer(const char* name) : m_name{ name } {
  m_start_point = std::chrono::high_resolution_clock::now();
  m_stopped = false;
}

ScopedTimer::ScopedTimer(const std::string& name) : m_name{ name.c_str() } {
  m_start_point = std::chrono::high_resolution_clock::now();
  m_stopped = false;
}

ScopedTimer::ScopedTimer() {
  m_name = "";
  m_start_point = std::chrono::high_resolution_clock::now();
  m_stopped = false;
}

ScopedTimer::~ScopedTimer() {
  if (!m_stopped) stop();
}

long long ScopedTimer::stop() {
  const auto end_point = std::chrono::high_resolution_clock::now();

  const long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_point).time_since_epoch().count();
  const long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();
  const long long diff = end - start;

  std::cout << "[TIMER] " << m_name << ' ' << diff << "ms\n";

  m_stopped = true;
  return diff;
}