#include "Core/Name.h"
#include "Core/Unicode.h"
#include "Core/Arena.h"
#include "Core/Log.h"
#include <cstring>

static constexpr uint32 MAX_NAMES_ENTRIES = 8192;
static constexpr uint32 HASH_SIZE = 4096;
static constexpr uint32 INVALID = UINT32_MAX;

struct FNameEntry {
  uint64 hash;
  uint32 offset;
  uint32 length;
};

struct FHashNode {
  uint32 next;
};

struct FNamePool {
  FNameEntry entries[MAX_NAMES_ENTRIES];
  FHashNode nodes[MAX_NAMES_ENTRIES];
  uint32 buckets[HASH_SIZE];
  FArena strings;
  uint32 count = 0;

  FNamePool() {
    for(uint32 i = 0; i < HASH_SIZE; ++i) {
      buckets[i] = INVALID;
    }
    if(!strings.Resize(256 * 1024)) {
      return;
    }
    count = 0;
    FindOrAdd("NoName", 7);
  }

  cstring GetString(uint32 Id) {
    if(Id >= count) {
      return "NoName";
    }
    return strings.Get<const char>(entries[Id].offset);
  }
  uint64 Hash(uint32 Id) const {
    if(Id >= count) {
      return 0;
    }
    return entries[Id].hash;
  }

  uint32 Length(uint32 Id) const {
    if(Id >= count) {
      return 0;
    }
    return entries[Id].length;
  }

  constexpr uint64 MakeHash(const char* str) {
    uint64 hash = 14695981039346656037ull;
    while(*str) {
      hash ^= static_cast<uint8>(*str++);
      hash *= 1099511628211ull;
    }
    return hash;
  }

  FName FindOrAdd(cstring Str, uint64 Length) {
    if(!Str || Str[0] == '\0') {
      return FName((uint32)0);
    }

    uint64 hash = MakeHash(Str);
    uint32 length = Length;
    uint32 bucket = hash % HASH_SIZE;
    uint32 node = buckets[bucket];

    while(node != INVALID) {
      FNameEntry& entry = entries[node];
      if(entry.hash == hash && entry.length == length) {
        cstring name = strings.Get<char>(entry.offset);
        if(std::memcmp(name, Str, length) == 0) {
          return FName(node);
        }
      }
      node = nodes[node].next;
    }

    UE_CHECK(count < MAX_NAMES_ENTRIES);
    if(count >= MAX_NAMES_ENTRIES) {
      return FName((uint32)0);
    }
    uint32 id = count++;
    uint32 offset = strings.used;
    char* dst = static_cast<char*>(strings.PushSize(length + 1, alignof(char)));
    UE_CHECK(dst);
    if(!dst) {
      count--;
      return FName((uint32)0);
    }

    std::memcpy(dst, Str, length);
    dst[length] = '\0';

    entries[id].hash = hash;
    entries[id].offset = offset;
    entries[id].length = length;

    nodes[id].next = buckets[bucket];
    buckets[bucket] = id;

    return FName(id);
  }
};

static FNamePool& GetNamePool() {
  static FNamePool pool{};
  return pool;
}

FName FName::FromStr(cstring Str) {
  uint64 length = FUnicode::ByteLength(Str);
  if(FUnicode::IsASCII(Str, length)) {
    return GetNamePool().FindOrAdd(Str, length);
  }
  FUnicode::FResult result{};
  if(!FUnicode::NormalizeNFC(Str, length, result)) {
    return FName();
  }
  FName name = GetNamePool().FindOrAdd((cstring)result.data, result.size);
  FUnicode::Free(result);
  return name;
}

cstring FName::ToStr() const {
  return GetNamePool().GetString(this->runtimeID);
}

uint64 FName::Hash() const {
  return GetNamePool().Hash(this->runtimeID);
}

void FName::TestUnicode() {
  const char* A = "\xC3\xA9";   // é NFC
  const char* B = "e\xCC\x81";  // e + combining acute
  FName NameA = FName::FromStr(A);
  FName NameB = FName::FromStr(B);
  UE_ALERT("A: %s ID:%u", NameA.ToStr(), NameA.GetID());
  UE_ALERT("B: %s ID:%u", NameB.ToStr(), NameB.GetID());
  UE_ASSERT(NameA == NameB);
  UE_ASSERT(std::strcmp(NameA.ToStr(), "\xC3\xA9") == 0);
  UE_ASSERT(NameA.Hash() == NameB.Hash());
}
