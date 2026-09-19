#pragma once
#include "Core/Core.h"
#include "Core/Log.h"

template<uint32 N>
struct TBitset {
  static_assert(N > 0, "TBitset size must be greater than zero.");

  static constexpr uint32 BitCount = N;
  static constexpr uint32 WordCount = (N + 63) / 64;

  uint64 words[WordCount] = {};

  void Clear() {
    for(uint32 i = 0; i < WordCount; ++i) {
      words[i] = 0;
    }
  }

  void Set(uint32 Index) {
    UE_ASSERT(Index < BitCount);
    if(Index >= BitCount) {
      return;
    }
    const uint32 word = Index / 64;
    const uint32 bit = Index % 64;
    words[word] |= uint64(1) << bit;
  }

  void Reset(uint32 Index) {
    UE_ASSERT(Index < BitCount);
    if(Index >= BitCount) {
      return;
    }
    const uint32 word = Index / 64;
    const uint32 bit = Index % 64;
    words[word] &= ~(uint64(1) << bit);
  }

  void Flip(uint32 Index) {
    UE_ASSERT(Index < BitCount);
    if(Index >= BitCount) {
      return;
    }
    const uint32 word = Index / 64;
    const uint32 bit = Index % 64;
    words[word] ^= uint64(1) << bit;
  }

  bool Has(uint32 Index) const {
    UE_ASSERT(Index < BitCount);
    if(Index >= BitCount) {
      return false;
    }
    const uint32 word = Index / 64;
    const uint32 bit = Index % 64;
    return (words[word] & (uint64(1) << bit)) != 0;
  }

  bool HasAll(const TBitset& Mask) const {
    for(uint32 i = 0; i < WordCount; ++i) {
      if((words[i] & Mask.words[i]) != Mask.words[i]) {
        return false;
      }
    }
    return true;
  }

  bool HasAny(const TBitset& Mask) const {
    for(uint32 i = 0; i < WordCount; ++i) {
      if((words[i] & Mask.words[i]) != 0) {
        return true;
      }
    }
    return false;
  }

  bool HasNone(const TBitset& Mask) const {
    for(uint32 i = 0; i < WordCount; ++i) {
      if((words[i] & Mask.words[i]) != 0) {
        return false;
      }
    }
    return true;
  }

  bool Equals(const TBitset& Other) const {
    for(uint32 i = 0; i < WordCount; ++i) {
      if(words[i] != Other.words[i]) {
        return false;
      }
    }
    return true;
  }

  TBitset And(const TBitset& Other) const {
    TBitset Result{};
    for(uint32 i = 0; i < WordCount; ++i) {
      Result.words[i] = words[i] & Other.words[i];
    }
    return Result;
  }

  void Or(const TBitset& Other) {
    for(uint32 i = 0; i < WordCount; ++i) {
      words[i] |= Other.words[i];
    }
  }

  void Xor(const TBitset& Other) {
    for(uint32 i = 0; i < WordCount; ++i) {
      words[i] ^= Other.words[i];
    }
  }

  uint32 Count() const {
    uint32 count = 0;
    for(uint32 i = 0; i < WordCount; ++i) {
      count += __builtin_popcountll(words[i]);
    }
    return count;
  }

  bool IsEmpty() const {
    for(uint32 i = 0; i < WordCount; ++i) {
      if(words[i] != 0) {
        return false;
      }
    }
    return true;
  }

  bool NextBit(uint32& Cursor, uint32& OutIndex) const {
    if(Cursor >= BitCount) {
      return false;
    }
    uint32 word = Cursor / 64;
    uint32 bit = Cursor % 64;
    for(uint32 i = word; i < WordCount; ++i) {
      uint64 bits = words[i];
      if(i == word) {
        bits &= ~uint64(0) << bit;
      }
      if(bits) {
        const uint32 foundBit = __builtin_ctzll(bits);
        OutIndex = i * 64 + foundBit;
        Cursor = OutIndex + 1;

        return true;
      }
      bit = 0;
    }
    Cursor = BitCount;
    return false;
  }

  bool PopNextBit(uint32& OutIndex) {
    for(uint32 i = 0; i < WordCount; ++i) {
      uint64 bits = words[i];
      if(bits) {
        const uint32 bit = __builtin_ctzll(bits);
        OutIndex = i * 64 + bit;
        words[i] = bits & (bits - 1);
        return true;
      }
    }
    return false;
  }
};
