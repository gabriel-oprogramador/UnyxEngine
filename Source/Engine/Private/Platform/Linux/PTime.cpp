#ifdef PLATFORM_LINUX
#include <time.h>

namespace Platform {

  double TimeGetNow() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
  }

}  // namespace Platform

#endif  // PLATFORM_LINUX
