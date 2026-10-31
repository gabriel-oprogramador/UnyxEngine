#pragma once
#include "Core/Core.h"

struct ENGINE_API FTime {
  FTime() = default;
  FTime(const FTime&) = delete;
  FTime(FTime&&) = delete;
  FTime& operator=(const FTime&) = delete;
  FTime& operator=(FTime&&) = delete;

  double Time();

  float DeltaTime() {
    return deltaTime;
  }
  uint32 Framerate() {
    return framerate;
  }

private:
  friend struct FApp;
  float deltaTime = {0.f};
  uint32 framerate = 60;

  void Update();
};
