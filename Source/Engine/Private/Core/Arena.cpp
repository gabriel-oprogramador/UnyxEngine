#include "Core/Arena.h"
#include "Core/Memory.h"

FArena::~FArena() {
  Release();
}

FArena::FArena(uint64 Capacity) {
  Reserve(Capacity);
}

FArena::FArena(FArena&& Other) noexcept : data(Other.data), used(Other.used), capacity(Other.capacity) {
  Other.data = nullptr;
  Other.used = 0;
  Other.capacity = 0;
}

FArena& FArena::operator=(FArena&& Other) noexcept {
  if(this == &Other) {
    return *this;
  }
  Release();
  data = Other.data;
  used = Other.used;
  capacity = Other.capacity;
  Other.data = nullptr;
  Other.used = 0;
  Other.capacity = 0;
  return *this;
}

bool FArena::Reserve(uint64 Capacity) {
  UE_ASSERT(data == nullptr);
  UE_ASSERT(Capacity != 0);
  if(data || Capacity == 0) {
    return false;
  }
  data = static_cast<uint8*>(FMemory::Alloc(Capacity, alignof(std::max_align_t)));
  UE_CHECK(data);
  capacity = Capacity;
  used = 0;
  return true;
}

void* FArena::PushSize(uint64 Size, uint64 Alignment) {
  uint64 address = reinterpret_cast<uint64>(data) + used;
  uint64 aligned = FMemory::AlignUp(address, Alignment);
  uint64 padding = aligned - address;
  if(padding > (capacity - used)) {
    return nullptr;
  }
  uint64 offset = used + padding;
  if(Size > (capacity - offset)) {
    return nullptr;
  }
  void* ptr = data + offset;
  used = offset + Size;
  return ptr;
}

void FArena::Reset() {
  used = 0;
}

void FArena::Release() {
  FMemory::Free(data);
  data = nullptr;
  used = 0;
  capacity = 0;
}
