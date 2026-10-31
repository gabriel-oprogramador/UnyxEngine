#include "Core/Name.h"
#include "Core/Array.h"
#include "Core/Unicode.h"

struct FNameEntry {
  char* str;
  uint64 hash;
  uint32 size;
  uint32 length;
};

struct FNameNode {
  uint32 entryIndex{0};
  uint32 distance{0};
};

struct FNamePool {
  FNamePool() {
    entries.Reserve(EntriesCapacity);
    nodes.Resize(EntriesCapacity);
    AddOrCreate("NoName");
  }

  cstring GetStr(uint32 Id) {
    if(Id == 0 || Id >= entries.Count()) {
      return "NoName";
    }
    return entries[Id].str;
  }

  uint32 GetSize(uint32 Id) {
    if(Id == 0 || Id >= entries.Count()) {
      return 0;
    }
    return entries[Id].size;
  }

  uint64 GetHash(uint32 Id) {
    if(Id == 0 || Id >= entries.Count()) {
      return 0;
    }
    return entries[Id].hash;
  }

  FName AddOrCreate(cstring Str) {
    uint32 size = std::strlen(Str);
    FUnicode::FResult result{};
    FUnicode::NormalizeNFC(Str, size, result);
    Str = static_cast<cstring>(result.data);
    size = result.size;

    uint64 hash = MakeHash(Str, size);
    uint32 index = static_cast<uint32>(hash & (EntriesCapacity - 1));
    FNameNode* node = Find(index, Str, size, hash);

    if(node) {
      return FName{node->entryIndex};
    }

    UE_CHECK(count < EntriesCapacity);

    FNameEntry& newEntry = entries.Add();
    newEntry.size = size;
    newEntry.hash = hash;
    newEntry.length = static_cast<uint32>(FUnicode::CodePointCount(Str, size));
    newEntry.str = static_cast<char*>(FMemory::Alloc(size + 1));
    newEntry.str[size] = '\0';
    FMemory::CopyBytes((void*)newEntry.str, (void*)Str, size);

    uint32 newEntryIndex = count;
    uint32 entryIndex = newEntryIndex;
    uint32 distance = 0;

    while(true) {
      node = &nodes[index];
      if(node->entryIndex == InvalidEntry) {
        node->entryIndex = entryIndex;
        node->distance = distance;
        break;
      }
      if(distance > node->distance) {
        FMemory::Swap(entryIndex, node->entryIndex);
        FMemory::Swap(distance, node->distance);
      }
      index = (index + 1) & (EntriesCapacity - 1);
      ++distance;
    }

    count++;
    return FName{newEntryIndex};
  }

private:
  FNameNode* Find(uint32 Index, cstring Str, uint32 Size, uint64 Hash) {
    uint32 distance = 0;
    while(true) {
      FNameNode& node = nodes[Index];
      if(node.entryIndex == InvalidEntry) {
        return nullptr;
      }
      if(distance > node.distance) {
        return nullptr;
      }
      FNameEntry& entry = entries[node.entryIndex];
      if(entry.size == Size && entry.hash == Hash && FMemory::Compare(entry.str, Str, Size) == 0) {
        return &node;
      }
      Index = (Index + 1) & (EntriesCapacity - 1);
      ++distance;
    }
  };

  uint64 MakeHash(cstring Str, uint32 Size) {
    uint64 hash = 14695981039346656037ull;
    const uint8* ptr = reinterpret_cast<const uint8*>(Str);
    for(uint32 i = 0; i < Size; ++i) {
      hash ^= ptr[i];
      hash *= 1099511628211ull;
    }
    return hash;
  }

  static constexpr uint32 EntriesCapacity = 4096;
  static constexpr uint32 InvalidEntry = 0;
  TArray<FNameEntry> entries{EntriesCapacity};
  TArray<FNameNode> nodes{EntriesCapacity};
  uint32 count{0};
};

static FNamePool& GetNamePool() {
  static FNamePool pool{};
  return pool;
}

FName FName::FromStr(cstring Str) {
  return GetNamePool().AddOrCreate(Str);
}

cstring FName::ToStr() const {
  return GetNamePool().GetStr(id);
}

uint64 FName::Hash() const {
  return GetNamePool().GetHash(id);
}
