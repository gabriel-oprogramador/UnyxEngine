#include "Core/StringView.h"
#include "Core/String.h"
#include "Core/Memory.h"
#include "Core/Unicode.h"

FStringView::FStringView(cstring String) {
  data = String;
  size = FUnicode::ByteLength(String);
  length = FUnicode::CodePointCount(String, size);
}

FStringView::FStringView(const FString& String) {
  data = String.ToStr();
  size = String.Size();
  length = String.Length();
}

FStringView::FStringView(cstring Data, uint32 Size, uint32 Length) : data(Data), size(Size), length(Length) {}

bool FStringView::operator==(const FStringView& Other) const {
  if(size != Other.size) {
    return false;
  }
  return (FMemory::Compare(data, Other.data, size) == 0);
}

bool FStringView::operator!=(const FStringView& Other) const {
  return !(*this == Other);
}

uint64 FStringView::Hash() const {
  uint64 hash = 14695981039346656037ull;
  const uint8* ptr = reinterpret_cast<const uint8*>(data);
  for(uint32 i = 0; i < size; ++i) {
    hash ^= ptr[i];
    hash *= 1099511628211ull;
  }
  return hash;
}

cstring FStringView::Data() const {
  return data;
}

uint32 FStringView::Size() const {
  return size;
}

uint32 FStringView::Length() const {
  return length;
}
