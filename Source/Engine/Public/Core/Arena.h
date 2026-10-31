#pragma once
#include "Core/Core.h"

struct ENGINE_API FArena {
  FArena() = default;
  ~FArena();

  explicit FArena(uint64 Capacity);

  FArena(const FArena& Other) = delete;
  FArena(FArena&& Other) noexcept;

  FArena& operator=(const FArena& Other) = delete;
  FArena& operator=(FArena&& Other) noexcept;

  bool Reserve(uint64 Capacity);
  void* PushSize(uint64 Size, uint64 Alignment);

  void Reset();
  void Release();

  template<typename T>
  T* Push(uint32 Count) {
    return static_cast<T*>(PushSize(sizeof(T) * Count, alignof(T)));
  }

private:
  uint8* data{nullptr};
  uint64 used{0};
  uint64 capacity{0};
};
