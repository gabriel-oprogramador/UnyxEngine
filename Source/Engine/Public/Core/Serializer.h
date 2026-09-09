#pragma once
#include "Core/BaseTypes.h"
#include <cstdio>
#include <cstdarg>

struct FArchive {
  FILE* file{nullptr};
  bool bReadOnly{false};

  FArchive() = default;
  ~FArchive() {
    Close();
  }

  void Close() {
    if(file) {
      fclose(file);
      file = nullptr;
    }
  }

  bool OpenRead(cstring Path) {
    file = fopen(Path, "r");
    if(!file) {
      return false;
    }
    bReadOnly = true;
    return true;
  }

  bool OpenWrite(cstring Path) {
    file = fopen(Path, "w");
    if(!file) {
      return false;
    }
    bReadOnly = false;
    return true;
  }

  void Write(cstring Text) {
    fputs(Text, file);
  }

  void WriteFormat(cstring Format, ...) {
    va_list args;
    va_start(args, Format);
    vfprintf(file, Format, args);
    va_end(args);
  }

  bool ReadFormat(cstring Format, ...) {
    va_list args;
    va_start(args, Format);
    int result = vfscanf(file, Format, args);
    va_end(args);
    return result > 0;
  }
};

template<typename T>
struct TSerializer {
  static constexpr bool Supported = false;
};

#define TYPE_SERIALIZE_DEF(Type)                                 \
  template<>                                                     \
  struct TSerializer<Type> {                                     \
    static constexpr bool Supported = true;                      \
    static ENGINE_API void Serialize(FArchive& Ar, Type& Value); \
  };

TYPE_SERIALIZE_DEF(bool)
TYPE_SERIALIZE_DEF(int8)
TYPE_SERIALIZE_DEF(int16)
TYPE_SERIALIZE_DEF(int32)
TYPE_SERIALIZE_DEF(int64)
TYPE_SERIALIZE_DEF(uint8)
TYPE_SERIALIZE_DEF(uint16)
TYPE_SERIALIZE_DEF(uint32)
TYPE_SERIALIZE_DEF(uint64)
TYPE_SERIALIZE_DEF(cstring)
TYPE_SERIALIZE_DEF(float)
TYPE_SERIALIZE_DEF(double)
TYPE_SERIALIZE_DEF(struct FName)
//TYPE_SERIALIZE_DEF(struct FVector3)
