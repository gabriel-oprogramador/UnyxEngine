#include "Core/Name.h"
#include "Core/Map.h"
#include "Core/Arena.h"
#include "Core/Unicode.h"
#include "Core/StringView.h"
#include <algorithm>

struct FNameEntry {
  cstring str;
  uint64 hash;
  uint32 size;
  uint32 length;
};

struct FNamePool {
  static constexpr uint64 DefaultArenaSize = 256 * 1024;
  TMap<FStringView, uint32> table;
  TArray<FNameEntry> entries;
  TArray<FArena> arenas;
  uint32 count{0};
  FNamePool() {
    table.Reserve(4096);
    entries.Reserve(4096);
    arenas.Emplace(DefaultArenaSize);
    FStringView key = AddString(FStringView("NoName"));
    entries.Add(FNameEntry{key.Data(), key.Hash(), key.Size(), key.Length()});
    table.Add(key, count);
    ++count;
  }

  FStringView AddString(const FStringView& View) {
    uint32 size = View.Size();
    for(auto& arena : arenas) {
      char* data = arena.Push<char>(size + 1);
      if(data) {
        FMemory::CopyBytes(data, View.Data(), size);
        data[size] = '\0';
        return FStringView(data, size, View.Length());
      }
    }
    uint64 capacity = std::max(DefaultArenaSize, FMemory::NextPowerOfTwo(size + 1));
    FArena& arena = arenas.Emplace(capacity);
    char* data = arena.Push<char>(size + 1);
    UE_ASSERT(data);
    FMemory::CopyBytes(data, View.Data(), size);
    data[size] = '\0';
    return FStringView(data, size, View.Length());
  }

  FName AddOrCreate(const FStringView& String) {
    FStringView view = String;
    FUnicode::FResult result{};
    FUnicode::NormalizeNFC(String.Data(), String.Size(), result);
    if(result.transformed) {
      view = FStringView(static_cast<char*>(result.data));
    }

    uint32* value = table.Find(view);
    if(value) {
      FUnicode::Free(result);
      return FName{*value};
    }

    uint32 id = count;
    FStringView key = AddString(view);
    FUnicode::Free(result);

    entries.Add(FNameEntry{key.Data(), key.Hash(), key.Size(), key.Length()});
    table.Add(key, id);
    ++count;

    return FName{id};
  }

  cstring GetStr(const FName& Name) const {
    uint32 id = Name.id;
    if(id >= count) {
      return "NoName";
    }
    return entries[id].str;
  }

  uint64 GetHash(const FName& Name) const {
    uint32 id = Name.id;
    if(id >= count) {
      return 0;
    }
    return entries[id].hash;
  }
};

static FNamePool& GetNamePool() {
  static FNamePool pool{};
  return pool;
}

FName FName::FromStr(cstring Str) {
  return GetNamePool().AddOrCreate(FStringView(Str));
}

cstring FName::ToStr() const {
  return GetNamePool().GetStr(*this);
}

uint64 FName::Hash() const {
  return GetNamePool().GetHash(*this);
}
