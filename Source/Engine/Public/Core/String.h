#pragma once
#include "Core/Core.h"

struct ENGINE_API FString {
  FString() = default;
  FString(cstring String);
  ~FString();

  FString(const FString& Other);
  FString(FString&& Other) noexcept;

  FString& operator=(const FString& Other);
  FString& operator=(FString&& Other) noexcept;
  bool operator==(const FString& Other) const;
  bool operator!=(const FString& Other) const;

  void Normalize();
  cstring ToStr() const;
  uint32 Length() const;
  uint32 Size() const;
  uint64 Hash() const;

private:
  static constexpr uint32 INLINE_CAPACITY = 24;
  char* str{nullptr};
  uint32 size{0};
  uint32 length{0};
  char inlineBuffer[INLINE_CAPACITY];
};

namespace std {
  template<>
  struct hash<FString> {
    size_t operator()(const FString& String) const noexcept {
      return static_cast<size_t>(String.Hash());
    }
  };
}  // namespace std
