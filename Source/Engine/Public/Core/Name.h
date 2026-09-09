#pragma once
#include "Core/BaseTypes.h"

struct ENGINE_API FName {
  FName() : runtimeID(0) {}
  FName(cstring Str) : runtimeID(FromStr(Str).runtimeID) {}

  static FName FromStr(cstring Str);

  cstring ToStr() const;
  uint64 Hash() const;

  bool IsValid() const {
    return runtimeID != 0;
  }

  uint32 GetID() const {
    return runtimeID;
  }

  bool operator==(const FName& Other) const {
    return runtimeID == Other.runtimeID;
  }

  static void TestUnicode();

private:
  friend class FNamePool;
  explicit FName(uint32 ID) : runtimeID(ID) {}
  uint32 runtimeID{0};
};
