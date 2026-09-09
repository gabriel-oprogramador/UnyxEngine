#pragma once
#include "Core/Core.h"
#include "Core/HandleList.h"
#include "Core/Reflection.h"
#include "Actor.h"

struct FSchedule;

struct ENGINE_API FWorld final {
  FWorld(const FWorld& Other) = delete;
  FWorld(FWorld&& Other) = delete;
  FWorld& operator=(const FWorld& Other) = delete;
  FWorld& operator=(FWorld&& Other) = delete;

private:
  friend class FApp;
  FWorld() = default;
  void Initialize();
  void Terminate();
  void BeginPlay(const FSchedule& Schedule);
  void EndPlay(const FSchedule& Schedule);
  void Update(const FSchedule& Schedule);
};
