#ifdef PLATFORM_WINDOWS
#include "Platform/Platform.h"

#include <windows.h>

namespace Platform {

  void* MemAlloc(uint64 Size, uint64 Align) {
    if(Size == 0) {
      return nullptr;
    }
    return _aligned_malloc(Size, Align);
  }

  void MemFree(void* Data) {
    if(!Data) {
      return;
    }
    _aligned_free(Data);
  }

}  // namespace Platform

#endif  // PLATFORM_WINDOWS
