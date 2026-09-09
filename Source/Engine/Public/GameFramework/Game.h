#pragma once
#include "Core/BaseTypes.h"

struct FVector2;

struct ENGINE_API FGame {
  static uint32 WindowWidth();
  static uint32 WindowHeight();
  static FVector2 WindowSize();
  static void ToggleFullscreen();
  static void QuitGame();
};
