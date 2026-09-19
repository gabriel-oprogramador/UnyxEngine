#pragma once
#include "Core/Array.h"

#include <functional>
#include <initializer_list>

template<typename TKey, typename TValue>
struct TPair {
  TKey key;
  TValue value;
};

// TODO: Replace TArray entries with TChunkStorage.
// Current TArray can reallocate and move FEntry objects, invalidating
// references/pointers to map values. Chunks keep existing entries at
// stable addresses while allowing new entries to be allocated without
// moving previous ones. Keep at least one chunk allocated and release
// completely free trailing chunks.
template<typename TKey, typename TValue>
struct TMap {
  TMap() = default;

  explicit TMap(uint32 Capacity) {
    Reserve(Capacity);
  }

  TMap(std::initializer_list<TPair<TKey, TValue>> Initializer) {
    Reserve(static_cast<uint32>(Initializer.size()));
    for(const TPair<TKey, TValue>& Pair : Initializer) {
      Add(Pair.key, Pair.value);
    }
  }

  uint32 Count() const {
    return count;
  }

  uint32 Capacity() const {
    return buckets.Count();
  }

  bool Empty() const {
    return count == 0;
  }

  void Clear() {
    for(auto& bucket : buckets) {
      bucket.entryIndex = InvalidIndex;
      bucket.distance = 0;
    }
    entries.Clear();
    count = 0;
    freeIndex = InvalidIndex;
  }

  void Reserve(uint32 NewCount) {
    uint32 requiredCapacity = static_cast<uint32>(NewCount / LoadFactor) + 1;
    requiredCapacity = NextPowerOfTwo(requiredCapacity);
    if(requiredCapacity <= buckets.Count()) {
      return;
    }
    Rehash(requiredCapacity);
  }

  bool Contains(const TKey& Key) const {
    return Find(Key) != nullptr;
  }

  bool Add(const TKey& Key, const TValue& Value) {
    return AddEntry(Key, Value);
  }

  bool Add(const TKey& Key, TValue&& Value) {
    return AddEntry(Key, std::move(Value));
  }

  const TValue* Find(const TKey& Key) const {
    if(buckets.Empty()) {
      return nullptr;
    }
    uint64 hash = std::hash<TKey>{}(Key);
    uint32 index = static_cast<uint32>(hash) & (buckets.Count() - 1);
    uint32 distance = 0;
    while(true) {
      const FBucket& bucket = buckets[index];
      if(bucket.entryIndex == InvalidIndex) {
        return nullptr;
      }
      if(distance > bucket.distance) {
        return nullptr;
      }
      const FEntry& entry = entries[bucket.entryIndex];
      if(entry.key == Key) {
        return &entry.value;
      }
      index = (index + 1) & (buckets.Count() - 1);
      ++distance;
    }
  }

  TValue* Find(const TKey& Key) {
    if(buckets.Empty()) {
      return nullptr;
    }
    uint64 hash = std::hash<TKey>{}(Key);
    uint32 index = static_cast<uint32>(hash) & (buckets.Count() - 1);
    uint32 distance = 0;
    while(true) {
      FBucket& bucket = buckets[index];
      if(bucket.entryIndex == InvalidIndex) {
        return nullptr;
      }
      if(distance > bucket.distance) {
        return nullptr;
      }
      FEntry& entry = entries[bucket.entryIndex];
      if(entry.key == Key) {
        return &entry.value;
      }
      index = (index + 1) & (buckets.Count() - 1);
      ++distance;
    }
  }

  bool Remove(const TKey& Key) {
    if(buckets.Empty()) {
      return false;
    }
    uint64 hash = std::hash<TKey>{}(Key);
    uint32 mask = buckets.Count() - 1;
    uint32 index = static_cast<uint32>(hash) & mask;
    uint32 distance = 0;
    while(true) {
      FBucket& bucket = buckets[index];
      if(bucket.entryIndex == InvalidIndex) {
        return false;
      }
      if(distance > bucket.distance) {
        return false;
      }
      FEntry& entry = entries[bucket.entryIndex];
      if(entry.key == Key) {
        break;
      }
      index = (index + 1) & mask;
      ++distance;
    }
    uint32 removedEntry = buckets[index].entryIndex;
    uint32 nextIndex = (index + 1) & mask;
    while(buckets[nextIndex].entryIndex != InvalidIndex && buckets[nextIndex].distance > 0) {
      buckets[index].entryIndex = buckets[nextIndex].entryIndex;
      buckets[index].distance = buckets[nextIndex].distance - 1;
      index = nextIndex;
      nextIndex = (index + 1) & mask;
    }
    buckets[index].entryIndex = InvalidIndex;
    buckets[index].distance = 0;
    entries[removedEntry].nextFree = freeIndex;
    freeIndex = removedEntry;
    --count;
    return true;
  }

