#pragma once
#include "Core/Core.h"
#include "Runtime/World.h"
#include "Runtime/Module.h"
#include "GameFramework/Input.h"
#include "GameFramework/Time.h"

struct ENGINE_API FGame {
  static uint32 WindowWidth();
  static uint32 WindowHeight();
  static FVector2 WindowSize();
  static void ToggleFullscreen();
  static void QuitGame();

  static FInput& GetInput() {
    return Input;
  }

  static FTime& GetTime() {
    return Time;
  }

  static FWorld& GetWorld() {
    return World;
  }

  static FModule& GetModule() {
    return Module;
  }

private:
  friend struct FApp;
  static inline FInput Input;
  static inline FTime Time;
  static inline FWorld World;
  static inline FModule Module{FName("Game")};
};
