#include "Platform/Platform.h"
#include "Core/Memory.h"

#include <algorithm>

void* FMemory::Alloc(uint64 Size, uint64 Align) {
  Align = std::max<uint64>(Align, sizeof(void*));
  return Platform::MemAlloc(Size, Align);
}

void FMemory::Free(void* Data) {
  Platform::MemFree(Data);
}
