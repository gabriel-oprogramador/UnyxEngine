#include "Core/String.h"
#include "Core/Unicode.h"
#include "Core/Memory.h"

#undef UE_INFO
#define UE_INFO(Format, ...)

FString::FString(cstring String) {
  size = static_cast<uint32>(FUnicode::ByteLength(String));
  length = static_cast<uint32>(FUnicode::CodePointCount(String, size));
  if(size < INLINE_CAPACITY) {
    str = inlineBuffer;
  } else {
    str = static_cast<char*>(FMemory::Alloc(size + 1));
  }
  FMemory::CopyBytes(str, String, size);
  str[size] = '\0';
  UE_INFO("FString Construct(%s) Size:%u, Length:%u", str, size, length);
}

FString::~FString() {
  if(size >= INLINE_CAPACITY) {
    FMemory::Free(str);
    UE_INFO("FString Destruct Heap(%s) Size:%u, Length:%u", str, size, length);
    return;
  }
  UE_INFO("FString Destruct(%s) Size:%u, Length:%u", str, size, length);
}

FString::FString(const FString& Other) : size(Other.size), length(Other.length) {
  if(size < INLINE_CAPACITY) {
    str = inlineBuffer;
  } else {
    str = static_cast<char*>(FMemory::Alloc(size + 1));
  }
  FMemory::CopyBytes(str, Other.str, size);
  str[size] = '\0';
  UE_INFO("FString CopyConstruct(%s) Size:%u, Length:%u", str, size, length);
}

FString::FString(FString&& Other) noexcept : size(Other.size), length(Other.length) {
  if(size < INLINE_CAPACITY) {
    str = inlineBuffer;
    FMemory::CopyBytes(str, Other.str, size);
    str[size] = '\0';
  } else {
    str = Other.str;
    Other.str = nullptr;
  }
  Other.size = 0;
  Other.length = 0;
  UE_INFO("FString MoveConstruct(%s) Size:%u, Length:%u", str, size, length);
}

FString& FString::operator=(const FString& Other) {
  if(this == &Other) {
    return *this;
  }
  if(size >= INLINE_CAPACITY) {
    FMemory::Free(str);
  }
  if(Other.size < INLINE_CAPACITY) {
    str = inlineBuffer;
  } else {
    str = static_cast<char*>(FMemory::Alloc(Other.size + 1));
  }
  FMemory::CopyBytes(str, Other.str, Other.size);
  str[Other.size] = '\0';
  size = Other.size;
  length = Other.length;
  UE_INFO("FString CopyAssign(%s) Size:%u, Length:%u", str, size, length);
  return *this;
}

FString& FString::operator=(FString&& Other) noexcept {
  if(this == &Other) {
    return *this;
  }
  if(size >= INLINE_CAPACITY) {
    FMemory::Free(str);
  }
  if(Other.size < INLINE_CAPACITY) {
    str = inlineBuffer;
    FMemory::CopyBytes(str, Other.str, Other.size);
    str[Other.size] = '\0';
  } else {
    str = Other.str;
    Other.str = nullptr;
  }
  size = Other.size;
  length = Other.length;
  Other.size = 0;
  Other.length = 0;
  UE_INFO("FString MoveAssign(%s) Size:%u, Length:%u", str, size, length);
  return *this;
}

bool FString::operator==(const FString& Other) const {
  if(size != Other.size) {
    return false;
  }
  return (FMemory::Compare(str, Other.str, size) == 0);
}

bool FString::operator!=(const FString& Other) const {
  return !(*this == Other);
}

void FString::Normalize() {
  FUnicode::FResult result{};
  if(!FUnicode::NormalizeNFC(str, size, result) || !result.transformed) {
    return;
  }

  if(size >= INLINE_CAPACITY) {
    FMemory::Free(str);
  }
  size = result.size;
  if(size < INLINE_CAPACITY) {
    str = inlineBuffer;
  } else {
    str = static_cast<char*>(FMemory::Alloc(size + 1));
  }
  FMemory::CopyBytes(str, result.data, size);
  str[size] = '\0';
  length = FUnicode::CodePointCount(str, size);
  FUnicode::Free(result);
}

cstring FString::ToStr() const {
  return str;
}

uint32 FString::Length() const {
  return length;
}

uint32 FString::Size() const {
  return size;
}

uint64 FString::Hash() const {
  uint64 hash = 14695981039346656037ull;
  const uint8* ptr = reinterpret_cast<const uint8*>(str);
  for(uint32 i = 0; i < size; ++i) {
    hash ^= ptr[i];
    hash *= 1099511628211ull;
  }
  return hash;
}
