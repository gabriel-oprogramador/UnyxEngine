#ifdef PLATFORM_WEB
#include <emscripten.h>

namespace Platform {

  float TimeGetNow() {
    return emscripten_get_now() / 1000.f;
  }

}  // namespace Platform

#endif  // PLATFORM_WEB
