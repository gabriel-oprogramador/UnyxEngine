#pragma once
#include "Core/Core.h"
#include "Math/Math.h"

struct PEvent;

struct FKey {
  FName keyName;
  EKeyCode keyCode;
};

struct FInputState {
  bool keys[KEY_MAX];
  FVector2 mouseDelta;
  FVector2 mouseScroll;
  FVector2 mousePos;
};

struct ENGINE_API FInput {
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

private:
  friend struct FApp;
  static inline FInputState CurrentState;
  static inline FInputState PreviousState;

  FInput() = delete;
  ~FInput() = delete;
  static void Update();
  static bool ProcessEvent(const PEvent& InputEvent);
};
