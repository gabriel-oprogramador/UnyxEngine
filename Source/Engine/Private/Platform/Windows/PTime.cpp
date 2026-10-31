#ifdef PLATFORM_WINDOWS

#include <windows.h>

#include "Platform/Platform.h"

namespace Platform {

  double TimeGetNow() {
    static LARGE_INTEGER timerFrequency = []() {
      LARGE_INTEGER freq;
      QueryPerformanceFrequency(&freq);
      return freq;
    }();

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)timerFrequency.QuadPart;
  }

}  // namespace Platform

#endif  // PLATFORM_WINDOWS
