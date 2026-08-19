#pragma once
#include "Core/Core.h"

using FCodePoint = uint32;

struct FUnicode {
  struct FResult {
    void* data;
    uint64 size;
    bool transformed;
  };

  static bool IsASCII(cstring Str, uint64 Length);
  static bool IsUTF8(cstring Str, uint64 Length);

  static uint64 ByteLength(cstring Str);
  static uint64 CodePointCount(cstring Str, uint64 Length);

  static bool IsNormalizedNFC(cstring Str, uint64 Length);
  static bool IsNormalizedNFD(cstring Str, uint64 Length);
  static bool IsNormalizedNFKC(cstring Str, uint64 Length);
  static bool IsNormalizedNFKD(cstring Str, uint64 Length);

  static bool NormalizeNFC(cstring Str, uint64 Length, FResult& Out);
  static bool NormalizeNFD(cstring Str, uint64 Length, FResult& Out);
  static bool NormalizeNFKC(cstring Str, uint64 Length, FResult& Out);
  static bool NormalizeNFKD(cstring Str, uint64 Length, FResult& Out);

  static bool ToLower(cstring Str, uint64 Length, FResult& Out);
  static bool ToUpper(cstring Str, uint64 Length, FResult& Out);
  static bool ToTitle(cstring Str, uint64 Length, FResult& Out);
  static bool CaseFold(cstring Str, uint64 Length, FResult& Out);
  static bool NFKCCaseFold(cstring Str, uint64 Length, FResult& Out);
  static void Free(FResult& Result);

  static bool IsGraphic(FCodePoint CodePoint);
  static bool IsLetter(FCodePoint CodePoint);
  static bool IsNumber(FCodePoint CodePoint);
  static bool IsWhitespace(FCodePoint CodePoint);

  static bool Decode(cstring Str, uint64 Length, FCodePoint& OutCodePoint, uint64& OutBytes);

  static void TestUnicode();
};