  template<typename TFunction>
  void ForEach(TFunction&& Function) {
    for(const FBucket& Bucket : buckets) {
      if(Bucket.entryIndex == InvalidIndex) {
        continue;
      }
      FEntry& Entry = entries[Bucket.entryIndex];
      Function(static_cast<const TKey&>(Entry.key), Entry.value);
    }
  }

private:
  static constexpr uint32 InvalidIndex = UINT32_MAX;
  static constexpr float LoadFactor = 0.75f;

  struct FBucket {
    uint32 entryIndex{InvalidIndex};
    uint32 distance{0};
  };

  struct FEntry {
    TKey key;
    TValue value;
    uint32 nextFree{InvalidIndex};
  };

  TArray<FBucket> buckets;
  TArray<FEntry> entries;
  uint32 count{0};
  uint32 freeIndex{InvalidIndex};

  template<typename TValueArg>
  bool AddEntry(const TKey& Key, TValueArg&& Value) {
    if(Contains(Key)) {
      return false;
    }
    if(count + 1 > static_cast<uint32>(buckets.Count() * LoadFactor)) {
      Reserve(count + 1);
    }

    uint32 entryIndex = entries.Count();
    if(freeIndex != InvalidIndex) {
      entryIndex = freeIndex;
      FEntry& entry = entries[entryIndex];
      freeIndex = entry.nextFree;
      entry.key = Key;
      entry.value = std::forward<TValueArg>(Value);
      entry.nextFree = InvalidIndex;
    } else {
      entryIndex = entries.Count();
      entries.Emplace(FEntry{Key, std::forward<TValueArg>(Value)});
    }

    uint32 distance = 0;
    uint64 hash = std::hash<TKey>{}(Key);
    uint32 index = static_cast<uint32>(hash) & (buckets.Count() - 1);

    while(true) {
      FBucket& bucket = buckets[index];
      if(bucket.entryIndex == InvalidIndex) {
        bucket.entryIndex = entryIndex;
        bucket.distance = distance;
        break;
      }
      if(distance > bucket.distance) {
        FMemory::Swap(entryIndex, bucket.entryIndex);
        FMemory::Swap(distance, bucket.distance);
      }
      index = (index + 1) & (buckets.Count() - 1);
      ++distance;
    }
    ++count;
    return true;
  }

  void Rehash(uint32 NewCapacity) {
    TArray<FBucket> NewBuckets;
    NewBuckets.Resize(NewCapacity);
    for(uint32 c = 0; c < buckets.Count(); c++) {
      uint32 entryIndex = buckets[c].entryIndex;
      if(entryIndex == InvalidIndex) {
        continue;
      }
      uint64 hash = std::hash<TKey>{}(entries[entryIndex].key);
      uint32 index = static_cast<uint32>(hash) & (NewCapacity - 1);
      uint32 distance = 0;
      while(true) {
        FBucket& bucket = NewBuckets[index];
        if(bucket.entryIndex == InvalidIndex) {
          bucket.entryIndex = entryIndex;
          bucket.distance = distance;
          break;
        }
        if(distance > bucket.distance) {
          FMemory::Swap(entryIndex, bucket.entryIndex);
          FMemory::Swap(distance, bucket.distance);
        }
        index = (index + 1) & (NewCapacity - 1);
        ++distance;
      }
    }
    buckets = std::move(NewBuckets);
  }

  static uint32 NextPowerOfTwo(uint32 Value) {
    UE_ASSERT(Value > 0);
    UE_ASSERT(Value <= (uint32(1) << 31));
    --Value;
    Value |= Value >> 1;
    Value |= Value >> 2;
    Value |= Value >> 4;
    Value |= Value >> 8;
    Value |= Value >> 16;
    return Value + 1;
  }
};
