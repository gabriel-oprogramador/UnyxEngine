#pragma once
#include "Core/Core.h"

struct FString;

struct ENGINE_API FStringView {
  FStringView() = default;

  FStringView(cstring String);
  FStringView(const FString& String);
  FStringView(cstring Data, uint32 Size, uint32 Length);

  bool operator==(const FStringView& Other) const;
  bool operator!=(const FStringView& Other) const;

  uint64 Hash() const;
  cstring Data() const;
  uint32 Size() const;
  uint32 Length() const;

private:
  cstring data{nullptr};
  uint32 size{0};
  uint32 length{0};
};

namespace std {
  template<>
  struct hash<FStringView> {
    size_t operator()(const FStringView& View) const noexcept {
      return static_cast<size_t>(View.Hash());
    }
  };
}  // namespace std
