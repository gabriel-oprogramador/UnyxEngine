#pragma once
#include "Core/BaseTypes.h"
#include "Math/Vector2.h"

struct FRenderer;

struct FInputState {
  bool keys[KEY_MAX];
  FVector2 mouseDelta;
  FVector2 mouseScroll;
  FVector2 mousePos;
};

struct FInputContext {
  FInputState currentState;
  FInputState previousState;
};

struct FTimeContext {
  float startTime;
  float deltaTime;
  uint32 framerate;
};

struct FEngine final {
  inline FInputContext& GetInputContext() {
    return inputContext;
  }
  inline FTimeContext& GetTimeContext() {
    return timeContext;
  }

private:
  FRenderer* renderer{nullptr};
  FInputContext inputContext{};
  FTimeContext timeContext{};
  friend struct FApp;
  void Initialize();
  void Terminate();
  void Update();
  void Render();
};

extern FEngine GEngine;
