#pragma once
#include "Core/BaseTypes.h"
#include "Core/Memory.h"

#include <utility>
#include <initializer_list>

template<typename T>
struct TArray {
  TArray() : data(nullptr), count(0), capacity(0) {}

  ~TArray() {
    FMemory::Destroy(data, count);
    FMemory::Free(data);
  }

  explicit TArray(uint32 Capacity) : data(nullptr), count(0), capacity(0) {
    Reserve(Capacity);
  }

  TArray(std::initializer_list<T> Init) {
    Reserve(static_cast<uint32>(Init.size()));
    for(const T& Value : Init) {
      Add(Value);
    }
  }

  TArray(const TArray& Other) : data(nullptr), count(0), capacity(0) {
    Reserve(Other.count);
    FMemory::CopyConstruct(data, Other.data, Other.count);
    count = Other.count;
  }

  TArray(TArray&& Other) noexcept : data(Other.data), count(Other.count), capacity(Other.capacity) {
    Other.data = nullptr;
    Other.count = 0;
    Other.capacity = 0;
  }

  TArray& operator=(const TArray& Other) {
    if(this == &Other) {
      return *this;
    }
    FMemory::Destroy(data, count);
    FMemory::Free(data);
    data = nullptr;
    count = 0;
    capacity = 0;
    Reserve(Other.count);
    FMemory::CopyConstruct(data, Other.data, Other.count);
    count = Other.count;
    return *this;
  }

  TArray& operator=(TArray&& Other) noexcept {
    if(this == &Other) {
      return *this;
    }
    FMemory::Destroy(data, count);
    FMemory::Free(data);
    data = Other.data;
    count = Other.count;
    capacity = Other.capacity;
    Other.data = nullptr;
    Other.capacity = 0;
    Other.count = 0;
    return *this;
  }

  T& operator[](uint32 Index) {
    return Get(Index);
  }
  const T& operator[](uint32 Index) const {
    return Get(Index);
  }

  uint32 Count() const {
    return count;
  }

  uint32 Capacity() const {
    return capacity;
  }

  bool Empty() {
    return count <= 0;
  }

  void Clear() {
    FMemory::Destroy(data, count);
    count = 0;
  }

  void Reserve(uint32 NewCapacity) {
    if(NewCapacity <= capacity) {
      return;
    }
    T* newData = static_cast<T*>(FMemory::Alloc(sizeof(T) * NewCapacity, alignof(T)));
    FMemory::MoveConstruct(newData, data, count);
    FMemory::Destroy(data, count);
    FMemory::Free(data);
    data = newData;
    capacity = NewCapacity;
  }

  T& Get(uint32 Index) {
    UE_ASSERT(Index < count);
    return data[Index];
  }
  const T& Get(uint32 Index) const {
    UE_ASSERT(Index < count);
    return data[Index];
  }

  template<typename... TArgs>
  T& Emplace(TArgs&&... Args) {
    if(count == capacity) {
      Reserve((capacity == 0) ? 8 : capacity * 2);
    }
    FMemory::Construct(data + count, std::forward<TArgs>(Args)...);
    return data[count++];
  }

  T& Add() {
    if(count == capacity) {
      Reserve((capacity == 0) ? 8 : capacity * 2);
    }
    FMemory::DefaultConstruct(data + count, 1);
    return data[count++];
  }

  T& Add(const T& Item) {
    if(count == capacity) {
      Reserve((capacity == 0) ? 8 : capacity * 2);
    }
    FMemory::CopyConstruct(data + count, &Item, 1);
    return data[count++];
  }

  T& Add(T&& Item) {
    if(count == capacity) {
      Reserve((capacity == 0) ? 8 : capacity * 2);
    }
    FMemory::MoveConstruct(data + count, &Item, 1);
    return data[count++];
  }

  T Pop() {
    UE_ASSERT(count > 0);
    uint32 last = count - 1;
    T retVal = std::move(data[last]);
    FMemory::Destroy(data + last, 1);
    --count;
    return retVal;
  }

  void Remove(uint32 Index) {
    UE_ASSERT(Index < count);
    uint32 moveCount = count - Index - 1;
    if(moveCount > 0) {
      FMemory::MoveAssign(data + Index, data + Index + 1, moveCount);
    }
    uint32 last = count - 1;
    FMemory::Destroy(data + last, 1);
    --count;
  }

  void RemoveSwap(uint32 Index) {
    UE_ASSERT(Index < count);
    uint32 last = count - 1;
    if(last != Index) {
      FMemory::Swap(data[Index], data[last]);
    }
    FMemory::Destroy(data + last, 1);
    --count;
  }

  T* begin() {
    return data;
  }
  T* end() {
    return data + count;
  }
  const T* begin() const {
    return data;
  }
  const T* end() const {
    return data + count;
  }

private:
  T* data{nullptr};
  uint32 count{0};
  uint32 capacity{0};
};
