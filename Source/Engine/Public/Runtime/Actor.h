#pragma once
#include "Core/Core.h"

struct FArchetype;
struct FChuckStorage;

struct FActorEntry {
  FName name;
  FArchetype* archetype;
  FChuckStorage* chunk;
};

using AActor = THandle<FActorEntry>;
