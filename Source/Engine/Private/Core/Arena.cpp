#include "Core/Arena.h"
#include "Core/Log.h"

#include <cstring>
#include <new>

FArena::FArena(FArena&& Other) noexcept : data(Other.data), used(Other.used), capacity(Other.capacity) {
  Other.data = nullptr;
  Other.used = 0;
  Other.capacity = 0;
}

FArena& FArena::operator=(FArena&& Other) noexcept {
  if(this == &Other) {
    return *this;
  }

  delete[] data;

  data = Other.data;
  used = Other.used;
  capacity = Other.capacity;

  Other.data = nullptr;
  Other.used = 0;
  Other.capacity = 0;

  return *this;
}

FArena::~FArena() {
  used = 0;
  capacity = 0;
  delete[] data;
}

bool FArena::Resize(uint64 NewCapacity) {
  UE_ASSERT(NewCapacity > capacity);
  char* newData = new(std::nothrow) char[NewCapacity];
  if(!newData) {
    return false;
  }
  if(data && used > 0) {
    std::memcpy(newData, data, used);
    delete[] data;
  }
  data = newData;
  capacity = NewCapacity;
  return true;
}

void* FArena::PushSize(uint64 Size, uint64 Align) {
  UE_ASSERT(Size > 0);
  UE_ASSERT(Align > 0);
  UE_ASSERT((Align & (Align - 1)) == 0);

  uint64 aligned = (used + Align - 1) & ~(Align - 1);

  if(aligned > capacity || Size > capacity - aligned) {
    return nullptr;
  }

  void* ptr = data + aligned;
  used = aligned + Size;

  return ptr;
}
