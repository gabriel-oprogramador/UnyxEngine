#ifdef PLATFORM_WEB
#include <emscripten.h>

namespace Platform {

  double TimeGetNow() {
    return emscripten_get_now() / 1000.0;
  }

}  // namespace Platform

#endif  // PLATFORM_WEB
