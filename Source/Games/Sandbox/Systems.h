#pragma once
#include <CoreMinimal.h>

struct FSetupSystem {
  static void BeginPlay(FWorld& World);
};

struct FEditorSystem {
  static void BeginPlay(FWorld& World);
  static void EndPlay(FWorld& World);
  static void Update(FWorld& World);
};
