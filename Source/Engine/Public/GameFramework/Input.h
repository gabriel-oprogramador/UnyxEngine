#pragma once
#include "Core/Core.h"

struct FVector2;

struct FKey {
  FName keyName;
  EKeyCode keyCode;
};

struct FInput {
  static FKey GetKeyByName(FName Name);
  static FKey GetKeyByCode(EKeyCode KeyCode);
  static bool IsKeyPressed(EKeyCode KeyCode);
  static bool IsKeyReleased(EKeyCode KeyCode);
  static bool IsKeyRepeat(EKeyCode KeyCode);
  static bool IsMouseCaptured();
  static void CaptureMouse(bool bCapture);
  static void ToggleCaptureMouse();
  static FVector2 GetMousePos();
  static FVector2 GetMouseDelta();
  static FVector2 GetMouseScroll();
};
