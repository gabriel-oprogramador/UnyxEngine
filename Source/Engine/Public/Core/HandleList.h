#pragma once
#include "BaseTypes.h"
#include "Handle.h"
#include "Array.h"

template<typename T>
struct THandleList {
  THandleList() = default;
  explicit THandleList(uint32 Capacity) {
    Reserve(Capacity);
  }

  THandleList(const THandleList& Other) = delete;
  THandleList(THandleList&& Other) = delete;

  THandleList& operator=(const THandleList& Other) = delete;
  THandleList& operator=(THandleList&& Other) = delete;

  void Reserve(uint32 NewCapacity) {
    if(NewCapacity <= resources.Capacity()) {
      return;
    }
    resources.Reserve(NewCapacity);
    owners.Reserve(NewCapacity);
    slots.Reserve(NewCapacity);
  }

  THandle<T> FindHandle(const T& Resource) {
    return FindHandle([&](const T& Item) { return Item == Resource; });
  }

  template<typename TPredicate>
  THandle<T> FindHandle(TPredicate Predicate) {
    for(uint32 c = 0; c < resources.Count(); c++) {
      if(Predicate(resources[c])) {
        uint32 slotIndex = owners[c];
        return THandle<T>{slotIndex, slots[slotIndex].version};
      }
    }
    return THandle<T>{};
  }

  T* Lookup(THandle<T>& Handle) {
    uint32 slotIndex = Handle.slotIndex;
    if(slotIndex >= slots.Count()) {
      return nullptr;
    }
    FSlot& slot = slots[slotIndex];
    if(slot.version != Handle.slotVersion) {
      return nullptr;
    }
    return &resources[slot.resourceIndex];
  }

  THandle<T> Add(const T& Resource) {
    THandle<T> handle = CreateHandle();
    resources.Add(Resource);
    return handle;
  }

  THandle<T> Add(T&& Resource) {
    THandle<T> handle = CreateHandle();
    resources.Add(std::move(Resource));
    return handle;
  }

  void Remove(THandle<T>& Handle) {
    uint32 slotIndex = Handle.slotIndex;
    if(slotIndex >= slots.Count()) {
      return;
    }
    FSlot& slot = slots[slotIndex];
    if(slot.version != Handle.slotVersion) {
      return;
    }
    uint32 removedIndex = slot.resourceIndex;
    uint32 lastIndex = resources.Count() - 1;
    if(removedIndex != lastIndex) {
      resources[removedIndex] = std::move(resources[lastIndex]);
      uint32 movedSlotIndex = owners[lastIndex];
      FSlot& movedSlot = slots[movedSlotIndex];
      movedSlot.resourceIndex = removedIndex;
    }
    resources.Pop();
    owners.Pop();
    ++slot.version;
    slot.nextFree = freeListStart;
    freeListStart = slotIndex;
    ++freeListCount;
    Handle.slotIndex = UINT32_MAX;
    Handle.slotVersion = 0;
  }

  T* begin() {
    return resources.begin();
  }
  T* end() {
    return resources.end();
  }
  const T* begin() const {
    return resources.begin();
  }
  const T* end() const {
    return resources.end();
  }

private:
  THandle<T> CreateHandle() {
    uint32 slotIndex = UINT32_MAX;
    if(freeListCount > 0) {
      slotIndex = freeListStart;
      FSlot& slot = slots[slotIndex];
      --freeListCount;
      slot.resourceIndex = resources.Count();
    } else {
      slotIndex = slots.Count();
      slots.Add(FSlot{resources.Count(), 1});
    }
    owners.Add(slotIndex);
    FSlot& slot = slots[slotIndex];
    return THandle<T>{slotIndex, slot.version};
  }

  struct FSlot {
    union {
      uint32 resourceIndex;
      uint32 nextFree;
    };
    uint32 version;
  };
  TArray<T> resources;
  TArray<FSlot> slots;
  TArray<uint32> owners;
  uint32 freeListStart{0};
  uint32 freeListCount{0};
};
