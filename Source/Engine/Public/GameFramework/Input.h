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
  FInput() = default;
  FInput(const FInput&) = delete;
  FInput& operator=(const FInput&) = delete;
  FInput(FInput&&) = delete;
  FInput& operator=(FInput&&) = delete;

  FKey GetKeyByName(FName Name);
  FKey GetKeyByCode(EKeyCode KeyCode);
  bool IsKeyPressed(EKeyCode KeyCode);
  bool IsKeyReleased(EKeyCode KeyCode);
  bool IsKeyRepeat(EKeyCode KeyCode);
  bool IsMouseCaptured();
  void CaptureMouse(bool bCapture);
  void ToggleCaptureMouse();
  FVector2 GetMousePos();
  FVector2 GetMouseDelta();
  FVector2 GetMouseScroll();

private:
  friend struct FApp;
  FInputState currentState;
  FInputState previousState;

  void Update();
  bool ProcessEvent(const PEvent& InputEvent);
};
