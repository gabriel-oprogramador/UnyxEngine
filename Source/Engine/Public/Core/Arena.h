#pragma once
#include "Core/BaseTypes.h"

struct FArena {
  char* data{nullptr};
  uint64 used{0};
  uint64 capacity{0};

  FArena() = default;

  explicit FArena(uint64 capacity) : data(new char[capacity]), used(0), capacity(capacity) {}

  ~FArena();

  FArena(const FArena& Other) = delete;
  FArena& operator=(const FArena& Other) = delete;

  FArena(FArena&& Other) noexcept;
  FArena& operator=(FArena&& Other) noexcept;

  bool Resize(uint64 NewCapacity);
  void* PushSize(uint64 Size, uint64 Align = 16);

  template<typename T>
  T* Get(uint64 Offset) {
    return reinterpret_cast<T*>(data + Offset);
  }

  template<typename T>
  T* Push(uint32 Count) {
    return static_cast<T*>(PushSize(sizeof(T) * Count, alignof(T)));
  }
};
