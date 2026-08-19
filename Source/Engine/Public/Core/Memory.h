#pragma once
#include "Core/BaseTypes.h"
#include "Core/Log.h"

#include <new>
#include <utility>
#include <cstring>
#include <type_traits>

struct FMemory {
  template<typename T>
  using TClean = std::remove_cv_t<std::remove_reference_t<T>>;

  static void* Alloc(uint64 Size, uint64 Align = alignof(std::max_align_t));
  static void Free(void* Data);

  static void* CopyBytes(void* Destination, const void* Source, uint64 Size) {
    UE_ASSERT(Destination);
    UE_ASSERT(Source);
    UE_ASSERT(Size > 0);
    return std::memcpy(Destination, Source, Size);
  }

  static void* MoveBytes(void* Destination, const void* Source, uint64 Size) {
    UE_ASSERT(Destination);
    UE_ASSERT(Source);
    UE_ASSERT(Size > 0);
    return std::memmove(Destination, Source, Size);
  }

  static void* SetBytes(void* Destination, int32 Value, uint64 Size) {
    UE_ASSERT(Destination);
    UE_ASSERT(Size > 0);
    return std::memset(Destination, Value, Size);
  }

  template<typename T, typename... TArgs>
  static void Construct(T* Destination, TArgs&&... Args) {
    UE_ASSERT(Destination);
    using Type = TClean<T>;
    new(Destination) Type(std::forward<TArgs>(Args)...);
  }

  template<typename T>
  static void DefaultConstruct(T* Destination, uint32 Count) {
    if(Count == 0) {
      return;
    }
    UE_ASSERT(Destination);
    using Type = TClean<T>;
    for(uint32 i = 0; i < Count; ++i) {
      new(Destination + i) Type();
    }
  }

  template<typename T>
  static void Destroy(T* Data, uint32 Count) {
    if(Count == 0) {
      return;
    }
    UE_ASSERT(Data);
    using Type = TClean<T>;
    if constexpr(!std::is_trivially_destructible_v<Type>) {
      for(uint32 i = 0; i < Count; ++i) {
        Data[i].~Type();
      }
    }
  }

  template<typename T>
  static void CopyConstruct(T* Destination, const T* Source, uint32 Count) {
    if(Count == 0) {
      return;
    }
    UE_ASSERT(Destination);
    UE_ASSERT(Source);
    using Type = TClean<T>;
    if constexpr(std::is_trivially_copyable_v<Type>) {
      std::memcpy(Destination, Source, sizeof(Type) * Count);
    } else {
      for(uint32 i = 0; i < Count; ++i) {
        new(Destination + i) Type(Source[i]);
      }
    }
  }

  template<typename T>
  static void MoveConstruct(T* Destination, T* Source, uint32 Count) noexcept {
    if(Count == 0) {
      return;
    }
    UE_ASSERT(Destination);
    UE_ASSERT(Source);
    using Type = TClean<T>;
    if constexpr(std::is_trivially_copyable_v<Type>) {
      std::memmove(Destination, Source, sizeof(Type) * Count);
    } else {
      for(uint32 i = 0; i < Count; ++i) {
        new(Destination + i) Type(std::move(Source[i]));
      }
    }
  }

  template<typename T>
  static void CopyAssign(T* Destination, const T* Source, uint32 Count) {
    if(Count == 0) {
      return;
    }
    UE_ASSERT(Destination);
    UE_ASSERT(Source);
    using Type = TClean<T>;
    if constexpr(std::is_trivially_copyable_v<Type>) {
      std::memmove(Destination, Source, sizeof(Type) * Count);
    } else {
      for(uint32 i = 0; i < Count; ++i) {
        Destination[i] = Source[i];
      }
    }
  }

  template<typename T>
  static void MoveAssign(T* Destination, T* Source, uint32 Count) noexcept {
    if(Count == 0) {
      return;
    }
    UE_ASSERT(Destination);
    UE_ASSERT(Source);
    using Type = TClean<T>;
    if constexpr(std::is_trivially_copyable_v<Type>) {
      std::memmove(Destination, Source, sizeof(Type) * Count);
    } else {
      for(uint32 i = 0; i < Count; ++i) {
        Destination[i] = std::move(Source[i]);
      }
    }
  }

  template<typename T>
  static void Swap(T& A, T& B) noexcept {
    std::swap(A, B);
  }
};
