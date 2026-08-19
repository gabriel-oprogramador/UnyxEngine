#pragma once
#include "BaseTypes.h"

template<typename T>
struct THandleList;

template<typename T>
struct THandle {
  THandle() : slotIndex(UINT32_MAX), slotVersion(0) {}

  bool operator==(const THandle<T>& Other) {
    return (slotIndex == Other.slotIndex && slotVersion == Other.slotVersion);
  }
  bool operator!=(const THandle<T>& Other) {
    return !(*this == Other);
  }

  THandle<T>& operator=(const THandle<T>& Other) {
    slotIndex = Other.slotIndex;
    slotVersion = Other.slotVersion;
    return *this;
  }

  uint32 GetIndex() const {
    return slotIndex;
  }
  uint32 GetVersion() const {
    return slotVersion;
  }

private:
  friend struct THandleList<T>;
  explicit THandle(uint32 SlotIndex, uint32 SlotVersion) : slotIndex(SlotIndex), slotVersion(SlotVersion) {}
  uint32 slotIndex;
  uint32 slotVersion;
};
