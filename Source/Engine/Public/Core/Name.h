#pragma once
#include "Core/Core.h"

struct ENGINE_API FName {
  FName() : id(0), number(0) {}
  explicit FName(cstring Name) : id(FromStr(Name).id), number(0) {}
  explicit FName(cstring Name, uint32 Number) : id(FromStr(Name).id), number(Number) {}

  static FName FromStr(cstring Str);

  cstring ToStr() const;
  uint64 Hash() const;

  bool IsValid() const {
    return id != 0;
  }

  uint32 GetID() const {
    return id;
  }

  uint32 GetNumber() const {
    return number;
  }

  bool operator==(const FName& Other) const {
    return (id == Other.id && number == Other.number);
  }

private:
  friend class FNamePool;
  explicit FName(uint32 ID) : id(ID) {}
  uint32 id{0};
  uint32 number{0};
};

namespace std {
  template<>
  struct hash<FName> {
    size_t operator()(const FName& Name) const noexcept {
      return static_cast<size_t>(Name.Hash());
    }
  };
}  // namespace std
