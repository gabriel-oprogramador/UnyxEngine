#pragma once
#include "Core/Core.h"
#include "Core/HandleList.h"
#include "Actor.h"

struct FVector2;
struct FSchedule;

struct FWorld final {
  FWorld(const FWorld& Other) = delete;
  FWorld(FWorld&& Other) = delete;
  FWorld& operator=(const FWorld& Other) = delete;
  FWorld& operator=(FWorld&& Other) = delete;

  FVector2 GetWindowSize();
  bool IsFullscreen();
  void SetFullscreen(bool bFullscreen);
  void QuitGame();

private:
  friend class FApp;
  FWorld() = default;
  void Initialize();
  void Terminate();
  void BeginPlay(const FSchedule& Schedule);
  void EndPlay(const FSchedule& Schedule);
  void Update(const FSchedule& Schedule);
};
