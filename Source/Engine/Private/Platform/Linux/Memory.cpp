#ifdef PLATFORM_LINUX

#include "Platform/Platform.h"

#include <unistd.h>
#include <cstdlib>

namespace Platform {

  void* MemAlloc(uint64 Size, uint64 Align) {
    if(Size == 0) {
      return nullptr;
    }
    void* Ptr = nullptr;
    if(posix_memalign(&Ptr, Align, Size) != 0) {
      return nullptr;
    }
    return Ptr;
  }

  void MemFree(void* Data) {
    if(!Data) {
      return;
    }
    std::free(Data);
  }

}  // namespace Platform

#endif  // PLATFORM_LINUX
