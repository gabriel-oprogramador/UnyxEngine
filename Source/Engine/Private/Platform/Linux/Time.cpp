#ifdef PLATFORM_LINUX
#include <time.h>

namespace Platform {

  float TimeGetNow() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (float)ts.tv_sec + (float)ts.tv_nsec * 1e-9;
  }

}  // namespace Platform

#endif  // PLATFORM_LINUX
